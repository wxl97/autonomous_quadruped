#include <ros/ros.h>
#include <nav_msgs/Odometry.h>
#include <geometry_msgs/PoseStamped.h>
#include <state_indicator_msgs/state_indicator.h>
#include <eigen3/Eigen/Dense>
#include <tf/transform_datatypes.h>
#include <tf/LinearMath/Matrix3x3.h>#include <ros/ros.h>
#include <nav_msgs/Odometry.h>
#include <geometry_msgs/PoseStamped.h>
#include <state_indicator_msgs/state_indicator.h>
#include <eigen3/Eigen/Dense>
#include <tf/transform_datatypes.h>
#include <tf/LinearMath/Matrix3x3.h>

// State definitions
#define STOP 0
#define STRAIGHT 1
#define LEFT_TURN 2
#define RIGHT_TURN 3
#define UPHILL 4
#define UPSTAIR 5

#define PI M_PI

// Global end point coordinates
const Eigen::Vector2d end_point(2.27, 9.80);

class StateMachineNode {
    ros::NodeHandle nh_;
    ros::Subscriber current_point_sub_;
    ros::Subscriber traj_point_sub_;
    ros::Publisher state_pub_;
    ros::Timer timer_;
    double hz_;
    state_indicator_msgs::state_indicator state_;
    int start_;

    Eigen::Vector2d current_position_, current_direction_, path_point_;

public:
    StateMachineNode() : hz_(200.0), start_(0) {
        state_.state_msg = STOP;
        current_point_sub_ = nh_.subscribe("/current_state_est", 1, &StateMachineNode::getCurrentPointCallback, this);
        traj_point_sub_ = nh_.subscribe("/trajectory_points", 1, &StateMachineNode::getTrajectoryPointCallback, this);
        timer_ = nh_.createTimer(ros::Rate(hz_), &StateMachineNode::controlLoopCallback, this);
        state_pub_ = nh_.advertise<state_indicator_msgs::state_indicator>("state", 1);
    }

    void getCurrentPointCallback(const nav_msgs::Odometry::ConstPtr& msg) {
        current_position_ << msg->pose.pose.position.x, msg->pose.pose.position.y;

        // get the current direction vector
        double yaw = tf::getYaw(msg->pose.pose.orientation);
        current_direction_ << -sin(yaw), cos(yaw);
    }

    void getTrajectoryPointCallback(const geometry_msgs::PoseStamped::ConstPtr& traj_info) {
        path_point_ << traj_info->pose.position.x, traj_info->pose.position.y;
        start_ = 1;
    }

    // Calculate the sine of the angle between the current direction and the goal direction
    double calculateSinTheta(const Eigen::Vector2d& current_direction, const Eigen::Vector2d& goal_direction) {
        return (-current_direction(0) * goal_direction(1) + current_direction(1) * goal_direction(0)) / 
               (current_direction.norm() * goal_direction.norm());
    }

    // Calculate the distance between the current position and the target position
    double calculateDeltaS(const Eigen::Vector2d& current, const Eigen::Vector2d& target) {
        return (current - target).norm();
    }

    void controlLoopCallback(const ros::TimerEvent&) {
        if (!start_) return;

        Eigen::Vector2d goal_direction = path_point_ - current_position_;
        double sin_theta = calculateSinTheta(current_direction_, goal_direction);
        double delta_s = calculateDeltaS(current_position_, end_point); 

    // Check if it has entered the UPHILL state, Uphill Range Condition
    if (current_position_.x() > 5.22 && current_position_.x() < 5.805 &&
        current_position_.y() > 1.36 && current_position_.y() < 2.3) {
        Eigen::Vector2d uphill_direction(0, 1);  // the direction vector of UPHILL
        double uphill_sin_theta = calculateSinTheta(current_direction_, uphill_direction);
        if (uphill_sin_theta >= -0.23 && uphill_sin_theta <= 0.23) {
            state_.state_msg = UPHILL;
        }
    }
    // Check if it has entered the UPSTAIR state, Upstair Range Condition
    else if (current_position_.x() > 5.22 && current_position_.x() < 5.805 &&
            current_position_.y() > 5.40 && current_position_.y() < 6.5) {
        Eigen::Vector2d upstair_direction(0, 1);  // the direction vector of UPSTAIR
        double upstair_sin_theta = calculateSinTheta(current_direction_, upstair_direction);
        if (upstair_sin_theta >= -0.3 && upstair_sin_theta <= 0.3) {
            state_.state_msg = UPSTAIR;
        }
    }
    // check if it has left the UPHILL or UPSTAIR area
    else {
        if (state_.state_msg == UPHILL) {
            // check if it has left the UPHILL area
            if (current_position_.x() <= 5.25 || current_position_.x() >= 5.75 ||
                current_position_.y() <= 1.4 || current_position_.y() >= 2.5) {
                state_.state_msg = STRAIGHT;
            }
        } else if (state_.state_msg == UPSTAIR) {
            // check if it has left the UPSTAIR area
            if (current_position_.x() <= 5.25 || current_position_.x() >= 5.75 ||
                current_position_.y() <= 5.4 || current_position_.y() >= 6.5) {
                state_.state_msg = STRAIGHT;
            }
        }
    }

    switch (state_.state_msg) {
        case STOP:
            if (delta_s > 0.1) {
                state_.state_msg = STRAIGHT;
            }
            break;

        case STRAIGHT:
            if (delta_s < 0.1) {
                state_.state_msg = STOP;
            } else if (sin_theta > 0.2) {
                state_.state_msg = RIGHT_TURN;
            } else if (sin_theta < -0.2) {
                state_.state_msg = LEFT_TURN;
            }
            break;

        case LEFT_TURN:
            if (sin_theta >= -0.2) {
                state_.state_msg = STRAIGHT;
            }
            break;

        case RIGHT_TURN:
            if (sin_theta <= 0.2) {
                state_.state_msg = STRAIGHT;
            }
            break;
    }

    state_pub_.publish(state_);


        state_pub_.publish(state_);

        // print the current state
        ROS_INFO_STREAM("Current Point: (" 
                        << current_position_.x() << ", " 
                        << current_position_.y() << ")");
        ROS_INFO_STREAM("Trajectory Point: (" 
                        << path_point_.x() << ", " 
                        << path_point_.y() << ")");
        ROS_INFO_STREAM("Current State: " << state_.state_msg);
        ROS_INFO_STREAM("Delta Sin: " << sin_theta);
        ROS_INFO_STREAM("Delta S: " << delta_s);

        // print the current direction vector
        ROS_INFO_STREAM("Current Direction Vector: (" 
                        << current_direction_.x() << ", " 
                        << current_direction_.y() << ")");
        ROS_INFO_STREAM("Goal Direction Vector: (" 
                        << goal_direction.x() << ", " 
                        << goal_direction.y() << ")");
    }
};

int main(int argc, char** argv) {
    ros::init(argc, argv, "state_machine_online_node");
    StateMachineNode n;
    ros::spin();
    return 0;
}
