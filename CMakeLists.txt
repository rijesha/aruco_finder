cmake_minimum_required (VERSION 2.6)
project (aruco_finder)

set (CMAKE_CXX_STANDARD 11)

set( OpenCV_DIR "~/opencv/release/" )
set( OpenCV_FOUND 1 )
find_package(OpenCV REQUIRED )

set( aruco_DIR "~/aruco-3.0.9/build/" )
find_package(aruco REQUIRED )


FIND_PACKAGE(Boost 1.45.0 COMPONENTS filesystem REQUIRED)
IF (Boost_FOUND)
    INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIR})
ENDIF()

message(STATUS "OpenCV library status:")
message(STATUS "    version: ${OpenCV_VERSION}")
message(STATUS "    libraries: ${OpenCV_LIBS}")
message(STATUS "    include path: ${OpenCV_INCLUDE_DIRS}")

include_directories(${OpenCV_INCLUDE_DIRS})
include_directories(${aruco_INCLUDE_DIRS})

add_executable(aruco_finder aruco_finder.cpp)
target_link_libraries(aruco_finder ${OpenCV_LIBS} ${aruco_LIBS}  ${Boost_LIBRARIES})