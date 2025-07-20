#include <ros/ros.h>
#include <geometry_msgs/PoseStamped.h>
#include <nav_msgs/Odometry.h>
#include <tf/transform_listener.h>
#include <nav_msgs/Path.h>
#include <trajectory_msgs/MultiDOFJointTrajectoryPoint.h>
#include <trajectory_planning/TrajectoryInfo.h>
#include <vector>

class TrajectoryPlanner {
public:
    TrajectoryPlanner() : nh_("~"), waypoint_index_(0), distance_threshold_(0.25), step_size_(0.1) {
        path_sub_ = nh_.subscribe("/global_plan", 1, &TrajectoryPlanner::pathCallback, this);
        current_pose_sub_ = nh_.subscribe("/current_state_est", 1, &TrajectoryPlanner::currentPoseCallback, this);
        traj_pub_ = nh_.advertise<trajectory_msgs::MultiDOFJointTrajectoryPoint>("/trajectory_point", 1);
        traj_info_pub_ = nh_.advertise<trajectory_planning::TrajectoryInfo>("/trajectory_info", 1);
        timer_ = nh_.createTimer(ros::Duration(0.1), &TrajectoryPlanner::timerCallback, this);

        // set waypoints
        waypoints_ = {

        { 0, 0, 0, 0, 0, 0, 0 },
        { 0, 1.75, 0, 0, 0, 0, 0 },
        { 1, 1.65, 0, 0, 0, 0, 0 },
        { 3, 1.65, 0, 0, 0, 0, 0 },
        { 4.4, 0.3, 0, 0, 0, 0, 0 },
        { 5.5, 0.3, 0, 0, 0, 0, 0 },
        { 5.5, 1.5, 0, 0, 0, 0, 0 },
        { 5.5, 2.65, 0.3, 0, 0, 0, 0 },
        { 5.5, 5.0, 0, 0, 0, 0, 0 },
        { 5.5, 6.5, 0.32, 0, 0, 0, 0},
        { 5.5, 7.2, 0,  0, 0, 0, 0},
        { 4.70, 8.14,0,  0, 0, 0, 0},
        { 4.75,8.06, 0, 0, 0, 0, 0},
        { 4.29, 8.5, 0,0, 0, 0, 0},
        { 3.55,  8.67,  0,0, 0, 0, 0},
        { 2.68, 8.63, 0, 0, 0, 0, 0},
        { 2.93, 8.64, 0,  0, 0, 0, 0},
        { 2.5,  10,  0, 0, 0, 0, 0},
 

        };
    }

    void pathCallback(const nav_msgs::Path::ConstPtr& path_msg) {
    }

    // Subscribe to current location information
    void currentPoseCallback(const nav_msgs::Odometry::ConstPtr& msg) {
        current_pose_ = msg->pose.pose;
    }

    // Timer callback function
    void timerCallback(const ros::TimerEvent&) {
        if (waypoints_.empty() || waypoint_index_ >= waypoints_.size()) {
            return;
        }

        geometry_msgs::Pose target_pose = convertToPose(waypoints_[waypoint_index_]);

        double distance = calculateDistance(current_pose_, target_pose);

        // If the distance between the current pose and the target pose is less than the threshold, move to the next waypoint
        if (distance < distance_threshold_ && waypoint_index_ < waypoints_.size() - 1) {
            waypoint_index_++;
            target_pose = convertToPose(waypoints_[waypoint_index_]);
        }

        geometry_msgs::Pose new_target_pose = moveTowards(current_pose_, target_pose, step_size_);
        publishTrajectoryPoint(new_target_pose);

        // Publish trajectory information
        trajectory_planning::TrajectoryInfo traj_info;
        traj_info.current_pose = current_pose_;
        traj_info.generated_trajectory_point = new_target_pose;
        traj_info.waypoint_pose = target_pose;
        traj_info_pub_.publish(traj_info);
    }

private:
    ros::NodeHandle nh_;
    ros::Subscriber path_sub_;
    ros::Subscriber current_pose_sub_;
    ros::Publisher traj_pub_;
    ros::Publisher traj_info_pub_;
    ros::Timer timer_;
    tf::TransformListener listener_;
    std::vector<std::vector<double>> waypoints_;
    size_t waypoint_index_;
    double distance_threshold_;
    double step_size_;
    geometry_msgs::Pose current_pose_;

    // Convert a vector of doubles to a Pose message
    geometry_msgs::Pose convertToPose(const std::vector<double>& waypoint) {
        geometry_msgs::Pose pose;
        pose.position.x = waypoint[0];
        pose.position.y = waypoint[1];
        pose.position.z = waypoint[2];
        pose.orientation.x = waypoint[3];
        pose.orientation.y = waypoint[4];
        pose.orientation.z = waypoint[5];
        pose.orientation.w = waypoint[6];
        return pose;
    }

    // Calculate the distance between two poses
    double calculateDistance(const geometry_msgs::Pose& pose1, const geometry_msgs::Pose& pose2) {
        double dx = pose1.position.x - pose2.position.x;
        double dy = pose1.position.y - pose2.position.y;
        return std::sqrt(dx * dx + dy * dy); // Euclidean distance in 2D
    }

    geometry_msgs::Pose moveTowards(const geometry_msgs::Pose& current_pose, const geometry_msgs::Pose& target_pose, double step_size) {
        geometry_msgs::Pose new_pose;
        double dx = target_pose.position.x - current_pose.position.x;
        double dy = target_pose.position.y - current_pose.position.y;

        double distance = calculateDistance(current_pose, target_pose);
        double ratio = distance > 0 ? std::min(step_size / distance, 1.0) : 0.0;

        new_pose.position.x = current_pose.position.x + ratio * dx;
        new_pose.position.y = current_pose.position.y + ratio * dy;
        new_pose.position.z = 0.0; // fixed z coordinate to 0
        new_pose.orientation = current_pose.orientation;

        return new_pose;
    }

    void publishTrajectoryPoint(const geometry_msgs::Pose& target_pose) {
        trajectory_msgs::MultiDOFJointTrajectoryPoint point;
        geometry_msgs::Transform transform;
        transform.translation.x = target_pose.position.x;
        transform.translation.y = target_pose.position.y;
        transform.translation.z = 0.0; 
        transform.rotation = target_pose.orientation;
        point.transforms.push_back(transform);
        traj_pub_.publish(point);
    }
};

int main(int argc, char** argv) {
    ros::init(argc, argv, "trajectory_planner");
    TrajectoryPlanner tp;
    ros::spin();
    return 0;
}
