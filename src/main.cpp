#define CGAL_USE_BASIC_VIEWER
#include <boost/asio.hpp>
#include <CGAL/Simple_cartesian.h>
#include <CGAL/Surface_mesh.h>
#include <CGAL/draw_surface_mesh.h>
#include <fstream>
#include <iostream>
#include <filesystem>
typedef CGAL::Simple_cartesian<double>                       Kernel;
typedef Kernel::Point_3                                      Point;
typedef CGAL::Surface_mesh<Point>                            Mesh;
int main(int argc, char* argv[])
{
    boost::asio::io_context io_context;
  Mesh sm1;
  std::ifstream in1((argc>1)?argv[1]:"data/elephant.off");
  std::cout << std::filesystem::current_path() << endl;
  std::cout << in1.is_open() << std::endl;
  in1 >> sm1;
  CGAL::draw(sm1);
  return EXIT_SUCCESS;
}