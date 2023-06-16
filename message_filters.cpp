#include <message_filters/subscriber.h>
#include <message_filters/synchronizer.h>
#include <message_filters/sync_policies/approximate_time.h>

    // Message Filters Subscribers
    message_filters::Subscriber<fiducial_msgs::FiducialTransformArray> sub_visual(nh, visual_topic, 1);
    message_filters::Subscriber<fiducial_msgs::FiducialTransformArray> sub_thermal(nh, thermal_topic, 1);
    message_filters::Subscriber<fiducial_msgs::FiducialTransformArray> sub_lidar(nh, lidar_topic, 1);
    typedef message_filters::sync_policies::ApproximateTime<fiducial_msgs::FiducialTransformArray, fiducial_msgs::FiducialTransformArray, fiducial_msgs::FiducialTransformArray> MySyncPolicy;
    message_filters::Synchronizer<MySyncPolicy> sync(MySyncPolicy(1), sub_visual, sub_thermal, sub_lidar);
    sync.registerCallback(boost::bind(&cb_getData, _1, _2, _3));
