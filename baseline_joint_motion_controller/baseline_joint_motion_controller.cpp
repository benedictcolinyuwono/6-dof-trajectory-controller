{\rtf1\ansi\ansicpg1252\cocoartf2867
\cocoatextscaling0\cocoaplatform0{\fonttbl\f0\fnil\fcharset0 Menlo-Regular;}
{\colortbl;\red255\green255\blue255;\red184\green111\blue179;\red210\green220\blue219;\red111\green194\blue194;
\red23\green147\blue150;\red236\green136\blue16;}
{\*\expandedcolortbl;;\cssrgb\c77483\c52772\c75312;\cssrgb\c85469\c89006\c88685;\cssrgb\c50133\c80010\c80608;
\cssrgb\c5053\c63629\c65572;\cssrgb\c94531\c60201\c5727;}
\paperw11900\paperh16840\margl1440\margr1440\vieww11520\viewh8400\viewkind0
\deftab720
\pard\pardeftab720\partightenfactor0

\f0\fs24 \cf0 \expnd0\expndtw0\kerning0
\outl0\strokewidth0 \strokec2 #include\strokec3  \strokec4 <chrono>\strokec3 \
\strokec2 #include\strokec3  \strokec4 <memory>\strokec3 \
\strokec2 #include\strokec3  \strokec4 <thread>\strokec3 \
\strokec2 #include\strokec3  \strokec4 <functional>\strokec3 \
\strokec2 #include\strokec3  \strokec4 <cmath>\strokec3 \
\strokec2 #include\strokec3  \strokec4 <control_msgs/msg/joint_jog.hpp>\strokec3 \
\strokec2 #include\strokec3  \strokec4 <sensor_msgs/msg/joint_state.hpp>\strokec3 \
\strokec2 #include\strokec3  \strokec4 "rclcpp/rclcpp.hpp"\strokec3 \
\strokec2 #include\strokec3  \strokec4 "std_msgs/msg/string.hpp"\strokec3 \
\
using std::placeholders::_1;\
using \strokec5 namespace\strokec3  std::chrono_literals;\
\
\pard\pardeftab720\partightenfactor0
\cf0 \strokec5 class\strokec3  GoalMovementMover6 : \strokec5 public\strokec3  rclcpp::Node\
\{\
\strokec5 public:\strokec3 \
\'a0\'a0\'a0\'a0\strokec6 GoalMovementMover6\strokec3 ()\
\'a0\'a0\'a0\'a0: \strokec6 Node\strokec3 (\strokec4 "GoalMovementMover6"\strokec3 ), \strokec6 count_\strokec3 (\strokec4 0\strokec3 )\
\'a0\'a0\'a0\'a0\{\
\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0cb_group_ =\
\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0this->\strokec6 create_callback_group\strokec3 (\
\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0rclcpp::CallbackGroupType::MutuallyExclusive);\
\
\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0tm_group_ =\
\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0this->\strokec6 create_callback_group\strokec3 (\
\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0rclcpp::CallbackGroupType::MutuallyExclusive);\
\
\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0rclcpp::SubscriptionOptions options;\
\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\strokec6 options\strokec3 .\strokec6 callback_group\strokec3  = cb_group_;\
\
\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0publisherJointPosition_ =\
\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0this->\strokec6 create_publisher\strokec3 <control_msgs::msg::JointJog>( \strokec4 "/JointJog"\strokec3 , \strokec4 10\strokec3 );\
\
\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0subscriptionJointPosition_ =\
\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0this->\strokec6 create_subscription\strokec3 <sensor_msgs::msg::JointState>(\
\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\strokec4 "/joint_states"\strokec3 , \strokec4 10\strokec3 ,\
\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0std::\strokec6 bind\strokec3 (\
\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0&GoalMovementMover6::topic_jointStatesCallback,\
\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0this, _1),\
\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0options);\
\
\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0demandSubscription_ =\
\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0this->\strokec6 create_subscription\strokec3 <sensor_msgs::msg::JointState>(\
\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\strokec4 "/joint_demands"\strokec3 , \strokec4 10\strokec3 ,\
\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0std::\strokec6 bind\strokec3 (\
\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0&GoalMovementMover6::jointDemandsCallback,\
\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0this, _1),\
\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0options);\
\
\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0timer_ =\
\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0this->\strokec6 create_wall_timer\strokec3 (\
\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\strokec4 50\strokec3 ms,\
\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0std::\strokec6 bind\strokec3 (\
\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0&GoalMovementMover6::timer_callback,\
\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0this),\
\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0tm_group_);\
\
\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0known_states = \strokec4 false\strokec3 ;\
\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0new_demands_received = \strokec4 false\strokec3 ;\
\
\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0joint1_angle = joint2_angle = joint3_angle =\
\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0joint4_angle = joint5_angle = joint6_angle = \strokec4 0.0\strokec3 f;\
\
\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0jointdemand_1 = jointdemand_2 = jointdemand_3 =\
\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0jointdemand_4 = jointdemand_5 = jointdemand_6 = \strokec4 0.0\strokec3 f;\
\'a0\'a0\'a0\'a0\}\
\
\strokec5 private:\strokec3 \
\'a0\'a0\'a0\'a0\strokec5 void\strokec3  \strokec6 timer_callback\strokec3 ()\
\'a0\'a0\'a0\'a0\{\
\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\strokec6 move_my_robot\strokec3 ();\
\'a0\'a0\'a0\'a0\}\
\
\'a0\'a0\'a0\'a0\strokec5 void\strokec3  \strokec6 topic_jointStatesCallback\strokec3 (\
\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\strokec5 const\strokec3  sensor_msgs::msg::JointState::SharedPtr msg)\
\'a0\'a0\'a0\'a0\{\
\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\strokec2 if\strokec3  (\strokec6 msg\strokec3 ->\strokec6 position\strokec3 .\strokec6 size\strokec3 () >= \strokec4 6\strokec3 )\
\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\{\
\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0joint1_angle = \strokec6 msg\strokec3 ->\strokec6 position\strokec3 [\strokec4 0\strokec3 ];\
\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0joint2_angle = \strokec6 msg\strokec3 ->\strokec6 position\strokec3 [\strokec4 1\strokec3 ];\
\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0joint3_angle = \strokec6 msg\strokec3 ->\strokec6 position\strokec3 [\strokec4 2\strokec3 ];\
\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0joint4_angle = \strokec6 msg\strokec3 ->\strokec6 position\strokec3 [\strokec4 3\strokec3 ];\
\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0joint5_angle = \strokec6 msg\strokec3 ->\strokec6 position\strokec3 [\strokec4 4\strokec3 ];\
\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0joint6_angle = \strokec6 msg\strokec3 ->\strokec6 position\strokec3 [\strokec4 5\strokec3 ];\
\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0known_states = \strokec4 true\strokec3 ;\
\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\}\
\'a0\'a0\'a0\'a0\}\
\
\'a0\'a0\'a0\'a0\strokec5 void\strokec3  \strokec6 jointDemandsCallback\strokec3 (\
\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\strokec5 const\strokec3  sensor_msgs::msg::JointState::SharedPtr msg)\
\'a0\'a0\'a0\'a0\{\
\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\strokec2 if\strokec3  (\strokec6 msg\strokec3 ->\strokec6 position\strokec3 .\strokec6 size\strokec3 () >= \strokec4 6\strokec3 )\
\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\{\
\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0jointdemand_1 = \strokec6 msg\strokec3 ->\strokec6 position\strokec3 [\strokec4 0\strokec3 ];\
\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0jointdemand_2 = \strokec6 msg\strokec3 ->\strokec6 position\strokec3 [\strokec4 1\strokec3 ];\
\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0jointdemand_3 = \strokec6 msg\strokec3 ->\strokec6 position\strokec3 [\strokec4 2\strokec3 ];\
\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0jointdemand_4 = \strokec6 msg\strokec3 ->\strokec6 position\strokec3 [\strokec4 3\strokec3 ];\
\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0jointdemand_5 = \strokec6 msg\strokec3 ->\strokec6 position\strokec3 [\strokec4 4\strokec3 ];\
\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0jointdemand_6 = \strokec6 msg\strokec3 ->\strokec6 position\strokec3 [\strokec4 5\strokec3 ];\
\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0new_demands_received = \strokec4 true\strokec3 ;\
\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\}\
\'a0\'a0\'a0\'a0\}\
\
\'a0\'a0\'a0\'a0\strokec5 void\strokec3  \strokec6 send_msg\strokec3 (\strokec5 float\strokec3  v1, \strokec5 float\strokec3  v2, \strokec5 float\strokec3  v3,\
\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\strokec5 float\strokec3  v4, \strokec5 float\strokec3  v5, \strokec5 float\strokec3  v6)\
\'a0\'a0\'a0\'a0\{\
\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0control_msgs::msg::JointJog msg;\
\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\strokec6 msg\strokec3 .\strokec6 joint_names\strokec3  =\
\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\{\strokec4 "joint1"\strokec3 , \strokec4 "joint2"\strokec3 , \strokec4 "joint3"\strokec3 ,\
\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\strokec4 "joint4"\strokec3 , \strokec4 "joint5"\strokec3 , \strokec4 "joint6"\strokec3 \};\
\
\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\strokec6 msg\strokec3 .\strokec6 velocities\strokec3  =\
\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\{v1, v2, v3, v4, v5, v6\};\
\
\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\strokec6 publisherJointPosition_\strokec3 ->\strokec6 publish\strokec3 (msg);\
\'a0\'a0\'a0\'a0\}\
\
\'a0\'a0\'a0\'a0\strokec5 void\strokec3  \strokec6 move_my_robot\strokec3 ()\
\'a0\'a0\'a0\'a0\{\
\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\strokec2 if\strokec3  (!known_states) \strokec2 return\strokec3 ;\
\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\strokec2 if\strokec3  (!new_demands_received) \strokec2 return\strokec3 ;\
\
\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\strokec5 const\strokec3  \strokec5 float\strokec3  gain = \strokec4 0.25\strokec3 f;\
\
\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\strokec5 float\strokec3  v1 = \strokec4 0.0\strokec3 f, v2 = \strokec4 0.0\strokec3 f, v3 = \strokec4 0.0\strokec3 f;\
\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\strokec5 float\strokec3  v4 = \strokec4 0.0\strokec3 f, v5 = \strokec4 0.0\strokec3 f, v6 = \strokec4 0.0\strokec3 f;\
\
\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\strokec2 if\strokec3  (std::\strokec6 fabs\strokec3 (jointdemand_1 - joint1_angle) > \strokec4 0.03\strokec3 f)\
\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0v1 = gain * ((jointdemand_1 - joint1_angle) /\
\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0std::\strokec6 fabs\strokec3 (jointdemand_1 - joint1_angle));\
\
\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\strokec2 if\strokec3  (std::\strokec6 fabs\strokec3 (jointdemand_2 - joint2_angle) > \strokec4 0.03\strokec3 f)\
\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0v2 = gain * ((jointdemand_2 - joint2_angle) /\
\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0std::\strokec6 fabs\strokec3 (jointdemand_2 - joint2_angle));\
\
\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\strokec2 if\strokec3  (std::\strokec6 fabs\strokec3 (jointdemand_3 - joint3_angle) > \strokec4 0.03\strokec3 f)\
\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0v3 = gain * ((jointdemand_3 - joint3_angle) /\
\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0std::\strokec6 fabs\strokec3 (jointdemand_3 - joint3_angle));\
\
\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\strokec2 if\strokec3  (std::\strokec6 fabs\strokec3 (jointdemand_4 - joint4_angle) > \strokec4 0.03\strokec3 f)\
\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0v4 = gain * ((jointdemand_4 - joint4_angle) /\
\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0std::\strokec6 fabs\strokec3 (jointdemand_4 - joint4_angle));\
\
\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\strokec2 if\strokec3  (std::\strokec6 fabs\strokec3 (jointdemand_5 - joint5_angle) > \strokec4 0.03\strokec3 f)\
\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0v5 = gain * ((jointdemand_5 - joint5_angle) /\
\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0std::\strokec6 fabs\strokec3 (jointdemand_5 - joint5_angle));\
\
\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\strokec2 if\strokec3  (std::\strokec6 fabs\strokec3 (jointdemand_6 - joint6_angle) > \strokec4 0.03\strokec3 f)\
\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0v6 = gain * ((jointdemand_6 - joint6_angle) /\
\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0std::\strokec6 fabs\strokec3 (jointdemand_6 - joint6_angle));\
\
\'a0\'a0\'a0\'a0\'a0\'a0\'a0\'a0\strokec6 send_msg\strokec3 (v1, v2, v3, v4, v5, v6);\
\'a0\'a0\'a0\'a0\}\
\
\'a0\'a0\'a0\'a0\strokec5 bool\strokec3  known_states;\
\'a0\'a0\'a0\'a0\strokec5 bool\strokec3  new_demands_received;\
\
\'a0\'a0\'a0\'a0\strokec5 float\strokec3  joint1_angle, joint2_angle, joint3_angle;\
\'a0\'a0\'a0\'a0\strokec5 float\strokec3  joint4_angle, joint5_angle, joint6_angle;\
\
\'a0\'a0\'a0\'a0\strokec5 float\strokec3  jointdemand_1, jointdemand_2, jointdemand_3;\
\'a0\'a0\'a0\'a0\strokec5 float\strokec3  jointdemand_4, jointdemand_5, jointdemand_6;\
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
\'a0\'a0\'a0\'a0\strokec5 unsigned\strokec3  \strokec5 long\strokec3  \strokec5 long\strokec3  state = \strokec4 0\strokec3 ;\
\'a0\'a0\'a0\'a0\strokec5 size_t\strokec3  count_;\
\};\
\
\strokec5 int\strokec3  \strokec6 main\strokec3 (\strokec5 int\strokec3  argc, \strokec5 char\strokec3  \strokec5 *\strokec3  argv[])\
\{\
\'a0\'a0\'a0\'a0rclcpp::\strokec6 init\strokec3 (argc, argv);\
\'a0\'a0\'a0\'a0\strokec5 auto\strokec3  node = std::\strokec6 make_shared\strokec3 <GoalMovementMover6>();\
\'a0\'a0\'a0\'a0rclcpp::executors::MultiThreadedExecutor executor;\
\'a0\'a0\'a0\'a0\strokec6 executor\strokec3 .\strokec6 add_node\strokec3 (node);\
\'a0\'a0\'a0\'a0\strokec6 executor\strokec3 .\strokec6 spin\strokec3 ();\
\'a0\'a0\'a0\'a0rclcpp::\strokec6 shutdown\strokec3 ();\
\'a0\'a0\'a0\'a0\strokec2 return\strokec3  \strokec4 0\strokec3 ;\
\}\
}
