#!/usr/bin/env python
import rospy
from geometry_msgs.msg import Twist
from std_srvs.srv import Trigger, TriggerResponse

pubCommand = False  # 是否在运动的标志，作为开关
turtle_vel_pub = None

# service回调函数
def command_callback(req):
    global pubCommand
    pubCommand = not pubCommand
    # 显示请求数据
    rospy.loginfo("Publish turtle velocity command [%s]", "Yes" if pubCommand else "No")
    # 设置反馈数据
    res = TriggerResponse()
    res.success = True
    res.message = "Change turtle command state!"
    return res

def main():
    global turtle_vel_pub
    # ROS节点初始化
    rospy.init_node('turtle_command_server')
    # 创建一个名为/turtle_command的server，注册回调函数command_callback
    command_service = rospy.Service('/turtle_command', Trigger, command_callback)
    # 创建一个Publisher，发布名为/turtle1/cmd_vel的topic，消息类型为geometry_msgs/Twist，队列长度10
    turtle_vel_pub = rospy.Publisher('/turtle1/cmd_vel', Twist, queue_size=10)
    # 循环等待回调函数
    rospy.loginfo("Ready to receive turtle command.")
    # 设置循环的频率
    loop_rate = rospy.Rate(10)
    while not rospy.is_shutdown():
        # 如果标志为true，则发布速度指令
        if pubCommand:
            vel_msg = Twist()
            vel_msg.linear.x = 0.5
            vel_msg.angular.z = 0.2
            turtle_vel_pub.publish(vel_msg)
        # 按照循环频率延时
        loop_rate.sleep()

if __name__ == '__main__':
    main()
