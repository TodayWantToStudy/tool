#include <ros/ros.h>
#include <std_msgs/Float64.h>
#include <nav_msgs/OccupancyGrid.h>

class MapOccupancy
{
public:
    bool fm_flag;
    std_msgs::Float64 fm_percent;
    nav_msgs::OccupancyGrid map_data;
    MapOccupancy()
    {
        fm_flag=false;
        pub_1 = n_.advertise<std_msgs::Float64>("/occpercent",1000);
        pub_2 = n_.advertise<std_msgs::Float64>("/fm_occp",1000);
        sub_1 = n_.subscribe<nav_msgs::OccupancyGrid>("/map",1000,&MapOccupancy::callback1,this);
        sub_2 = n_.subscribe<nav_msgs::OccupancyGrid>("/full_map",1000,&MapOccupancy::callback2,this);
    }
    void callback2(const nav_msgs::OccupancyGridConstPtr& map)
    {
        if(fm_flag)
        {
            //pub_2.publish(fm_percent);
            ROS_INFO("fm:%f",fm_percent.data);
            return;
        }    
        else
        {
            fm_flag=true;
            fm_percent.data=0.0;
            uint32_t pixnum=0;
            double resolution=map->info.resolution;
            uint32_t w=map->info.width;
            uint32_t h=map->info.height;
            uint32_t num=w*h;
            ROS_INFO("FM:w=%d,h=%d,w*h=%d\n",w,h,num);
            for(uint32_t i=0;i<num;i++)
            {
                uint8_t occupy=map->data.at(i);
                if(occupy==0)
                    pixnum++;
            }
            fm_percent.data=pixnum*resolution;
            //ROS_INFO("callback\n");
            pub_2.publish(fm_percent);
        }
        
    }
    void callback1(const nav_msgs::OccupancyGridConstPtr& map)
    {
        std_msgs::Float64 percent;
        percent.data=0.0;
        uint32_t pixnum=0;
        double resolution=map->info.resolution;
        uint32_t w=map->info.width;
        uint32_t h=map->info.height;
        uint32_t num=w*h;
        ROS_INFO("M:w=%d,h=%d,w*h=%d\n",w,h,num);
        for(uint32_t i=0;i<num;i++)
        {
            uint8_t occupy=map->data.at(i);
            if(occupy==0)
                pixnum++;
        }
        percent.data=pixnum*resolution;
        //ROS_INFO("callback\n");
        if(fm_flag)
        {
            percent.data=percent.data/fm_percent.data;
            pub_1.publish(percent);
        }    
    }
private:
    ros::NodeHandle n_; 
	ros::Publisher pub_1;
    ros::Publisher pub_2;
    ros::Subscriber sub_1;
    ros::Subscriber sub_2;
};
int main(int argc, char** argv){
	ros::init(argc, argv, "occpuy_cal");
	MapOccupancy MOOjectory;//create an object
	ros::spin();
	return 0;
}