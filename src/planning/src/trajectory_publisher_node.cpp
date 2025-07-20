#include <ros/ros.h>
#include <nav_msgs/Path.h>
#include <geometry_msgs/PoseStamped.h>
#include <cmath>

class TrajectoryPublisher {
public:
    TrajectoryPublisher()
    {
        // Initialize node handle
        ros::NodeHandle nh;
        
        // Subscribe to the global path ros topic
        path_sub_ = nh.subscribe("/global_plan", 10, &TrajectoryPublisher::pathCallback, this);
        
        // Publish the trajectory points
        traj_pub_ = nh.advertise<geometry_msgs::PoseStamped>("/trajectory_points", 10);
        
        // Set the lookahead distance
        lookahead_distance_ = 0.45; // 0.45 meters
    }

private:
    ros::Subscriber path_sub_;
    ros::Publisher traj_pub_;
    double lookahead_distance_;
    
    void pathCallback(const nav_msgs::Path::ConstPtr& path_msg)
    {
        // Check if the path is empty
        if (path_msg->poses.empty()) return;

        // Initialize distance traveled
        double distance_traveled = 0.0;
        geometry_msgs::Point last_point = path_msg->poses[0].pose.position;

        // Iterate through the path
        for (size_t i = 1; i < path_msg->poses.size(); ++i)
        {
            geometry_msgs::Point current_point = path_msg->poses[i].pose.position;
            double distance = distanceBetweenPointsXY(last_point, current_point);
            distance_traveled += distance;

            if (distance_traveled >= lookahead_distance_)
            {
                // When the desired distance is reached, publish this point.
                traj_pub_.publish(path_msg->poses[i]);
                return;
            }

            last_point = current_point;
        }

        // If the lookahead distance is not reached, publish the last point in the path
        traj_pub_.publish(path_msg->poses.back());
    }

    double distanceBetweenPointsXY(const geometry_msgs::Point& p1, const geometry_msgs::Point& p2)
    {
        return std::sqrt(std::pow(p1.x - p2.x, 2) + std::pow(p1.y - p2.y, 2));
    }
};

int main(int argc, char** argv)
{
    ros::init(argc, argv, "trajectory_publisher_node");
    TrajectoryPublisher trajectory_publisher;
    ros::spin();
    return 0;
}
