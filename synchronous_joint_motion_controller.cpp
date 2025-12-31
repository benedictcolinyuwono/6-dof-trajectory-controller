{\rtf1\ansi\ansicpg1252\cocoartf2867
\cocoatextscaling0\cocoaplatform0{\fonttbl\f0\fnil\fcharset0 Menlo-Regular;}
{\colortbl;\red255\green255\blue255;\red0\green0\blue0;}
{\*\expandedcolortbl;;\cssrgb\c0\c0\c0;}
\paperw11900\paperh16840\margl1440\margr1440\vieww11520\viewh8400\viewkind0
\deftab720
\pard\pardeftab720\partightenfactor0

\f0\fs24 \cf0 \expnd0\expndtw0\kerning0
#include <chrono>\
#include <memory>\
#include <thread>\
#include <functional>\
#include <cmath>\
#include <control_msgs/msg/joint_jog.hpp>\
#include <sensor_msgs/msg/joint_state.hpp>\
#include "rclcpp/rclcpp.hpp"\
#include "std_msgs/msg/string.hpp"\
\
using std::placeholders::_1;\
using namespace std::chrono_literals;\
\
class GoalMovementMover6 : public rclcpp::Node\
\{\
public:\
\'a0\'a0\'a0\'a0GoalMovementMover6()\
\'a0\'a0\'a0\'a0: Node("GoalMovementMover6"), count_(0)\
\'a0\'a0\'a0\'a0\{\
\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0cb_group_ =\
\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0this->create_callback_group(\
\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0rclcpp::CallbackGroupType::MutuallyExclusive);\
\
\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0tm_group_ =\
\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0this->create_callback_group(\
\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0rclcpp::CallbackGroupType::MutuallyExclusive);\
\
\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0rclcpp::SubscriptionOptions options;\
\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0options.callback_group = cb_group_;\
\
\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0publisherJointPosition_ =\
\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0this->create_publisher<control_msgs::msg::JointJog>(\
\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0"/JointJog", 10);\
\
\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0subscriptionJointPosition_ =\
\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0this->create_subscription<sensor_msgs::msg::JointState>(\
\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0"/joint_states", 10,\
\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0std::bind(\
\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0&GoalMovementMover6::topic_jointStatesCallback,\
\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0this, _1),\
\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0options);\
\
\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0demandSubscription_ =\
\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0this->create_subscription<sensor_msgs::msg::JointState>(\
\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0"/joint_demands", 10,\
\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0std::bind(\
\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0&GoalMovementMover6::jointDemandsCallback,\
\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0this, _1),\
\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0options);\
\
\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0timer_ =\
\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0this->create_wall_timer(\
\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a050ms,\
\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0std::bind(\
\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0&GoalMovementMover6::timer_callback,\
\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0this),\
\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0tm_group_);\
\
\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0known_states = false;\
\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0new_demands_received = false;\
\
\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0joint1_angle = joint2_angle = joint3_angle =\
\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0joint4_angle = joint5_angle = joint6_angle = 0.0f;\
\
\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0jointdemand_1 = jointdemand_2 = jointdemand_3 =\
\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0jointdemand_4 = jointdemand_5 = jointdemand_6 = 0.0f;\
\'a0\'a0\'a0\'a0\}\
\
private:\
\'a0\'a0\'a0\'a0void timer_callback()\
\'a0\'a0\'a0\'a0\{\
\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0move_my_robot();\
\'a0\'a0\'a0\'a0\}\
\
\'a0\'a0\'a0\'a0void topic_jointStatesCallback(\
\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0const sensor_msgs::msg::JointState::SharedPtr msg)\
\'a0\'a0\'a0\'a0\{\
\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0if (msg->position.size() >= 6)\
\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\{\
\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0joint1_angle = msg->position[0];\
\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0joint2_angle = msg->position[1];\
\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0joint3_angle = msg->position[2];\
\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0joint4_angle = msg->position[3];\
\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0joint5_angle = msg->position[4];\
\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0joint6_angle = msg->position[5];\
\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0known_states = true;\
\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\}\
\'a0\'a0\'a0\'a0\}\
\
\'a0\'a0\'a0\'a0void jointDemandsCallback(\
\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0const sensor_msgs::msg::JointState::SharedPtr msg)\
\'a0\'a0\'a0\'a0\{\
\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0if (msg->position.size() >= 6)\
\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\{\
\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0jointdemand_1 = msg->position[0];\
\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0jointdemand_2 = msg->position[1];\
\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0jointdemand_3 = msg->position[2];\
\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0jointdemand_4 = msg->position[3];\
\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0jointdemand_5 = msg->position[4];\
\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0jointdemand_6 = msg->position[5];\
\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0new_demands_received = true;\
\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\}\
\'a0\'a0\'a0\'a0\}\
\
\'a0\'a0\'a0\'a0void send_msg(float v1, float v2, float v3,\
\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0float v4, float v5, float v6)\
\'a0\'a0\'a0\'a0\{\
\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0control_msgs::msg::JointJog msg;\
\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0msg.joint_names =\
\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\{"joint1", "joint2", "joint3",\
\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0"joint4", "joint5", "joint6"\};\
\
\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0msg.velocities =\
\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\{v1, v2, v3, v4, v5, v6\};\
\
\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0publisherJointPosition_->publish(msg);\
\'a0\'a0\'a0\'a0\}\
\
\'a0\'a0\'a0\'a0void move_my_robot()\
\'a0\'a0\'a0\'a0\{\
\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0if (!known_states) return;\
\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0if (!new_demands_received) return;\
\
\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0const float gain\'a0 \'a0 = 0.25f;\
\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0const float tol_err = 0.03f;\
\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0const float v_min \'a0 = 0.05f;\
\
\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0float e1 = jointdemand_1 - joint1_angle;\
\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0float e2 = jointdemand_2 - joint2_angle;\
\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0float e3 = jointdemand_3 - joint3_angle;\
\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0float e4 = jointdemand_4 - joint4_angle;\
\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0float e5 = jointdemand_5 - joint5_angle;\
\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0float e6 = jointdemand_6 - joint6_angle;\
\
\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0float max_err = std::max(\{\
\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0std::fabs(e1), std::fabs(e2), std::fabs(e3),\
\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0std::fabs(e4), std::fabs(e5), std::fabs(e6)\
\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\});\
\
\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0float v1 = 0.0f, v2 = 0.0f, v3 = 0.0f;\
\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0float v4 = 0.0f, v5 = 0.0f, v6 = 0.0f;\
\
\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0if (max_err <= tol_err)\
\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\{\
\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0send_msg(v1, v2, v3, v4, v5, v6);\
\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0return;\
\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\}\
\
\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0auto compute_vel = [&](float e) -> float\
\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\{\
\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0if (std::fabs(e) <= tol_err)\
\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0return 0.0f;\
\
\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0float ratio = e / max_err;\
\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0float v = gain * ratio;\
\
\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0return v;\
\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\};\
\
\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0v1 = compute_vel(e1);\
\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0v2 = compute_vel(e2);\
\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0v3 = compute_vel(e3);\
\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0v4 = compute_vel(e4);\
\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0v5 = compute_vel(e5);\
\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0v6 = compute_vel(e6);\
\
\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0send_msg(v1, v2, v3, v4, v5, v6);\
\'a0\'a0\'a0\'a0\}\
\
\'a0\'a0\'a0\'a0bool known_states;\
\'a0\'a0\'a0\'a0bool new_demands_received;\
\
\'a0\'a0\'a0\'a0float joint1_angle, joint2_angle, joint3_angle;\
\'a0\'a0\'a0\'a0float joint4_angle, joint5_angle, joint6_angle;\
\
\'a0\'a0\'a0\'a0float jointdemand_1, jointdemand_2, jointdemand_3;\
\'a0\'a0\'a0\'a0float jointdemand_4, jointdemand_5, jointdemand_6;\
\
\'a0\'a0\'a0\'a0rclcpp::CallbackGroup::SharedPtr cb_group_;\
\'a0\'a0\'a0\'a0rclcpp::CallbackGroup::SharedPtr tm_group_;\
\'a0\'a0\'a0\'a0rclcpp::TimerBase::SharedPtr timer_;\
\
\'a0\'a0\'a0\'a0rclcpp::Publisher<control_msgs::msg::JointJog>::SharedPtr\
\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0publisherJointPosition_;\
\
\'a0\'a0\'a0\'a0rclcpp::Subscription<sensor_msgs::msg::JointState>::SharedPtr\
\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0subscriptionJointPosition_;\
\
\'a0\'a0\'a0\'a0rclcpp::Subscription<sensor_msgs::msg::JointState>::SharedPtr\
\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0demandSubscription_;\
\
\'a0\'a0\'a0\'a0unsigned long long state = 0;\
\'a0\'a0\'a0\'a0size_t count_;\
\};\
\
int main(int argc, char * argv[])\
\{\
\'a0\'a0\'a0\'a0rclcpp::init(argc, argv);\
\'a0\'a0\'a0\'a0auto node = std::make_shared<GoalMovementMover6>();\
\'a0\'a0\'a0\'a0rclcpp::executors::MultiThreadedExecutor executor;\
\'a0\'a0\'a0\'a0executor.add_node(node);\
\'a0\'a0\'a0\'a0executor.spin();\
\'a0\'a0\'a0\'a0rclcpp::shutdown();\
\'a0\'a0\'a0\'a0return 0;\
\}\
}