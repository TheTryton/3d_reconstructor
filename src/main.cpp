
#include <vertexDetection.h>
#include <disparityMap.h>
#include <cameraCalibration.h>
#include <map>

#define CAMERA  0// "http://192.168.1.112:81/stream" //(for local Webcam)
// or something like: "http://192.168.0.111:81/stream" (for ESP32 cam)

#define VERTEX_WINDOW "Vertex test"
#define DISPARITY_TEST "Disparity test"
#define LEFT "Left"
#define RIGHT "Right"

#define FRAME_SKIP 5

cv::Rect roi;
cv::Point origin;
cv::Mat frame_size;
bool select;
bool apply_roi = false;

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

    params["numDisparities"] = 1;
    params["blockSize"] = 10;
    params["filterSize"] = 15;
    params["filterCap"] = 20;
    params["minDisparity"] = 0;
    params["textureThreshold"] = 0;
    params["uniquenessRatio"] = 8;
    params["speckleWindowSize"] = 0;
    params["speckleRange"] = 0;

    camera >> frame_right;
    frame_right.copyTo(frame_size);

    int frame_counter = 0;
    for(;;){
        if(frame_counter % FRAME_SKIP == 0) {
            camera >> frame;
            frame_right.copyTo(frame_left);
            frame.copyTo(frame_right);

            vertices_frame = vertex_detection(frame);
            disparity_frame = apply_roi ?
                              disparity_map(frame_left, frame_right, params, roi) :
                              disparity_map(frame_left, frame_right, params);

            cv::createTrackbar( "PreFilterSize", VERTEX_WINDOW, &params["filterSize"], 256, 0 );
            cv::createTrackbar( "PreFilterCap", VERTEX_WINDOW, &params["filterCap"], 63, 0 );
            cv::createTrackbar( "NumDisparities", VERTEX_WINDOW, &params["numDisparities"], 256, 0 );
            cv::createTrackbar( "BlockSize", VERTEX_WINDOW, &params["blockSize"], 256, 0 );
            cv::createTrackbar( "FilterSize", VERTEX_WINDOW, &params["filterSize"], 256, 0 );
            cv::createTrackbar( "MinDisparity", VERTEX_WINDOW, &params["minDisparity"], 256, 0 );
            cv::createTrackbar( "TextureThreshold", VERTEX_WINDOW, &params["textureThreshold"], 256, 0 );
            cv::createTrackbar( "UniquenessRatio", VERTEX_WINDOW, &params["uniquenessRatio"], 256, 0 );
            cv::createTrackbar( "SpeckleWindowSize", VERTEX_WINDOW, &params["speckleWindowSize"], 256, 0 );
            cv::createTrackbar( "SpeckleRange", VERTEX_WINDOW, &params["speckleRange"], 256, 0 );

            if( (select || apply_roi) && roi.width > 0 && roi.height > 0 ){
                cv::Mat select_rectangle(frame, roi);
                bitwise_not(select_rectangle, select_rectangle);
            }

            cv::imshow(VERTEX_WINDOW, vertices_frame);
            cv::imshow(DISPARITY_TEST, disparity_frame);
            cv::imshow(LEFT, frame_left);
            cv::imshow(RIGHT, frame);
        }
        frame_counter++;

        char keyboardInput = (char) cv::waitKey(10);
        if(keyboardInput == 27)
            break;
    }

    return 0;
}

