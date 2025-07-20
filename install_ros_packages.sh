#!/bin/bash

# Update package list
sudo apt-get update

# Install ROS packages
sudo apt-get install -y ros-noetic-pcl-ros
sudo apt-get install -y ros-noetic-tf2-sensor-msgs
sudo apt-get install -y ros-noetic-octomap-rviz-plugins
sudo apt-get install -y ros-noetic-octomap-server
sudo apt-get install -y ros-noetic-map-server
sudo apt-get install ros-noetic-image-pipeline
sudo apt-get install ros-noetic-navigation