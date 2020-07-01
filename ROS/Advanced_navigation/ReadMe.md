# SLAM建图——Turtlebot3自动导航

此代码已通过gazebo搭建测试仿真地图，旨在通过SLAM进行建图并实现机器人的自动导航

### 说明1

若需要重新构图，可在gazebo中创建相应地图后添加至对应launch文件启动keyboard进行建图，或使用上一目录的SLAM-auto-mapping进行自动建图

### 说明2

此部分仅提供功能包，具体流程请参见ROS实验中的ppt

* catkin_make

* .bashrc文件中修改turtlebot3 model为burger模型（gedit ~/.bashrc）

* launch文件添加到对应的turtlebot3_gazebo目录下(文件中已给出 turtlebot3_house_final.launch文件)

* roslaunch turtlebot3_gazebo turtlebot3_house_final.launch

* roslaunch advanced_navi navi_expr.launch

* roslaunch advanced_navi sequence_goals.launch

### 效果展示

* 机器人仿真模型及地图模型

  ![png1](https://raw.githubusercontent.com/zacdeng/Image-hosting-service/master/ROS_Figures/ada1.png)

* 导航起始

  ![png2](https://raw.githubusercontent.com/zacdeng/Image-hosting-service/master/ROS_Figures/ada2.png)

* 导航进行中

  ![png3](https://github.com/zacdeng/Image-hosting-service/raw/master/ROS_Figures/ada3.png)

* 导航结束

  ![png4](https://github.com/zacdeng/Image-hosting-service/raw/master/ROS_Figures/ada4.png)