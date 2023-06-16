#include "ros/ros.h"
#include "std_msgs/String.h"
#include "/home/odroid/Desktop/gpio_test/wiringPi/wiringPi/wiringPi.h"
#include <mavros_msgs/State.h>
#include "sensor_msgs/Range.h"

//message_filters libs
#include <message_filters/subscriber.h>
#include <message_filters/synchronizer.h>
#include <message_filters/sync_policies/approximate_time.h>

/*
    typedef enum{
       locked, //drone in platform, locked
       launch, //drone in platform and ready for flight, oppened
       on_air, //drone on air, locked
       land //drone on air, ready to land, oppened
   }stateNames;
   
    stateNames currentState = locked;
 */ 
    ros::Time last;
    bool callback_flag = 0;
    ros::Duration IDLE(10.0);
    //ros::Duration TIME_INTERVAL_LAUNCH(10.0);
    //ros::Duration TIME_INTERVAL_LAND(10.0);
    
    
void myCallback(const mavros_msgs::State::ConstPtr& state, const sensor_msgs::Range::ConstPtr& d)
{
    
    callback_flag = true;
    if ( (state->armed  == true) && (d->range<=2.0) && ((state->mode == "LAND") || (state->mode=="GUIDED")) )
            {
                
                digitalWrite(0,HIGH);
            }
    else 
            {
                digitalWrite(0,LOW);
            }
    
    /*
    timer = ros::Time::now();
    std::cout << "timer:  " << timer.toSec() << std::endl;
    std::cout << "a dormir por  " <<  TIME_INTERVAL_LAUNCH.toSec() << std::endl;
    TIME_INTERVAL_LAUNCH.sleep();
   
    std::cout << "ros::Time::now:  " <<  ros::Time::now().toSec() << std::endl;
    std::cout << "passaram:  " <<  ros::Time::now().toSec() - timer.toSec() << std::endl;
   */
   
   //ROS_INFO("callback");
   
   /*
   switch(currentState)
   {
        case(locked):
        {
            ROS_INFO("locked");
            if (state->armed==true)
            {
                currentState = launch;
                //ros::Time timer = ros::Time::now();
            }
            
            digitalWrite(0, LOW); 
            break;
        }
        
        
        case(launch):
        {
            ROS_INFO("launch");
            //std::cout <<  " << ( ros::Time::now() -timer.toSec() )" << std::endl;
            if ( (state->mode  != "LAND") && (d->range>=2.0) )
            {
                currentState = on_air;
            }
            digitalWrite(0, HIGH); 
            break;
        }
        
        case(on_air):
        {
            ROS_INFO("on_air");
            if ((state->mode == "LAND") && (state->armed == true) && d->range <= 2.0 )
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
            if ( state->armed == false )
            {
                currentState = locked;
            }
            digitalWrite(0, HIGH); 
            break;
        }
        
   }
   */
   
   
  

}



int main(int argc, char **argv)
{
   //GPIO setup
   wiringPiSetup();
      pinMode(0, OUTPUT);
      digitalWrite(0, LOW); 
 
  ros::init(argc, argv, "subscriber");

  ros::NodeHandle n;
  
  //ros::Subscriber state = n.subscribe("mavros/state", 1, StateCallback);//passar para 2 com o msg_filters



    //2 subscribers 
  message_filters::Subscriber<mavros_msgs::State> sub_state(n, "mavros/state", 1);
  message_filters::Subscriber<sensor_msgs::Range> sub_range(n, "mavros/rangefinder/rangefinder", 1);
   
  
  //AproximateTime
  typedef message_filters::sync_policies::ApproximateTime<mavros_msgs::State, sensor_msgs::Range> MySyncPolicy;
  
  //state -> 1hz ,  rangefinder -> 5 hz
  message_filters::Synchronizer<MySyncPolicy> sync(MySyncPolicy(10), sub_state, sub_range);
    
  sync.registerCallback(boost::bind(&myCallback, _1, _2));
   
   
  
  
  
   ros::Rate rate(100);
   
   
   
   while(ros::ok()){
        //ROS_INFO("ok");
        ros::spinOnce();
        //std::cout << "" <<  ros::Time::now().toSec() - timer.toSec() << std::endl;
        if (callback_flag) 
        {
            last = ros::Time::now();
            callback_flag = false;
        } 
        else
        {
            if( (ros::Time::now().toSec() - last.toSec() ) > IDLE.toSec() )
            digitalWrite(0, LOW); 
            
        
        }
        
        
        rate.sleep();
   }
   
   digitalWrite(0, LOW); 
   
   //std::cout << "passaram:  " <<  ros::Time::now().toSec() - timer.toSec() << std::endl;

  return 0;
}
