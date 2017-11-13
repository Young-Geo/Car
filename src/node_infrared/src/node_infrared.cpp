#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <wiringPi.h>

#include "ros/ros.h"
#ifdef USE_TF
#include "tf/transform_broadcaster.h"
#include "tf/transform_listener.h"
#endif
#include "node_infrared/infr.h"

#define LEFT	27
#define RIGHT	26

int		infrPiinit()
{
	wiringPiSetup();
	pinMode (LEFT, INPUT);	//IN1
    pinMode (RIGHT, INPUT);	//IN2
	return 0;
}

int		read_infr(node_infrared::infr &infr)
{
	int l, r;
	
	r = digitalRead(RIGHT);//有信号表明在白色区域，车子底板上L亮；没信号表明压在黑线上，车子底板上L灭
	l = digitalRead(LEFT);//有信号表明在白色区域，车子底板上L亮；没信号表明压在黑线上，车子底板上L灭
	infr.left = (l == LOW);
	infr.right = (r == LOW);

	ROS_INFO("INFR L %d, R %d\n", infr.left, infr.right);
	
	return 0;
}

#ifdef USE_TF
int		broadcase_infr_left()
{
	static tf::TransformBroadcaster br;
	tf::Transform tf;
	
	tf.setOrigin(tf::vector3(7.5, -5.0, 2.5));
	tf.setRotation(tf::Quaternion(0, 0, 0, 1));
	br.sendTransform(tf::StampedTransform(tf, ros::Time::now(), "base_link", "infr_left_link"));
	
	return 0;
}

int		broadcase_infr_right()
{
	static tf::TransformBroadcaster br;
	tf::Transform tf;
	
	tf.setOrigin(tf::vector3(7.5, 5.0, 2.5));
	tf.setRotation(tf::Quaternion(0, 0, 0, 1));
	br.sendTransform(tf::StampedTransform(tf, ros::Time::now(), "base_link", "infr_right_link"));
	
	return 0;
}
#endif

int		main(int argc, const char **argv)
{
	ros::init(argc, (char **)argv, "node_infrared");

	ros::NodeHandle n;
	ros::Rate r(10);
	ros::Publisher pub;

	node_infrared::infr infr;

	infrPiinit();

	pub = n.advertise<node_infrared::infr>("infrmess", 1000);

	while (ros::ok())
	{
		ROS_INFO("node_infrared ...\n");
		read_infr(infr);
#ifdef USE_TF
		broadcase_infr_left();
		broadcase_infr_right();
#endif
		pub.publish(infr);
		r.sleep();
	}
	return 0;
}
