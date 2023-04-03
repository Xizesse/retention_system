#include "ros/ros.h"
#include "std_msgs/String.h"
#include "/home/odroid/Desktop/gpio_test/wiringPi/wiringPi/wiringPi.h"
#include <mavros_msgs/State.h>

//bibliotecas do message_filters
#include <message_filters/subscriber.h>
#include <message_filters/synchronizer.h>
#include <message_filters/sync_policies/approximate_time.h>


    typedef enum{
       locked,
       launch,
       on_air,
       land
   }stateNames;
   
    stateNames currentState = preso;
    
    ros::Time timer;
    //ros::Duration TIME_INTERVAL_LAUNCH(10.0);
    //ros::Duration TIME_INTERVAL_LAND(10.0);
    
    
void StateCallback(const mavros_msgs::State::ConstPtr& msg)
{
    
    /*
    timer = ros::Time::now();
    std::cout << "timer:  " << timer.toSec() << std::endl;
    std::cout << "a dormir por  " <<  TIME_INTERVAL_LAUNCH.toSec() << std::endl;
    TIME_INTERVAL_LAUNCH.sleep();
   
    std::cout << "ros::Time::now:  " <<  ros::Time::now().toSec() << std::endl;
    std::cout << "passaram:  " <<  ros::Time::now().toSec() - timer.toSec() << std::endl;
   */
   
   
   /*
   
   */
   switch(currentState)
   {
        case(locked):
        {
            ROS_INFO("locked");
            if (msg->armed==true)
            {
                currentState = launch;
                ros::Time timer = ros::Time::now();
            }
            digitalWrite(0, LOW); 
            break;
        }
        
        case(launch):
        {
            ROS_INFO("launch");
            //std::cout <<  " << ( ros::Time::now() -timer.toSec() )" << std::endl;
            if ( 1  /* rangefinder >= 1 metro */ )//TIME_INTERVAL_LAUNCH
            {
                currentState = no_ar;
            }
            digitalWrite(0, HIGH); 
            break;
        }
        case(on_air):
        {
            ROS_INFO("on_air");
            if (1 /* mode == "LAND"  &&  rangefinder <= 2 m*/)
            {
                currentState = land;
                ros::Time timer = ros::Time::now();
                
            }
            digitalWrite(0, LOW); 
            break;
        }
        
        case(land):
        {
            ROS_INFO("land");
            if ( 1 /* armed == True */)
            {
                currentState = locked;
            }
            digitalWrite(0, HIGH); 
            break;
        }
        
   }
  

}

int main(int argc, char **argv)
{
   
   wiringPiSetup();
      pinMode(0, OUTPUT);
      digitalWrite(0, LOW); 
 
  ros::init(argc, argv, "listener");

  ros::NodeHandle n;
  
  ros::Subscriber state = n.subscribe("mavros/state", 1, StateCallback);

  
   
   ros::Rate rate(100);
   while(ros::ok()){
   
        ros::spinOnce();
        rate.sleep();
   }
   
   digitalWrite(0, LOW); 

  return 0;
}
