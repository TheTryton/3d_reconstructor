
#include <vertexDetection.h>
#include <disparityMap.h>
#include <cameraCalibration.h>
#include <map>

#define CAMERA  0 //"http://192.168.1.114:81/stream" (for local Webcam)
// or something like: "http://192.168.0.111:81/stream" (for ESP32 cam)

#define VERTEX_WINDOW "Vertex test"
#define DISPARITY_TEST "Disparity test"
#define LEFT "Left"
#define RIGHT "Right"

#define FRAME_SKIP 30

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
    cv::Mat frame, vertices_frame, disparity_frame,
            frame_left, frame_right;

    camera >> frame_right;

    int frame_counter = 0;
    for(;;){
        if(frame_counter % FRAME_SKIP == 0) {
            camera >> frame;
            frame_right.copyTo(frame_left);
            frame.copyTo(frame_right);
            vertices_frame = vertex_detection(frame);
            disparity_frame = disparity_map(frame_left, frame_right, params);

            cv::createTrackbar( "PreFilterSize", VERTEX_WINDOW, &params["filterCap"], 256, 0 );
            cv::createTrackbar( "NumDisparities", VERTEX_WINDOW, &params["numDisparities"], 256, 0 );
            cv::createTrackbar( "BlockSize", VERTEX_WINDOW, &params["blockSize"], 256, 0 );
            cv::createTrackbar( "FilterSize", VERTEX_WINDOW, &params["filterSize"], 256, 0 );
            cv::createTrackbar( "MinDisparity", VERTEX_WINDOW, &params["minDisparity"], 256, 0 );
            cv::createTrackbar( "TextureThreshold", VERTEX_WINDOW, &params["textureThreshold"], 256, 0 );
            cv::createTrackbar( "UniquenessRatio", VERTEX_WINDOW, &params["uniquenessRatio"], 256, 0 );
            cv::createTrackbar( "SpeckleWindowSize", VERTEX_WINDOW, &params["speckleWindowSize"], 256, 0 );
            cv::createTrackbar( "SpeckleRange", VERTEX_WINDOW, &params["speckleRange"], 256, 0 );
//            cv::createTrackbar( "Roi1", DISPARITY_TEST, &params["roi1"], 256, 0 );

            cv::imshow(VERTEX_WINDOW, vertices_frame);
            cv::imshow(DISPARITY_TEST, disparity_frame);
            cv::imshow(LEFT, frame_left);
            cv::imshow(RIGHT, frame_right);
        }
        frame_counter++;

        char keyboardInput = (char) cv::waitKey(10);
        if(keyboardInput == 27)
            break;
    }

    return 0;
}

