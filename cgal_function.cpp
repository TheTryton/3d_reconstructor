//
// Created by micro on 05.05.2021.
//

#include <vector>
#include <disparityMap.h>
#include <CGAL/Kernel/Type_equality_wrapper.h>
#include <CGAL/Simple_cartesian.h>
#include <CGAL/Surface_mesh.h>
#include <CGAL/draw_surface_mesh.h>
#include <CGAL/Cartesian/Cartesian_base.h>

typedef CGAL::Simple_cartesian<double>                       Kernel;
typedef Kernel::Point_3                                      Point;
typedef CGAL::Surface_mesh<Point>                            Mesh;

