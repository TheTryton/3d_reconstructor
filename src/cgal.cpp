//
// Created by micro on 25.03.2021.
//

#include "cgal.h"

#define CGAL_USE_BASIC_VIEWER
//#include <boost/asio.hpp>
//#include <CGAL/Simple_cartesian.h>
//#include <CGAL/Surface_mesh.h>
//#include <CGAL/draw_surface_mesh.h>
//#include <fstream>
//#include <iostream>
//#include <filesystem>
//#include "opencv2/videoio.hpp"
//#include <vector>
//#include <disparityMap.h>
//
//typedef CGAL::Simple_cartesian<double>                       Kernel;
//typedef Kernel::Point_3                                      Point;
//typedef CGAL::Surface_mesh<Point>                            Mesh;

//int cgal_main(int argc, char* argv[]) {
//    boost::asio::io_context io_context;
//    Mesh sm1;
//    std::ifstream in1((argc > 1) ? argv[1] : "data/elephant.off");
//    std::cout << std::filesystem::current_path() << std::endl;
//    std::cout << in1.is_open() << std::endl;
//    in1 >> sm1;
//    CGAL::draw(sm1);
//    return EXIT_SUCCESS;
//}

// TODO
//  contour - vector of points on edges of object
//  disparity - matrix of z coordinate for x = row, y = col
//  (comment it out if it does not compile)
//void cgal_process_data(const std::vector<cv::Point>& contour, const cv::Mat& disparity, const Point& camera_position){
//    std::vector<Point> model_vertices;
//    for(const auto& point : contour){
//        // general idea, but not working
//        // here we need to transform points from frame
//        // to model coordinates
//        int z = disparity.at<int>(point);   // can be counted as average or min/max from points around
//        Point vertex_on_frame(point.x,point.y,z);
//        // Mathemagical stuff
//    }
//}
