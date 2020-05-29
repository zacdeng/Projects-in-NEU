#include "ros/ros.h"
#include "hw1/hw1_srv.h"

bool add(hw1::hw1_srv::Request  &req,
         hw1::hw1_srv::Response &res)
{
  res.ret = req.A * req.B ;
  ROS_INFO("request: A=%d, B=%d", (int)req.A, (int)req.B);
  ROS_INFO("sending back response: [%d]", (int)res.ret);
  return true;
}

int main(int argc, char **argv)
{
  ros::init(argc, argv, "multiply_2_ints_server");
  ros::NodeHandle n;

  ros::ServiceServer service = n.advertiseService("multiply_2_ints", add);
  ROS_INFO("Who are you? Please help to calculate X*Y.");
  ros::spin();

  return 0;
}
