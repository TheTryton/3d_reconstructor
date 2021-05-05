//
// Created by micro on 07.04.2021.
//

#ifndef DATA_PROCESSOR_VERTEXDETECTION_H
#define DATA_PROCESSOR_VERTEXDETECTION_H

#include "opencv.h"
#include "opencv2/videoio.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/core/utility.hpp"
#include "opencv2/video/tracking.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"
#include <iostream>

cv::Mat vertex_detection(const cv::Mat& frame);

#endif //DATA_PROCESSOR_VERTEXDETECTION_H
