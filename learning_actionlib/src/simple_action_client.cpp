#include<ros/ros.h>
#include<actionlib/client/simple_action_client.h>
#include<actionlib/client/terminal_state.h>
#include<learning_actionlib/FibonacciAction.h>

int main(int argc,char** argv){
    ros::init(argc,argv,"fibonacci_client");
    setlocale(LC_ALL,"");
    //创建action client
    //True使客户端spin自己的线程
    actionlib::SimpleActionClient<learning_actionlib::FibonacciAction> ac("fibonacci",true);

    ROS_INFO("等待action server 启动");

    ac.waitForServer();

    ROS_INFO("acion server启动，发送goal");
    //发送目标至action
    learning_actionlib::FibonacciGoal goal;
    goal.order = 20;
    ac.sendGoal(goal);

    //等待action 返回信息
    bool finished_before_timeout = ac.waitForResult(ros::Duration(30.0));

    if(finished_before_timeout){
        actionlib::SimpleClientGoalState state = ac.getState();
        ROS_INFO("Action finished:%s",state.toString().c_str());
    }
    else{
        ROS_INFO("动作在超时前没有完成");
        ac.cancelGoal();
    }
    return 0;

}