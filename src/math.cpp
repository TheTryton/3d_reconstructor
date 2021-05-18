//
// Created by Admin on 18.05.2021.
//

#include "math.h"
#include <cmath>


cv::Mat create_model_transform_matrix(const cv::Mat& T, const float& theta) {
    cv::Mat R = (cv::Mat_<double>(4,4) <<   std::cos(theta), 0, std::sin(theta), 0,
                                            0, 1, 0, 0,
                                            -std::sin(theta), 0, std::cos(theta), 0,
                                            0, 0, 0, 1);
    return T * R;
}

