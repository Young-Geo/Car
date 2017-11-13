#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "ros/ros.h"
#ifdef USE_TF
#include "tf/transform_broadcaster.h"
#include "tf/transform_listener.h"
#endif
#include "../../../devel/include/node_infrared/infr.h"
#include "../../../devel/include/node_ultrasonic/ultr.h"
#include "../../../devel/include/node_motion/motion.h"
#include "../../../devel/include/node_client/client.h"
#include "../../../devel/include/node_camera/camera.h"

#include "node_decm/robot.h"
#include "afx.h"

void	ultr_call_back(const node_ultrasonic::ultr::ConstPtr &ultrdis)
{
	ROS_INFO("ultr_call_back ultrdis = %f\n", ultrdis->distance);
}

void	infr_call_back(const node_infrared::infr::ConstPtr &infrlf)
{
	ROS_INFO("infr_call_back infr l %d, r %d\n", infrlf->left, infrlf->right);
}

void	client_call_back(const node_client::client::ConstPtr &clientmess)
{

}

void		decm_camera_call_back(const node_camera::camera::ConstPtr &camera)
{
	unsigned int size = 0, width = 0, height = 0;
	unsigned char buf[Kbyte(100)] = { 0 };
	
	ROS_INFO("node_decm sub cameramess ...\n");

	size = camera->size;
	width = camera->width;
	height = camera->height;

	ROS_INFO("node_decm receive camera size %d, width %d, height %d\n", size, width, height);
		
	if (!size || !width || !height) {
		ROS_INFO("node_decm receive camera data error\n");
		return;
	}
	
	xmemcpy(buf, camera->data.c_str(), size);
	return;
}

#ifdef USE_TF
int 	broadcast_baselink()
{
	static tf::TransformBroadcaster br;//广播用的
	tf::Transform tf;//需要被广播的变换关系
	
	tf.setOrigin(tf::Vector3(0.0, 0.0, 0.0));//设置在父坐标系中的原点
	//tf::Quaternion qu;
	//qu.setRPY(0, 0, 0);
	//tf.setRotation(qu);//设置旋转rpy
	tf.setRotation(tf::Quaternion(0, 0, 0, 1));//设置旋转rpy
	br.sendTransform(tf::StampedTransform(tf, ros::Time::now(), "word", "base_link"));
	return 0;
}

int		listen_transform_infr_left()
{
	tf::Transform_Listener tlis;
	tf::StampedTransform stf;

	tlis.waitForTransform("infr_left_link", "base_link", ros::Time(0), ros::Duration(3.0));
	tlis.lookupTransfrom("infr_left_link", "base_link", ros::Time(0), stf);
	
	return 0;
}

int		listen_transform_infr_right()
{
	tf::Transform_Listener tlis;
	tf::StampedTransform stf;

	tlis.waitForTransform("infr_right_link", "base_link", ros::Time(0), ros::Duration(3.0));
	tlis.lookupTransfrom("infr_right_link", "base_link", ros::Time(0), stf);
	
	return 0;
}

int		listen_transform_ultr()
{
	tf::Transform_Listener tlis;
	tf::StampedTransform stf;

	tlis.waitForTransform("ultr_link", "base_link", ros::Time(0), ros::Duration(3.0));
	tlis.lookupTransfrom("ultr_link", "base_link", ros::Time(0), stf);
	
	return 0;
}


int listen_transform_wheel_left_front()
{
	tf::Transform_Listener tlis;
	tf::StampedTransform stf;

	tlis.waitForTransform("wheel_left_front_link", "base_link", ros::Time(0), ros::Duration(3.0));
	tlis.lookupTransform("wheel_left_front_link", "base_link", ros::Time(0), stf);
	return 0;
}

int listen_transform_wheel_left_back()
{
	tf::Transform_Listener tlis;
	tf::StampedTransform stf;

	tlis.waitForTransform("wheel_left_back_link", "base_link", ros::Time(0), ros::Duration(3.0));
	tlis.lookupTransform("wheel_left_back_link", "base_link", ros::Time(0), stf);
	return 0;
}

int listen_transform_wheel_right_front()
{
	tf::Transform_Listener tlis;
	tf::StampedTransform stf;

	tlis.waitForTransform("wheel_right_front_link", "base_link", ros::Time(0), ros::Duration(3.0));
	tlis.lookupTransform("wheel_right_front_link", "base_link", ros::Time(0), stf);
	return 0;
}

int listen_transform_wheel_right_back()
{
	tf::Transform_Listener tlis;
	tf::StampedTransform stf;

	tlis.waitForTransform("wheel_right_back_link", "base_link", ros::Time(0), ros::Duration(3.0));
	tlis.lookupTransform("wheel_right_back_link", "base_link", ros::Time(0), stf);
	return 0;
}

#endif



int		main(int argc, const char **argv)
{

	ros::init(argc, (char **)argv, "node_decm");	

	ros::NodeHandle n;
	ros::Rate r(10);
	ros::Publisher pubmotion;
	ros::Subscriber subultr;
	ros::Subscriber subinfr;	
	ros::Subscriber subclient;
	ros::Subscriber subcamera;
	
	robot_t robot;
	
	xassert(!robot_init(&robot));

	pubmotion = n.advertise<node_motion::motion>("motionmess", 1000);
	subultr = n.subscribe("ultrmess", 1000, ultr_call_back);
	subinfr = n.subscribe("infrmess", 1000, infr_call_back);	
	subclient = n.subscribe("clientmess", 1000, client_call_back);
	subcamera = n.subscribe("cameramess", 1000, decm_camera_call_back);
	
	
	while (ros::ok())
	{
		ROS_INFO("node_decm ...\n");
#ifdef USE_TF
		broadcast_baselink();
#endif
		ros::spinOnce();
		r.sleep();
	}
	return 0;
}
