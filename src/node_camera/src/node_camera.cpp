#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "ros/ros.h"
#include "node_camera/camera.h"
#include "node_camera/v4l2.h"




int			main(int argc, const char **argv)
{
	ros::init(argc, (char **)argv, "node_camera");

	ros::NodeHandle n;
	ros::Rate r(10);
	ros::Publisher pub;
	node_camera::camera camera;
	v4l2_t *v4l2 = NULL;
	unsigned char buf[Mbyte(1)];
	int buf_count = 0;	
	std::stringstream ss;

	pub = n.advertise<node_camera::camera>("cameramess", 1000);
	
	xassert((v4l2 = v4l2_init((char *)DEVICE, WIDTH, HEIGHT, FORMAT)));
	v4l2_startcapture(v4l2);

	while (ros::ok())
	{
		ROS_INFO("node_camera ...\n");
		if (v4l2_collect( v4l2, buf, &buf_count)) {
			ROS_INFO("collect error\n");
			continue;
		}

		ss << buf;
		camera.size = buf_count;
		camera.width = WIDTH;
		camera.height = HEIGHT;
		camera.data = ss.str();
		ss.clear();
		
		
		pub.publish(camera);
		ros::spinOnce();
		r.sleep();
	}
	
	v4l2_stopcapture(v4l2);
	v4l2_destory(v4l2);
	
	return 0;
}
