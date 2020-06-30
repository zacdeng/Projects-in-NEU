# 功能包使用说明

此功能包为SLAM自动建图功能包，旨在代替人为通过键盘控制turtlebot3进行SLAM建图

(以下步骤仅为个人使用总结，若实际运行过程出错请自行更改路径)

* 启动已搭建好的gazebo仿真空间，对于本人而言是

source ./catkin_ws/devel/setup.bash && export TURTLEBOT3_MODEL=burger && roslaunch turtlebot3_gazebo turtlebot3_house_final.launch

* source ./catkin_ws/devel/setup.bash && export TURTLEBOT3_MODEL=burger && rosrun rviz rviz -d `rospack find turtlebot3_gazebo`/rviz/turtlebot3_gazebo_model.rviz

* source ./catkin_ws/devel/setup.bash && export TURTLEBOT3_MODEL=burger && roslaunch turtlebot3_slam turtlebot3_gmapping.launch set_base_frame:=base_footprint set_odom_frame:=odom set_map_frame:=map

* source ./catkin_ws/devel/setup.bash && export TURTLEBOT3_MODEL=burger && roslaunch explore_lite explore.launch

  ### 效果展示

- 运行过程

  ![start](https://raw.githubusercontent.com/zacdeng/Image-hosting-service/master/ROS_Figures/1.png)

  ![half](https://raw.githubusercontent.com/zacdeng/Image-hosting-service/master/ROS_Figures/2.png)

  ![end](https://raw.githubusercontent.com/zacdeng/Image-hosting-service/master/ROS_Figures/3.png)

  

* 最终建图效果

  ![house-map](https://raw.githubusercontent.com/zacdeng/Image-hosting-service/master/ROS_Figures/4.png)

  

  

  