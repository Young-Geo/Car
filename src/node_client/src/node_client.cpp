#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "ros/ros.h"
#include "node_motion/motion.h"
#include "node_camera/camera.h"
#include "node_client/client.h"
#include "node_client/client_socket.h"

#include "afx.h"

#define COMMIP "192.168.0.104"
#define COMMPORT 9001

#define VIDEOIP COMMIP
#define VIDEOPORT 9002

enum _comm {
	CONTROL_MODEL = 0x01,
	AUTO_MODEL = 0x02,
	COMM_LEFT = 0x03,
	COMM_RIGHT = 0x04,
	COMM_FORWARD = 0x05,
	COMM_BACK = 0x06,
	COMM_STOP = 0x07
};


static client_socket_t * scomm_socket = NULL;	
static client_socket_t * svideo_socket = NULL;

typedef struct _Client_pub_t {
	ros::Publisher *pub_client;
	ros::Publisher *pub_motion;
	node_client::client *client;
	node_motion::motion *motion;
} Client_pub_t;




void		camera_call_back(const node_camera::camera::ConstPtr &camera)
{
	unsigned int size = 0, width = 0, height = 0;
	unsigned char buf[Kbyte(100)] = { 0 };
	
	ROS_INFO("node_client sub cameramess ...\n");

	size = camera->size;
	width = camera->width;
	height = camera->height;

	ROS_INFO("receive camera size %d, width %d, height %d\n", size, width, height);
		
	if (!size || !width || !height) {
		ROS_INFO("receive camera data error\n");
		return;
	}

	xmemcpy(buf, &size, sizeof(size));
	xmemcpy(buf + sizeof(size), camera->data.c_str(), size);
	if (svideo_socket)
		client_socket_push(svideo_socket, buf, size + sizeof(size));
	return;
}

int	temp_parse_buf(unsigned char *buf, int buf_size, unsigned char *data, int data_size)
{
	xmemcpy(data, buf, buf_size);
	return 0;
}

int		comm_call_back(int fd, unsigned char *buf, int buf_size, void *arg)
{
	Client_pub_t *pub = NULL;
	//node_client::client client;
	//node_motion::motion motion;
	
	u8 data[BUFFER_SIZE] = { 0 }, type = 0;
	int i = 0;

	xmessage("comm_call_back\n");
	
	xassert(buf);
	xassert(arg);
	xassert((buf_size > 0));

	pub = (Client_pub_t *)arg;
	xassert(pub->pub_client);
	xassert((pub->pub_motion));


	//解析buf---
	temp_parse_buf(buf, buf_size, data, sizeof(data));

	for (i = 0; i < buf_size; ++i)
	{
		type = data[i];
		ROS_INFO("type %d\n", type);
		xmessage("pusb next type %d\n", type);
		
		switch (type)
		{
			case CONTROL_MODEL:
			{
				pub->client->type = CONTROL_MODEL;				
				pub->pub_client->publish(*(pub->client));
			}
			break;
	
			case AUTO_MODEL:
			{
				pub->client->type = AUTO_MODEL; 			
				pub->pub_client->publish(*(pub->client));
			}
			break;
	
			case COMM_LEFT:
			{
				pub->motion->type = COMM_LEFT;				
				pub->pub_motion->publish(*(pub->motion));
			}
			break;
	
			case COMM_RIGHT:
			{
				pub->motion->type = COMM_RIGHT; 			
				pub->pub_motion->publish(*(pub->motion));
			}
			break;
	
			case COMM_FORWARD:
			{
				pub->motion->type = COMM_FORWARD;				
				pub->pub_motion->publish(*(pub->motion));
			}
			break;
	
			case COMM_BACK:
			{
				pub->motion->type = COMM_BACK;				
				pub->pub_motion->publish(*(pub->motion));
			}
			break;
	
			case COMM_STOP:
			{
				pub->motion->type = COMM_STOP;				
				pub->pub_motion->publish(*(pub->motion));
			}
			break;
			
			default:
			break;
		}
	}
	
	return 0;
}

int		video_call_back(int fd, unsigned char *buf, int buf_size, void *arg)
{
	xmessage("video_call_back\n");
	return 0;
}


int			main(int argc, const char **argv)
{
	ros::init(argc, (char **)argv, "node_client");

	ros::NodeHandle n;
	ros::Publisher pub_client;
	ros::Publisher pub_motion;
	ros::Subscriber sub;
	ros::Rate r(10);
	node_client::client client;
	node_motion::motion motion;

	client_socket_t * comm_socket = NULL;	
	client_socket_t * video_socket = NULL;

	Client_pub_t pub;

	pub_client = n.advertise<node_client::client>("clientmess", 1000);
	pub_motion = n.advertise<node_motion::motion>("motiontype_mess", 1000);
	sub = n.subscribe("cameramess", 1000, camera_call_back);

	xmemset(&pub, 0, sizeof(pub));
	xmemset(&client, 0, sizeof(client));
	xmemset(&motion, 0, sizeof(motion));
	
	pub.pub_client = &pub_client;	
	pub.pub_motion = &pub_motion;
	pub.client = &client;
	pub.motion = &motion;

	xassert((comm_socket = client_socket_init((char *)COMMIP, COMMPORT)));
	client_socket_set_call_back(comm_socket, comm_call_back, &pub);
	
	xassert((video_socket = client_socket_init((char *)VIDEOIP, VIDEOPORT)));
	client_socket_set_call_back(video_socket, video_call_back, &pub);

	scomm_socket = comm_socket;
	svideo_socket = video_socket;

	while (ros::ok())
	{		
		ROS_INFO("node_client ...\n");
		client_socket_spinOnce(comm_socket);
		client_socket_spinOnce(video_socket);
		ros::spinOnce();
		r.sleep();
	}
	//释放资源
	client_socket_destory(comm_socket);
	client_socket_destory(video_socket);
	return 0;
}
