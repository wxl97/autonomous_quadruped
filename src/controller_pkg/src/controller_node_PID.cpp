#include <ros/ros.h>
#include <ros/console.h>
#include <tf/transform_datatypes.h>
#include <tf_conversions/tf_eigen.h>
#include <eigen_conversions/eigen_msg.h>
#include <mav_msgs/Actuators.h>
#include <nav_msgs/Odometry.h>
#include <trajectory_msgs/MultiDOFJointTrajectoryPoint.h>
#include <std_msgs/Float64.h>
#include <state_indicator_msgs/state_indicator.h>
#include <eigen3/Eigen/Dense>

#define PI M_PI

class PID {
public:
    PID(double kp, double ki, double kd) 
        : kp_(kp), ki_(ki), kd_(kd), prev_error_(0), integral_(0) {}

    double compute(double setpoint, double pv) {
        double error = setpoint - pv;
        integral_ += error;
        double derivative = error - prev_error_;
        prev_error_ = error;
        return kp_ * error + ki_ * integral_ + kd_ * derivative;
    }

private:
    double kp_, ki_, kd_;
    double prev_error_;
    double integral_;
};

class ControllerNode {
    ros::NodeHandle nh_;

    ros::Publisher commands_pub_;
    ros::Subscriber state_sub_;
    ros::Subscriber odom_sub_;
    ros::Subscriber path_point_sub_;
    ros::Timer timer_;

    double hz_;
    int current_state_;

    Eigen::Vector3d current_position_;
    Eigen::Vector3d current_velocity_;
    Eigen::Matrix3d current_orientation_;
    Eigen::Vector3d current_omega_;
    Eigen::Vector3d goal_position_;
    double current_yaw_;

    PID pid_turn_;
    PID pid_speed_;

public:
    ControllerNode() : hz_(200.0), current_state_(0), pid_turn_(1.0, 0.0, 0.1), pid_speed_(0.3, 0.1, 0.0) {
        commands_pub_ = nh_.advertise<mav_msgs::Actuators>("commands", 1);
        state_sub_ = nh_.subscribe("state", 1, &ControllerNode::stateCallback, this);
        odom_sub_ = nh_.subscribe("odom", 1, &ControllerNode::poseCallback, this);
        path_point_sub_ = nh_.subscribe("path_point", 1, &ControllerNode::pathCallback, this);
        timer_ = nh_.createTimer(ros::Rate(hz_), &ControllerNode::controlLoop, this);
    }

    void poseCallback(const nav_msgs::Odometry::ConstPtr& msg) {
        tf::pointMsgToEigen(msg->pose.pose.position, current_position_);
        tf::vectorMsgToEigen(msg->twist.twist.linear, current_velocity_);

        Eigen::Quaterniond quat;
        tf::quaternionMsgToEigen(msg->pose.pose.orientation, quat);
        current_orientation_ = quat.toRotationMatrix();

        tf::vectorMsgToEigen(msg->twist.twist.angular, current_omega_);
        current_yaw_ = tf::getYaw(msg->pose.pose.orientation);
    }

    void pathCallback(const geometry_msgs::Point::ConstPtr& msg) {
        goal_position_ << msg->x, msg->y, msg->z;
    }

    void stateCallback(const state_indicator_msgs::state_indicator::ConstPtr& msg) {
        current_state_ = msg->state_msg;
    }

    void controlLoop(const ros::TimerEvent&) {
        mav_msgs::Actuators msg;
        msg.angular_velocities.resize(5);

        Eigen::Vector2d current_dir(-sin(current_yaw_), cos(current_yaw_));
        Eigen::Vector2d goal_dir(goal_position_.head<2>() - current_position_.head<2>());
        double sin_theta = (-current_dir.x() * goal_dir.y() + current_dir.y() * goal_dir.x()) / 
                           (current_dir.norm() * goal_dir.norm());
        double delta_s = (goal_position_.head<2>() - current_position_.head<2>()).norm();

        double turn_speed = std::max(0.0, pid_turn_.compute(0.0, sin_theta));
        double straight_speed = std::max(0.0, 10.0 + pid_speed_.compute(0.0, delta_s)); 

        switch (current_state_) {
            case 0: // STOP
                msg.angular_velocities[0] = 0;
                msg.angular_velocities[1] = 0;
                msg.angular_velocities[2] = 0;
                msg.angular_velocities[3] = 0;
                msg.angular_velocities[4] = 0;
                break;
            case 1: // STRAIGHT
                msg.angular_velocities[0] = 0;
                msg.angular_velocities[1] = 90;
                msg.angular_velocities[2] = 0;
                msg.angular_velocities[3] = 0;
                msg.angular_velocities[4] = std::max(12.0, straight_speed);
                break;
            case 2: // LEFT_TURN
                msg.angular_velocities[0] = 0;
                msg.angular_velocities[1] = -45; // Phase between legs for left turn
                msg.angular_velocities[2] = 0;
                msg.angular_velocities[3] = 0;
                msg.angular_velocities[4] = std::max(9.0, straight_speed - std::abs(turn_speed));
                break;
            case 3: // RIGHT_TURN
                msg.angular_velocities[0] = 0;
                msg.angular_velocities[1] = 45; // Phase between legs for right turn
                msg.angular_velocities[2] = 0;
                msg.angular_velocities[3] = 0;
                msg.angular_velocities[4] = std::max(9.0, straight_speed - std::abs(turn_speed));
                break;
            case 4: // UPHILL
                msg.angular_velocities[0] = 0;
                msg.angular_velocities[1] = 0;
                msg.angular_velocities[2] = 4.5;
                msg.angular_velocities[3] = 32;
                msg.angular_velocities[4] = 9.5;
                break;
            case 5: // UPSTAIR
                msg.angular_velocities[0] = 0;
                msg.angular_velocities[1] = 0;
                msg.angular_velocities[2] = 4.0;
                msg.angular_velocities[3] = 35;
                msg.angular_velocities[4] = 8.0;
                break;
            default:
                msg.angular_velocities[0] = 0;
                msg.angular_velocities[1] = 0;
                msg.angular_velocities[2] = 0;
                msg.angular_velocities[3] = 0;
                msg.angular_velocities[4] = 7.0;
                break;
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
