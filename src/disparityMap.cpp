//
// Created by micro on 21.04.2021.
//

#include "opencv2/videoio.hpp"
#include "opencv2/video/tracking.hpp"
#include "opencv2/calib3d/calib3d.hpp"
#include <map>


cv::Mat disparity_map(const cv::Mat& frame_left,
                      const cv::Mat& frame_right,
                      std::map<std::string, int>& params,
                      const cv::Rect roi = cv::Rect(0,0,0,0)){

    cv::Mat g1, g2, disparity;

    int sigma = 3;
    int k = (sigma*5) | 1;

    cv::resize(frame_left, g1, cv::Size(640, 360));
    cv::resize(frame_right, g2, cv::Size(640, 360));
    cv::GaussianBlur(g1, g1, cv::Size(k, k), sigma, sigma);
    cv::GaussianBlur(g2, g2, cv::Size(k, k), sigma, sigma);

    cv::cvtColor(g1, g1, cv::COLOR_BGR2GRAY);
    cv::cvtColor(g2, g2, cv::COLOR_BGR2GRAY);
    auto sbm = cv::StereoBM::create(16 * params["numDisparities"], params["blockSize"]);    // TODO: dynamic parametrization - toolbars and stuff
//    auto sbm = cv::StereoBM::create(80, 5);
//
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