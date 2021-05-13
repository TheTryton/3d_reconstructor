//
// Created by micro on 21.04.2021.
//

#ifndef DATA_PROCESSOR_DISPARITYMAP_H
#define DATA_PROCESSOR_DISPARITYMAP_H

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
#include <map>
#include <CGAL/Surface_mesh.h>
#include <CGAL/Simple_cartesian.h>

typedef CGAL::Simple_cartesian<double>                       Kernel;
typedef Kernel::Point_3                                      Point;
typedef CGAL::Surface_mesh<Point>                            Mesh;


cv::Mat disparity_map(const cv::Mat& frame_left,
                      const cv::Mat& frame_right,
                      std::map<std::string, int>& params,
                      const cv::Rect& roi = cv::Rect(0,0,0,0));

std::vector<Point> translate_to_3d(const cv::Mat& disparity,
                                   const cv::Mat& frame,
                                   const cv::Mat& camera_matrix);
#endif //DATA_PROCESSOR_DISPARITYMAP_H
