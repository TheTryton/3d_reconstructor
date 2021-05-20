//
// Created by micro on 21.04.2021.
//

#include "opencv2/videoio.hpp"
#include "opencv2/video/tracking.hpp"
#include "opencv2/calib3d/calib3d.hpp"
#include <map>
#include <disparityMap.h>
#include <fstream>
#include <algorithm>

cv::Mat disparity_map(const cv::Mat& frame_left,
                      const cv::Mat& frame_right,
                      std::map<std::string, int>& params,
                      const cv::Rect& roi){

    cv::Mat g1, g2, disparity;

    int sigma = 3;
    int k = (sigma*5) | 1;

    cv::resize(frame_left, g1, cv::Size(640, 360));
    cv::resize(frame_right, g2, cv::Size(640, 360));
    cv::GaussianBlur(g1, g1, cv::Size(k, k), sigma, sigma);
    cv::GaussianBlur(g2, g2, cv::Size(k, k), sigma, sigma);

    cv::cvtColor(g1, g1, cv::COLOR_BGR2GRAY);
    cv::cvtColor(g2, g2, cv::COLOR_BGR2GRAY);
    auto sbm = cv::StereoBM::create(16 * params["numDisparities"], 2*params["blockSize"]+5);    // TODO: dynamic parametrization - toolbars and stuff

    sbm->setPreFilterSize(params["filterSize"]);
    sbm->setPreFilterCap(params["filterCap"]);
    sbm->setMinDisparity(params["minDisparity"]);
    sbm->setTextureThreshold(params["textureThreshold"]);
    sbm->setUniquenessRatio(params["uniquenessRatio"]);
    sbm->setSpeckleWindowSize(params["speckleWindowSize"]);
    sbm->setSpeckleRange(params["speckleRange"]);
    if(roi.area() > 0)
        sbm->setROI1(roi);

    sbm->compute(g1, g2, disparity);
    cv::normalize(disparity, disparity, 0, 255, cv::NORM_MINMAX, CV_8U);

    return disparity;
}

// TODO:
//  Change CGAL to o3d
//  Change Point class (CGAL)

std::vector<Point> translate_to_3d(const cv::Mat& disparity,
                                   const cv::Mat& frame,
                                   const cv::Mat& model_transform,
                                   const cv::Mat& Q){

    cv::Mat point_cloud;
    cv::reprojectImageTo3D(disparity, point_cloud, model_transform * Q);

    double min, max;
    cv::minMaxLoc(point_cloud, &min, &max);
    std::vector<Point> points, colors;

    for(size_t row = 0; row < point_cloud.rows; ++row)
        for(size_t col = 0; col < point_cloud.cols; ++col)
            if(point_cloud.at<int>(row, col) > min){
                points.push_back(Point(row, col, disparity.at<int>(row, col)));
                //colors.push_back(Point(row, col, frame.at<int>(row,col)));
            }

    return points;
}