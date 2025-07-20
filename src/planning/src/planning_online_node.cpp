#include <ros/ros.h>
#include <tf/transform_listener.h>
#include <move_base_msgs/MoveBaseAction.h>
#include <actionlib/client/simple_action_client.h>
#include <geometry_msgs/PoseStamped.h>
#include <vector>

// Define a structure to hold position and orientation data
struct Waypoint {
    double x, y, z;
    double qx, qy, qz, qw;
};

int main(int argc, char** argv) {
    ros::init(argc, argv, "planning_node");
    ros::NodeHandle nh;

    // Define action client for interacting with move_base
    actionlib::SimpleActionClient<move_base_msgs::MoveBaseAction> ac("move_base", true);

    // Wait for the action server to come up
    while(!ac.waitForServer(ros::Duration(5.0))){
        ROS_INFO("Waiting for the move_base action server to come up");
    }

    // Define waypoints
    std::vector<Waypoint> waypoints = {
        { 0, 0.60, 0, 0, 0, 0, 1 },
        { 5.62, 0.4, 0, 0, 0, 0, 1},
        { 5.62, 3.6, 0, 0, 0, 0, 1},
        { 5.6, 7.42, 0,  0, 0, 0, 1},
        { 2.8, 9, 0,  0, 0, 0, 1},
        { 2.5,  10,  0, 0, 0, 0, 1}

    };

    tf::TransformListener listener;

    for (const auto& waypoint : waypoints) {
        move_base_msgs::MoveBaseGoal goal;
        goal.target_pose.header.frame_id = "world";
        goal.target_pose.header.stamp = ros::Time::now();
        goal.target_pose.pose.position.x = waypoint.x;
        goal.target_pose.pose.position.y = waypoint.y;
        goal.target_pose.pose.position.z = waypoint.z;
        goal.target_pose.pose.orientation.x = waypoint.qx;
        goal.target_pose.pose.orientation.y = waypoint.qy;
        goal.target_pose.pose.orientation.z = waypoint.qz;
        goal.target_pose.pose.orientation.w = waypoint.qw;

        ROS_INFO("Sending goal: [%.2f, %.2f, %.2f, %.2f, %.2f, %.2f, %.2f]",
                 waypoint.x, waypoint.y, waypoint.z, waypoint.qx, waypoint.qy, waypoint.qz, waypoint.qw);

        ac.sendGoal(goal);

        bool reached_goal = false;
        while (nh.ok() && !reached_goal) {
            tf::StampedTransform transform;
            try {
                listener.lookupTransform("world", "true_body", ros::Time(0), transform);
                double dx = transform.getOrigin().x() - waypoint.x;
                double dy = transform.getOrigin().y() - waypoint.y;
                double distance = sqrt(dx*dx + dy*dy);
                if (distance < 0.18) {  // 18 cm tolerance
                    reached_goal = true;
                    ROS_INFO("Custom check: Reached goal within tolerance of 0.18m.");
                } else {
                    ROS_INFO("Custom check: Not within tolerance yet (distance: %.2f).", distance);
                }
            } catch (tf::TransformException &ex) {
                ROS_WARN("Transform error: %s", ex.what());
                ros::Duration(0.05).sleep();
            }
            ros::Duration(0.05).sleep(); // Short delay to prevent tight loop
        }
        // ros::Duration(1.0).sleep(); // Optional delay between goals
    }

    return 0;
}
