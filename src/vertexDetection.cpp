//
// Created by micro on 07.04.2021.
//

#include "opencv.h"
#include "opencv2/videoio.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/core/utility.hpp"
#include "opencv2/video/tracking.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"
#include <iostream>

#define CAMERA_NUM 0

int opencv_vertex_detection(int argc, char* argv[]){
    cv::VideoCapture camera;
    camera.open(CAMERA_NUM);
    cv::namedWindow("Vertex test", 0);
    cv::namedWindow("Clean camera", 0);
    cv::Mat frame, edges, cleanFrame;
    cv::RNG rng(12345);
    std::vector<std::vector<cv::Point>> contours;
    std::vector<cv::Vec4i> hierarchy;
    int sigma = 3;
    int k = (sigma*5)|1;
    int frame_counter = 0;
    int frame_skip = 60;

    for(;;){
        camera >> frame;
        frame.copyTo(cleanFrame);
        cv::cvtColor(frame, frame, cv::COLOR_RGB2GRAY);
        cv::GaussianBlur(frame, frame, cv::Size(k,k), sigma, sigma);
        cv::threshold(frame, frame, 0, 255, cv::THRESH_BINARY + cv::THRESH_OTSU);
        cv::Canny(frame, edges, 1,3);
        frame.copyTo(frame, edges);
        cv::findContours(frame, contours, hierarchy, cv::RETR_TREE, cv::CHAIN_APPROX_SIMPLE);

       std::vector<std::vector<cv::Point>> finalContours;
       finalContours.resize(contours.size());
       for(size_t i = 0; i < contours.size(); i++){
            cv::approxPolyDP(cv::Mat(contours[i]), finalContours[i], 3, true);
            cv::Scalar color = cv::Scalar(rng.uniform(0,256), rng.uniform(0,256),rng.uniform(0,256));
            for(const auto& point : finalContours[i])
                cv::drawMarker(cleanFrame, point, color, cv::MARKER_SQUARE, 10);
            cv::drawContours(cleanFrame, finalContours, (int) i, color, 2, cv::LINE_8, hierarchy, 0);
        }
       if(frame_counter % frame_skip == 0){
               std::cout << "Contour: ";
               for (const auto &point : finalContours[0])
                   std::cout << point << " ";
               std::cout << "\n";
           }

       frame_counter++;

       cv::imshow("Vertex test", frame);
       cv::imshow("Clean camera", cleanFrame);

        char keyboardInput = (char) cv::waitKey(10);
        if(keyboardInput == 27)
            break;
    }
    return 0;
}