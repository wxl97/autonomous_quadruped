cmake_minimum_required(VERSION 3.0.2)
project(perception)

find_package(catkin REQUIRED COMPONENTS
  roscpp
  sensor_msgs
  tf2_ros
  depth_image_proc
  nodelet
  nodelet_topic_tools
  pcl_ros
)

find_package(Eigen3 REQUIRED)
include_directories(
  include
  ${catkin_INCLUDE_DIRS}
  ${EIGEN3_INCLUDE_DIR}
)

catkin_package(
  CATKIN_DEPENDS roscpp sensor_msgs tf2_ros pcl_ros 
)

include_directories(
  ${catkin_INCLUDE_DIRS}
  ${PCL_INCLUDE_DIRS}
)


add_definitions(${PCL_DEFINITIONS})

