//
// Created by micro on 07.04.2021.
//

#include "opencv2/videoio.hpp"
#include "opencv2/video/tracking.hpp"
#include <iostream>


cv::Mat vertex_detection(const cv::Mat& frame){

    cv::Mat vertices_frame, edges;
    cv::RNG rng(12345);
    std::vector<std::vector<cv::Point>> contours;
    std::vector<cv::Vec4i> hierarchy;
    int sigma = 3;
    int k = (sigma*5)|1;

    frame.copyTo(vertices_frame);

    cv::cvtColor(vertices_frame, vertices_frame, cv::COLOR_RGB2GRAY);
    cv::GaussianBlur(vertices_frame, vertices_frame, cv::Size(k, k), sigma, sigma);
    cv::threshold(vertices_frame, vertices_frame, 0, 255, cv::THRESH_BINARY + cv::THRESH_OTSU);
    cv::Canny(vertices_frame, edges, 1, 3);
    vertices_frame.copyTo(vertices_frame, edges);
    cv::findContours(vertices_frame, contours, hierarchy, cv::RETR_TREE, cv::CHAIN_APPROX_SIMPLE);

   std::vector<std::vector<cv::Point>> finalContours;
   finalContours.resize(contours.size());
   for(size_t i = 0; i < contours.size(); i++){
        cv::approxPolyDP(cv::Mat(contours[i]), finalContours[i], 3, true);
        cv::Scalar color = cv::Scalar(rng.uniform(0,256), rng.uniform(0,256),rng.uniform(0,256));
        for(const auto& point : finalContours[i])
            cv::drawMarker(vertices_frame, point, color, cv::MARKER_SQUARE, 10);
        cv::drawContours(vertices_frame, finalContours, (int) i, color, 2, cv::LINE_8, hierarchy, 0);
    }
//
//               std::cout << "Contour: ";
//               for (const auto &point : finalContours[0])
//                   std::cout << point << " ";
//               std::cout << "\n";
//           }
//
//





    return vertices_frame;
}