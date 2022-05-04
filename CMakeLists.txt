cmake_minimum_required (VERSION 3.10)

# add_definitions ( -std=c++17 )
set ( CMAKE_CXX_STANDARD 17 )

project ( IMU-Data-Ingestion )

set ( CMAKE_BUILD_TYPE Debug )
set ( source_dir "${CMAKE_CURRENT_SOURCE_DIR}/src/" )
set ( csv_reader "${CMAKE_CURRENT_SOURCE_DIR}/include/imu-data-ingestion/" )

find_package ( OpenCV REQUIRED )                              
include_directories ( ${OpenCV_INCLUDE_DIRS} )
include_directories ( ${csv_reader} )

add_executable ( program1 ${source_dir}/main.cpp ${source_dir}/csv_reader.cpp )
target_link_libraries ( program1 ${OpenCV_LIBS} )