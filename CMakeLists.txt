cmake_minimum_required (VERSION 3.10)

add_definitions (-std=c++11)
## $(pkg-config --cflags --libs opencv4)
# set(CXX_FLAGS "-Wall")
# set(CMAKE_CXX_FLAGS, "${CXX_FLAGS}")

set (CMAKE_BUILD_TYPE Debug)

project (IMU-Data-Ingestion)

find_package ( OpenCV REQUIRED )                              
include_directories ( ${OpenCV_INCLUDE_DIRS} )

add_executable (program1 read_csv.cpp)
target_link_libraries (program1 ${OpenCV_LIBS})
