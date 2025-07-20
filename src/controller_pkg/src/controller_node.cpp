#include <ros/ros.h>
#include <state_indicator_msgs/state_indicator.h>
#include <mav_msgs/Actuators.h>
#include <vector>
#include <unordered_map>

#define PI M_PI

class ControllerNode {
  ros::NodeHandle nh_;

  ros::Publisher commands_pub_;
  ros::Subscriber state_sub_;
  ros::Timer timer_;

  double hz_;
  int current_state_;
  std::unordered_map<int, std::string> state_map_;

public:
  ControllerNode() : hz_(200.0), current_state_(0) {
    commands_pub_ = nh_.advertise<mav_msgs::Actuators>("commands", 1);
    state_sub_ = nh_.subscribe("state", 1, &ControllerNode::stateCallback, this);
    timer_ = nh_.createTimer(ros::Rate(hz_), &ControllerNode::controlLoop, this);

    state_map_ = {
      {0, "STOP"},
      {1, "STRAIGHT"},
      {2, "LEFT_TURN"},
      {3, "RIGHT_TURN"},
      {4, "UPHILL"},
      {5, "UPSTAIR"}
    };
  }

  void stateCallback(const state_indicator_msgs::state_indicator::ConstPtr& msg) {
    current_state_ = msg->state_msg;
    // ROS_INFO_STREAM("State changed to: " << state_map_[current_state_]);
  }

  void controlLoop(const ros::TimerEvent& t) {
    publishControl(getControlInputs(current_state_));
  }

  std::vector<double> getControlInputs(int state) {
    switch (state) {
      case 0: // STOP
        return {0, 0, 0, 0, 0};
      case 1: // STRAIGHT
        return {0, 90, 0, 0, 12};
      case 2: // LEFT_TURN
        return {0, -45, 0, 0, 10};
      case 3: // RIGHT_TURN
        return {0, 45, 0, 0, 10};
      case 4: // UPHILL
        return {0, 0, 4.5, 32, 9.5};
      case 5: // UPSTAIR
        return {0, 0, 4, 35, 8};
      default:
        return {0, 90, 0, 0, 7};
    }
  }

  void publishControl(const std::vector<double>& control_inputs) {
    mav_msgs::Actuators msg;
    msg.angular_velocities.resize(control_inputs.size());
    for (size_t i = 0; i < control_inputs.size(); ++i) {
      msg.angular_velocities[i] = control_inputs[i];
    }
    commands_pub_.publish(msg);
  }
};

int main(int argc, char** argv) {
  ros::init(argc, argv, "controller_node");
  ROS_INFO("Controller started!");
  ControllerNode controller;
  ros::spin();
  return 0;
}
