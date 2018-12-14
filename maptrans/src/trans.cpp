#include <ros/ros.h>
#include <nav_msgs/OccupancyGrid.h>

using namespace nav_msgs;

class Maptranslate
{
public:
	int flag1;
	int flag2;
	ros::Duration D;
	ros::Duration d;
	nav_msgs::OccupancyGrid map_data;
	nav_msgs::OccupancyGrid costmap_data;
	
	Maptranslate()//construction
	{
		flag1=0;
		flag2=0;
		d=ros::Duration(1);
		//Topic you want to publish
		pub_ = n_.advertise<nav_msgs::OccupancyGrid>("/fusion_map", 1);
		pub_1 = n_.advertise<nav_msgs::OccupancyGrid>("/pub_map", 1);
		pub_2 = n_.advertise<nav_msgs::OccupancyGrid>("/pub_costmap", 1);
		//Topic you want to subscribe
		map_sub_ = n_.subscribe("map",1000,&Maptranslate::callback1,this);
		costmap_sub_ = n_.subscribe("/move_base_node/global_costmap/costmap",1000,&Maptranslate::callback2,this);
	}
	void callback1(const nav_msgs::OccupancyGridConstPtr& map)
	{
		flag1=1;
		map_data.header=map->header;
		if(flag2==1)
		{
			D=map_data.header.stamp-costmap_data.header.stamp;
			if(D>d)
				ROS_INFO("c1 out of time:%f\n",D.toSec());
			else
			{
				nav_msgs::OccupancyGrid new_data;
				uint32_t w=map->info.width;
				uint32_t h=map->info.height;
				uint32_t num=w*h;
				uint32_t w1=costmap_data.info.width;
				uint32_t h1=costmap_data.info.height;
				uint32_t num1=w1*h1;
				ROS_INFO("w=%d,h=%d,w*h=%d,cost:w*h=%d\n",w,h,num,num1);
				new_data.header.stamp=map->header.stamp;
				new_data.info=map->info;
				for(uint32_t i=0;i<num;i++)
				{
					uint8_t occupy=costmap_data.data.at(i);
					
					if(occupy!=0)
					{
						new_data.data.push_back(occupy);
						ROS_INFO("%d ",occupy);
					}
					else
					{
						new_data.data.push_back(map->data[i]);
						ROS_INFO("%d ",map->data[i]);					
					}
				}
				ROS_INFO("pub\n");
				pub_.publish(new_data);
			}
		}
	}
	void callback2(const nav_msgs::OccupancyGridConstPtr& costmap)
	{
		flag2=1;
		costmap_data.header=costmap->header;
		costmap_data.data=costmap->data;
		ROS_INFO("cost map:w=%d,h=%d",costmap->info.width,costmap->info.height);
		if(flag1==1)
		{
			D=costmap_data.header.stamp-map_data.header.stamp;
			if(D>d)
				ROS_INFO("c2 out of time:%f\n",D.toSec());	
			else
				ROS_INFO("ok2\n");
		}
		//your callback function
		//pub_2.publish(costmap);
	}
private:
	//defination of class members
	ros::NodeHandle n_; 
	ros::Publisher pub_;
	ros::Publisher pub_1;	
	ros::Publisher pub_2;
	ros::Subscriber map_sub_;
	ros::Subscriber costmap_sub_;
};

int main(int argc, char** argv){
	ros::init(argc, argv, "fusionmap_publisher");
	Maptranslate maptranslater;//create an object
	ros::spin();
	return 0;
}

