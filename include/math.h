//
// Created by Admin on 18.05.2021.
//

#ifndef DATA_PROCESSOR_MATH_H
#define DATA_PROCESSOR_MATH_H

#include "opencv.h"
#include "opencv2/videoio.hpp"
#include "opencv2/core/hal/interface.h"
#include "opencv2/highgui.hpp"
#include "opencv2/core/utility.hpp"
#include "opencv2/video/tracking.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/calib3d/calib3d.hpp"

inline float calculate_rotation_angle(const float& t, const float& T) { return t / T * 2 * 3.141592; }
cv::Mat create_model_transform_matrix(const int& r, const int& train_height, const float& theta);

#endif //DATA_PROCESSOR_MATH_H
