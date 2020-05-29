#include "ros/ros.h"
#include "hw1/hw1_srv.h"
#include <cstdlib>

int main(int argc, char **argv)
{
  ros::init(argc, argv, "multiply_2_ints_client");
  if (argc != 3)
  {
    ROS_INFO("usage: multiply_2_ints_client A B ");
    return 1;
  }

  ros::NodeHandle n;
  ros::ServiceClient client = n.serviceClient<hw1::hw1_srv>("multiply_2_ints");
  hw1::hw1_srv srv;
  srv.request.A = atoll(argv[1]);
  srv.request.B = atoll(argv[2]);
  if (client.call(srv))
  {
    ROS_INFO("I'm 邓志豪, 20173878. Result is: %ld", (long int)srv.response.ret);
  }
  else
  {
    ROS_ERROR("Failed to call service multiply_two_ints");
    return 1;
  }

  return 0;
}
