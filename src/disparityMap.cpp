//
// Created by micro on 21.04.2021.
//

#include "opencv.h"
#include "opencv2/videoio.hpp"
#include "opencv2/core/hal/interface.h"
#include "opencv2/highgui.hpp"
#include "opencv2/core/utility.hpp"
#include "opencv2/video/tracking.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/calib3d/calib3d.hpp"
#include <iostream>
#include <fstream>

int disparity_map_test(int argc, char* argv[]){
    cv::namedWindow("Disparity test", 0);
    cv::namedWindow("Left", 0);
    cv::namedWindow("Right", 0);
    cv::Mat img_L, img_R,
        g1, g2,
        disparity;

    img_L = cv::imread("data/cat_good_l.jpg");
    img_R = cv::imread("data/cat_good_r.jpg");

    int sigma = 3;
    int k = (sigma*5) | 1;

    cv::resize(img_L, img_L, cv::Size(640, 360));
    cv::resize(img_R, img_R, cv::Size(640, 360));
    cv::GaussianBlur(img_L, img_L, cv::Size(k,k), sigma, sigma);
    cv::GaussianBlur(img_R, img_R, cv::Size(k,k), sigma, sigma);

    cv::cvtColor(img_L, g1, cv::COLOR_BGR2GRAY);
    cv::cvtColor(img_R, g2, cv::COLOR_BGR2GRAY);
    auto sbm = cv::StereoBM::create(16, 15);
    sbm->compute(g1, g2, disparity);
    cv::normalize(disparity, disparity, 0, 255, cv::NORM_MINMAX, CV_8U);

    for(;;) {
        cv::imshow("Disparity test", disparity);
        cv::imshow("Left", g1);
        cv::imshow("Right", g2);

        char keyboardInput = (char) cv::waitKey(10);
        if(keyboardInput == 27)
            break;
    }

    return 0;
}