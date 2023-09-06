#include<ros/ros.h>
#include<actionlib/server/simple_action_server.h>
#include<learning_actionlib/FibonacciAction.h>

class FibonacciAction
{
    protected:
        ros::NodeHandle nh_;
        actionlib::SimpleActionServer<learning_actionlib::FibonacciAction> as_;
        std:: string action_name_;
        //创建用于发布feedback/result的消息
        learning_actionlib::FibonacciFeedback feedback_;
        learning_actionlib::FibonacciResult result_;
    
    public:
        FibonacciAction(std::string name):as_(nh_,name,boost::bind(&FibonacciAction::executeCB,this,_1),false),
        action_name_(name){
            as_.start();
        }

        ~FibonacciAction(void){}
        
        void executeCB(const learning_actionlib::FibonacciGoalConstPtr &goal){
            //辅助变量
            ros::Rate r(1);
            bool success = true;
            //将值压进斐波那契序列中
            feedback_.sequence.clear();
            feedback_.sequence.push_back(0);
            feedback_.sequence.push_back(1);
            
            ROS_INFO("%s: Executing, creating fibonacci sequence of order %i with seeds %i, %i", \
            action_name_.c_str(), goal->order, feedback_.sequence[0], feedback_.sequence[1]);

            //开始执行action
            for(int i=1;i<=goal->order;i++){
                //检查客户端是否没有请求抢占
                if(as_.isPreemptRequested() || !ros::ok())
                    {
                        ROS_INFO("%s: Preempted", action_name_.c_str());
                        //设置动作状态为抢占
                        as_.setPreempted();
                        success = false;
                        break;
                    }
                    feedback_.sequence.push_back(feedback_.sequence[i] + feedback_.sequence[i-1]);
                    //发布反馈
                    as_.publishFeedback(feedback_);
                    //这个睡眠是不必要的，为了演示目的，序列以1hz计算
                    r.sleep();
            }

            if(success){
                result_.sequence = feedback_.sequence;
                ROS_INFO("%s: Succeeded",action_name_.c_str());
                //设置动作状态为succeeded
                as_.setSucceeded(result_);
            }
        }
};

int main(int argc,char** argv){
    ros::init(argc,argv,"fibonacci_server");

    FibonacciAction fibonacci("fibonacci");
    ros::spin();
    return 0;

}