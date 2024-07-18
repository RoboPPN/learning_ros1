#include<ros/ros.h>
#include<turtlesim/Pose.h>
void cb1(const turtlesim::Pose::ConstPtr& msg) 
{
    ROS_INFO("turtle_pose x: %f", msg->x);
}

void cb2(const turtlesim::Pose::ConstPtr& msg) 
{
    sleep(2);
    ROS_INFO("turtle_pose y: %f", msg->y);
}
int main(int argc, char **argv) 
{
    ros::init(argc, argv, "node");
    ros::NodeHandle nh;
    setlocale(LC_ALL, "");
    ros::Subscriber turtle_x = nh.subscribe<turtlesim::Pose>("/turtle1/pose", 1, cb1);
    ros::Subscriber turtle_y = nh.subscribe<turtlesim::Pose>("/turtle1/pose", 1, cb2);
    //----------add-------------
    ros::AsyncSpinner spinner(0);
    spinner.start();
    // spinner.stop();
    ros::waitForShutdown();
    //--------------------------
    return 1;
}