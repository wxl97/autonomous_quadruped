#include <ros/ros.h>
#include <nav_msgs/Path.h>
#include <geometry_msgs/PoseStamped.h>
#include <vector>

// Function to publish the global path
void publishPath(ros::Publisher& path_pub, const std::vector<std::vector<double>>& waypoints) {
    nav_msgs::Path path;
    path.header.frame_id = "world"; // Ensure this matches your global frame_id
    path.header.stamp = ros::Time::now();

    // Add each waypoint to the path
    for (const auto& waypoint : waypoints) {
        geometry_msgs::PoseStamped pose;
        pose.header.frame_id = "world"; // Ensure this matches your global frame_id
        pose.header.stamp = ros::Time::now();
        pose.pose.position.x = waypoint[0];
        pose.pose.position.y = waypoint[1];
        pose.pose.position.z = waypoint[2];
        pose.pose.orientation.x = waypoint[3];
        pose.pose.orientation.y = waypoint[4];
        pose.pose.orientation.z = waypoint[5];
        pose.pose.orientation.w = waypoint[6];

        path.poses.push_back(pose);
    }

    path_pub.publish(path);
    ROS_INFO("Published global path with %lu points", path.poses.size());
}

int main(int argc, char** argv) {
    ros::init(argc, argv, "planner");
    ros::NodeHandle nh;

    ros::Publisher path_pub = nh.advertise<nav_msgs::Path>("/global_plan", 1);
    ros::Rate rate(1.0);  // Publish at 1 Hz

    // Define waypoints to form a continuous path
    std::vector<std::vector<double>> waypoints = {
        { 0, 0, 0, 0, 0, 0, 0 },
        { 0, 1.76, 0, 0, 0, 0, 1 },
        { 3, 1.65, 0, 0, 0, 0, 1 },
        { 4.4, 0.38, 0, 0, 0, 0, 1 },
        { 5.5, 0.42, 0, 0, 0, 0, 1 },
        { 5.5, 1.5, 0, 0, 0, 0, 1 },
        { 5.5, 2.65, 0.3, 0, 0, 0, 1 },
        { 5.5, 5.0, 0, 0, 0, 0, 1 },
        { 5.47, 7.29, 0,  0, 0, 0, 1},
        { 4.50, 8.14, 0,  0, 0, 0, 1},
        { 3.68, 8.32, 0,  0, 0, 0, 1},
        { 2.91, 8.40, 0,  0, 0, 0, 1},
        { 2.5,  10,  0, 0, 0, 0, 1}
    };

    while (ros::ok()) {
        publishPath(path_pub, waypoints);
        ros::spinOnce();
        rate.sleep();
    }

    return 0;
}

