#include <ros/ros.h>
#include <message_filters/subscriber.h>
#include <message_filters/time_synchronizer.h>
#include <message_filters/sync_policies/approximate_time.h>
#include <nav_msgs/OccupancyGrid.h>

 
using namespace nav_msgs;
using namespace message_filters;
 
void callback(const OccupancyGridConstPtr& map, const OccupancyGridConstPtr& costmap)
{
  // Solve all of perception here...
  // ros.info("ok\n");
  ROS_INFO("ok\n");
}
 
int main(int argc, char** argv)
{
  ros::init(argc, argv, "fusionmap_node");
 
  ros::NodeHandle nh;
 
  message_filters::Subscriber<OccupancyGrid> map_sub(nh, "/map", 1000);
  message_filters::Subscriber<OccupancyGrid> costmap_sub(nh, "/move_base_node/global_costmap/costmap", 1000);

  //TimeSynchronizer<OccupancyGrid, OccupancyGrid> sync(map_sub, costmap_sub, 1);
  typedef sync_policies::ApproximateTime<OccupancyGrid, OccupancyGrid> MySyncPolicy;
  Synchronizer<MySyncPolicy> sync(MySyncPolicy(5), map_sub, costmap_sub);

  sync.registerCallback(boost::bind(&callback, _1, _2));
 
  ros::spin();
 
  return 0;
}

