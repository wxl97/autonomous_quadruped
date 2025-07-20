cmake_minimum_required(VERSION 3.0.2)
project(trajectory_planning)

find_package(catkin REQUIRED COMPONENTS
  roscpp
  nav_msgs
  tf
  geometry_msgs
  trajectory_msgs
  message_generation
)

add_message_files(
  FILES
  TrajectoryInfo.msg
)

generate_messages(
  DEPENDENCIES
  geometry_msgs
  std_msgs
)

catkin_package(
  CATKIN_DEPENDS roscpp nav_msgs tf geometry_msgs trajectory_msgs message_runtime
)

include_directories(
  ${catkin_INCLUDE_DIRS}
)

add_executable(trajectory_planner src/trajectory_node.cpp)
add_dependencies(trajectory_planner ${${PROJECT_NAME}_EXPORTED_TARGETS} ${catkin_EXPORTED_TARGETS})
target_link_libraries(trajectory_planner ${catkin_LIBRARIES})
