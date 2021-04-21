cmake_minimum_required(VERSION 3.8)

project(data_processor)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

# Project dependencies
# - boost: CGAL dependency + boost.asio for asynchronous socket i/o
# - Eigen: CGAL dependency for Poisson Surface Reconstruction
# - Qt5: CGAL dependency for visualization
# - CGAL

set(CGAL_DO_NOT_WARN_ABOUT_CMAKE_BUILD_TYPE TRUE)

find_package(OpenCV)
find_package(CGAL COMPONENTS Core ImageIO Qt5 REQUIRED)

set(CMAKE_MODULE_PATH "${CMAKE_MODULE_PATH};${CMAKE_CURRENT_SOURCE_DIR}/cmake")
include(common)

find_c_and_cpp_files("${CMAKE_CURRENT_SOURCE_DIR}/include" data_processor_includes)
find_c_and_cpp_files("${CMAKE_CURRENT_SOURCE_DIR}/src" data_processor_sources)
find_ui_files("${CMAKE_CURRENT_SOURCE_DIR}/ui" data_processor_uis)

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES
    ${data_processor_includes} ${data_processor_sources} ${data_processor_uis})

add_executable(data_processor
        ${data_processor_includes} ${data_processor_sources} ${data_processor_uis} src/cgal.cpp include/cgal.h src/opencv.cpp include/opencv.h src/vertexDetection.cpp include/vertexDetection.h include/disparityMap.h src/disparityMap.cpp)
target_include_directories(data_processor PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_SOURCE_DIR}/build/
        ${OpenCV_INCLUDE_DIRS})
target_link_libraries(data_processor PUBLIC CGAL::CGAL CGAL::CGAL_Core CGAL::CGAL_ImageIO CGAL::CGAL_Qt5 ${OpenCV_LIBS})

if(MINGW)
    target_link_libraries(data_processor PUBLIC ws2_32)
endif()

set_target_properties(data_processor
    PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)
set_target_properties(data_processor PROPERTIES CXX_STANDARD 20)
set_target_properties(data_processor PROPERTIES LINKER_LANGUAGE CXX)