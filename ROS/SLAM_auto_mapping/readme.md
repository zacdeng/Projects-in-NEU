# 功能包使用说明

此功能包为SLAM自动建图功能包，旨在代替人为通过键盘控制turtlebot3进行SLAM建图

* 启动已搭建好的gazebo仿真空间，对于本人而言是

source ./catkin_ws/devel/setup.bash && export TURTLEBOT3_MODEL=burger && roslaunch turtlebot3_gazebo turtlebot3_house_final.launch

* source ./catkin_ws/devel/setup.bash && export TURTLEBOT3_MODEL=burger && rosrun rviz rviz -d `rospack find turtlebot3_gazebo`/rviz/turtlebot3_gazebo_model.rviz
* source ./catkin_ws/devel/setup.bash && export TURTLEBOT3_MODEL=burger && roslaunch turtlebot3_slam turtlebot3_gmapping.launch set_base_frame:=base_footprint set_odom_frame:=odom set_map_frame:=map
* source ./catkin_ws/devel/setup.bash && export TURTLEBOT3_MODEL=burger && roslaunch explore_lite explore.launch