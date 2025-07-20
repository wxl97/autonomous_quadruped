#include <ros/ros.h>
#include <trajectory_planning/TrajectoryInfo.h>
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
    ros::Subscriber traj_info_sub_;
    ros::Publisher state_pub_;
    ros::Timer timer_;
    double hz_;
    state_indicator_msgs::state_indicator state_;
    int start_;

    Eigen::Vector2d current_position_, current_direction_, path_point_;
    Eigen::Vector3d path_point_3d_;

public:
    StateMachineNode() : hz_(200.0), start_(0) {
        state_.state_msg = STOP;
        traj_info_sub_ = nh_.subscribe("trajectory_info", 1, &StateMachineNode::getTrajectoryInfoCallback, this);
        timer_ = nh_.createTimer(ros::Rate(hz_), &StateMachineNode::controlLoopCallback, this);
        state_pub_ = nh_.advertise<state_indicator_msgs::state_indicator>("state", 1);
    }

    void getTrajectoryInfoCallback(const trajectory_planning::TrajectoryInfo::ConstPtr& traj_info) {
        current_position_ << traj_info->current_pose.position.x, traj_info->current_pose.position.y;

        // get the current direction vector
        double yaw = tf::getYaw(traj_info->current_pose.orientation);
        current_direction_ << -sin(yaw), cos(yaw);

        path_point_ << traj_info->generated_trajectory_point.position.x, traj_info->generated_trajectory_point.position.y;
        path_point_3d_ << traj_info->waypoint_pose.position.x, traj_info->waypoint_pose.position.y, traj_info->waypoint_pose.position.z;
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

        // state machine logic
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
                } else if (path_point_3d_(2) > 0.28 && path_point_3d_(2) < 0.31){
                    state_.state_msg = UPHILL;
                } else if (path_point_3d_(2) > 0.31){
                    state_.state_msg = UPSTAIR;
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

            case UPHILL :
                if (path_point_3d_(2) < 0.28) {
                    state_.state_msg = STRAIGHT;
                }
                break;

            case UPSTAIR :
                if (path_point_3d_(2) < 0.31) {
                    state_.state_msg = STRAIGHT;
                }
                break;
        }

        state_pub_.publish(state_);

        // print the current state
        ROS_INFO_STREAM("Current Point: (" 
                        << current_position_.x() << ", " 
                        << current_position_.y() << ")");
        ROS_INFO_STREAM("Trajectory Point: (" 
                        << path_point_.x() << ", " 
                        << path_point_.y() << ")");
        ROS_INFO_STREAM("Waypoint: (" 
                        << path_point_3d_.x() << ", " 
                        << path_point_3d_.y() << ", " 
                        << path_point_3d_.z() << ")");
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
    ros::init(argc, argv, "state_machine_offline_node");
    StateMachineNode n;
    ros::spin();
    return 0;
}

