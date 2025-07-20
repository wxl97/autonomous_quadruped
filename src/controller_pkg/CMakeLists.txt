cmake_minimum_required(VERSION 2.8.3)
project(controller_pkg)

include(CheckCXXCompilerFlag)

CHECK_CXX_COMPILER_FLAG("-std=c++11" COMPILER_SUPPORTS_CXX11)
CHECK_CXX_COMPILER_FLAG("-std=c++0x" COMPILER_SUPPORTS_CXX0X)

if(COMPILER_SUPPORTS_CXX11)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wl,--no-as-needed")
elseif(COMPILER_SUPPORTS_CXX0X)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
endif()

find_package(catkin REQUIRED COMPONENTS 
  roscpp
  nav_msgs
  std_msgs
  tf
  geometry_msgs
  mav_msgs
  tf_conversions
  eigen_conversions
  state_indicator_msgs
  trajectory_msgs
  trajectory_planning
)

catkin_package(
  CATKIN_DEPENDS 
  roscpp
  nav_msgs
  std_msgs
  tf
  geometry_msgs
  mav_msgs
  state_indicator_msgs
  trajectory_msgs
  trajectory_planning
)

find_package(Eigen3)

include_directories(${catkin_INCLUDE_DIRS})

add_executable(state_machine_offline_node src/state_machine_offline_node.cpp)
add_executable(state_machine_online_node src/state_machine_online_node.cpp)

add_dependencies(state_machine_offline_node ${catkin_EXPORTED_TARGETS})
add_dependencies(state_machine_online_node ${catkin_EXPORTED_TARGETS})

target_link_libraries(state_machine_offline_node ${catkin_LIBRARIES})
target_link_libraries(state_machine_online_node ${catkin_LIBRARIES})


add_executable(controller_node src/controller_node.cpp)
add_dependencies(controller_node ${catkin_EXPORTED_TARGETS})
target_link_libraries(controller_node ${catkin_LIBRARIES})

add_executable(controller_node_PID src/controller_node_PID.cpp)
add_dependencies(controller_node_PID ${catkin_EXPORTED_TARGETS})
target_link_libraries(controller_node_PID ${catkin_LIBRARIES})


install(TARGETS state_machine_offline_node controller_node
  RUNTIME DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION}
)

install(TARGETS state_machine_online_node controller_node
  RUNTIME DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION}
)
