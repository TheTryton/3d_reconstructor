
#include <vertexDetection.h>
#include <disparityMap.h>
#include <math.h>
#include <map>
#include <chrono>
#include "opencv2/calib3d.hpp"

#define CAMERA "http://192.168.0.101:81/stream" //(for local Webcam)
// or something like: "http://192.168.0.111:81/stream" (for ESP32 cam)

#define VERTEX_WINDOW "Vertex test"
#define DISPARITY_TEST "Disparity test"
#define LEFT "Left"
#define RIGHT "Right"

#define EPS 1e6

#define R 19.5
#define TRAIN_HEIGHT 10.5
#define LAP_TIME 9.134

cv::Mat CAMERA_MATRIX = (cv::Mat_<double>(3,3) <<
        1.2213483657167785e+03, 0,                      5.2038106031345080e+02,
        0,                      1.2227670297198733e+03, 3.9953361672646463e+02,
        0,                      0,                      1);

cv::Mat T = (cv::Mat_<double>(4,4) <<
        1, 0, 0, R,
        0, 1, 0, TRAIN_HEIGHT,
        0, 0, 1, 0,
        0, 0, 0, 1);

cv::Mat Q = (cv::Mat_<double>(4,4) <<
        1,  0,  0,              -5.62774048e+02,
        0,  1,  0,              -3.84172371e+02,
        0,  0,  0,              1.22276703e+03,
        0,  0,  9.22664093e-01, 0);

cv::Rect roi;
cv::Point origin;
cv::Mat frame_size;
bool select;
bool apply_roi = false;
float travel_time = 0.f;
int capture_motion = 0;

static void onButton( int state, void* userData) {
    if(state == -1)
        capture_motion = !capture_motion;
}

// User draws box around object to track. This triggers CAMShift to start tracking
static void onMouse( int event, int x, int y, int, void* ) {
    if (select){
        roi.x = MIN(x, origin.x);
        roi.y = MIN(y, origin.y);
        roi.width = std::abs(x - origin.x);
        roi.height = std::abs(y - origin.y);
        roi &= cv::Rect(0, 0, frame_size.cols, frame_size.rows);
    }
    switch( event )
    {
        case cv::EVENT_LBUTTONDOWN:
            origin = cv::Point(x,y);
            roi  = cv::Rect(x,y,0,0);
            select = true;
            apply_roi = false;
            break;
        case cv::EVENT_LBUTTONUP:
            select = false;
            if( roi.width > 0 && roi.height > 0 )
                apply_roi = true;
            break;
    }
}

bool equal(const cv::Mat & a, const cv::Mat & b)
{
    if ( (a.rows != b.rows) || (a.cols != b.cols) )
        return false;
    cv::Scalar s = sum( a - b );
    //std::cout << s << '\n';

    return (s[0] <= EPS) || (s[1] <= EPS) || (s[2] <= EPS);
}

int main(int argc, char* argv[])
{
//    mainCalibration(argc,argv);
    std::map<std::string, int> params;

    cv::VideoCapture camera;
    camera.open(CAMERA);
    cv::namedWindow(VERTEX_WINDOW, 0);
    cv::namedWindow(DISPARITY_TEST, 0);
    cv::namedWindow(LEFT, 0);
    cv::namedWindow(RIGHT, 0);
    cv::setMouseCallback(RIGHT, onMouse, 0);
    cv::Mat frame, vertices_frame, disparity_frame,
            frame_left, frame_right;

    params["numDisparities"] = 2;
    params["blockSize"] = 20;
    params["filterSize"] = 15;
    params["filterCap"] = 20;
    params["minDisparity"] = 0;
    params["textureThreshold"] = 0;
    params["uniquenessRatio"] = 8;
    params["speckleWindowSize"] = 79;
    params["speckleRange"] = 51;

    camera >> frame_right;
    frame_right.copyTo(frame_size);

    int frame_counter = 0;
    auto start = std::chrono::steady_clock::now();
    auto end = std::chrono::steady_clock::now();
    float theta = 0.f;
    cv::Mat model_transform;

    for(;;){
        start = end;
        camera >> frame;
        end = std::chrono::steady_clock::now();
        frame_right.copyTo(frame_left);
        frame.copyTo(frame_right);

        if (capture_motion == 1 && !equal(frame_left, frame_right)) {
            travel_time += std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() / 1000.0f;
            vertices_frame = vertex_detection(frame);
            disparity_frame = apply_roi ?
                              disparity_map(frame_left, frame_right, params, roi) :
                              disparity_map(frame_left, frame_right, params);

            theta = calculate_rotation_angle(travel_time, LAP_TIME);
            model_transform = create_model_transform_matrix(T, theta);
            //translate_to_3d and and points to model

            cv::imshow(VERTEX_WINDOW, vertices_frame);
            cv::imshow(DISPARITY_TEST, disparity_frame);
            frame_counter++;
        }

        std::cout << "Travel time: " << travel_time << '\n';

        cv::createTrackbar("button", RIGHT, &capture_motion, 1, 0);

        cv::createTrackbar( "PreFilterSize", VERTEX_WINDOW, &params["filterSize"], 256, 0 );
        cv::createTrackbar( "PreFilterCap", VERTEX_WINDOW, &params["filterCap"], 63, 0 );
        cv::createTrackbar( "NumDisparities", VERTEX_WINDOW, &params["numDisparities"], 256, 0 );
        cv::createTrackbar( "BlockSize", VERTEX_WINDOW, &params["blockSize"], 256, 0 );
        cv::createTrackbar( "FilterSize", VERTEX_WINDOW, &params["filterSize"], 256, 0 );
        cv::createTrackbar( "MinDisparity", VERTEX_WINDOW, &params["minDisparity"], 256, 0 );
        cv::createTrackbar( "TextureThreshold", VERTEX_WINDOW, &params["textureThreshold"], 256, 0 );
        cv::createTrackbar( "UniquenessRatio", VERTEX_WINDOW, &params["uniquenessRatio"], 256, nullptr );
        cv::createTrackbar( "SpeckleWindowSize", VERTEX_WINDOW, &params["speckleWindowSize"], 256, 0 );
        cv::createTrackbar( "SpeckleRange", VERTEX_WINDOW, &params["speckleRange"], 256, 0 );

        if( (select || apply_roi) && roi.width > 0 && roi.height > 0 ){
            cv::Mat select_rectangle(frame, roi);
            bitwise_not(select_rectangle, select_rectangle);
            //translate_to_3d(disparity_frame, frame_right, REPROJECTION_ERR_MATRIX);
        }

        cv::imshow(LEFT, frame_left);
        cv::imshow(RIGHT, frame);
        std::cout << "Frame count: " << frame_counter << '\n';

        char keyboardInput = (char) cv::waitKey(10);
        if(keyboardInput == 27)
            break;
    }

    return 0;
}

