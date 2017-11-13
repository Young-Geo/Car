#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/types.h>
#include <unistd.h>
#include <errno.h>
#include <wiringPi.h>
#include <softPwm.h>

#include "ros/ros.h"
#ifdef USE_TF
#include "tf/transform_broadcaster.h"
#include "tf/transform_listener.h"
#endif
#include "node_ultrasonic/ultr.h"

#define Trig    28
#define Echo    29

void ultraPiinit(void)
{
    wiringPiSetup();
    pinMode(Echo, INPUT);
    pinMode(Trig, OUTPUT);
}


float disMeasure(void)
{
    struct timeval tv1;
    struct timeval tv2;
    long start, stop;
    float dis;

    digitalWrite(Trig, LOW);
    delayMicroseconds(2);

    ROS_INFO("%s\n", "start send Ultrasonic");
    digitalWrite(Trig, HIGH);
    delayMicroseconds(10);
    digitalWrite(Trig, LOW);
    ROS_INFO("%s\n", "stop send Ultrasonic");


    while(!(digitalRead(Echo) == 1));
    gettimeofday(&tv1, NULL);


    while(!(digitalRead(Echo) == 0));
    gettimeofday(&tv2, NULL);

    start = tv1.tv_sec * 1000000 + tv1.tv_usec;
    stop  = tv2.tv_sec * 1000000 + tv2.tv_usec;

    dis = (float)(stop - start) / 1000000 * 34000 / 2;

    ROS_INFO("distance %f\n", dis);

    return dis;
}

#ifdef USE_TF
int	broadcaster_ultr()
{
	static tf::TransformBroadcaster br;
	tf::Transfrom tf;
	tf.setOrigin(tf::vector3(10.0, 0.0, 2.5));
	tf.setRotation(tf::Quaternion(0, 0, 0, 1));
	br.sendTransform(tf::StampedTransform(tf, ros::Time::now(), "base_link", "ultr_link"));
}
#endif


int main(int argc, const char **argv)
{
    ros::init(argc, (char**)argv, "node_ultrasonic");
    ros::NodeHandle n;
    ros::Publisher pub = n.advertise<node_ultrasonic::ultr>("ultrmess", 1000);
    ros::Rate r(10);

    node_ultrasonic::ultr ultrdis;

    ultraPiinit();

    while (ros::ok())
    {
        ultrdis.distance = disMeasure();//Computing distance

        pub.publish(ultrdis);
#ifdef USE_TF
		broadcaster_ultr();
#endif

        ros::spinOnce();//spin one 

        r.sleep();
    }
    return 0;
}
