#include <ros/ros.h>
#include <nav_msgs/OccupancyGrid.h>
#include <message_filters/subscriber.h>
#include <message_filters/time_synchronizer.h>
#include <message_filters/sync_policies/approximate_time.h>

class Maptranslate
{
public:
	Maptranslate()//construction
	{
		//Topic you want to publish
		pub_ = n_.advertise<nav_msgs::OccupancyGrid>("/fusion_map", 1);
		//Topic you want to subscribe
		map_sub_ = n_.subscribe("/map",1000);
		costmap_sub_ = n_.subscribe("/move_base_node/global_costmap/costmap",1000);
		TimeSynchronizer<OccupancyGrid, OccupancyGrid> sync(map_sub_, costmap_sub_, 10);		
		sync.registerCallback(boost::bind(&callback, _1, _2));		
		//message_filters::Synchronizer<sync_policy_classification> sync(sync_policy_classification(100), info_sub, pose_sub);
		//TimeSynchronizer<CameraInfo, PoseStamped> sync(info_sub, pose_sub, 10);
		
	}
	void callback(const nav_msgs::OccupancyGridConstPtr& map,const nav_msgs::OccupancyGridConstPtr& costmap)
	{
		//your callback function
		
		pub_.publish(map);
	}
private:
	//defination of class members
	ros::NodeHandle n_; 
	ros::Publisher pub_;
	message_filters::Subscriber<OccupancyGrid> map_sub_;
	message_filters::Subscriber<OccupancyGrid> costmap_sub_;
};

int main(int argc, char** argv){
	ros::init(argc, argv, "fusionmap_publisher");
	Maptranslate maptranslater;//create an object
	ros::spin();
	return 0;
}

