#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <time.h>
#include <wiringPi.h>
#include <softPwm.h>

#include "ros/ros.h"
#ifdef USE_TF
#include "tf/transform_broadcaster.h"
#include "tf/transform_listener.h"
#endif
#include "node_motion/motion.h"

#include "g_robot.h"


#define PINONE 1
#define PINTWO 4
#define PINTHREE 5
#define PINFOUR 6
#define PWMMIN 1
#define PWMMAX 500

#define MOTOR_GO_FORWARD   digitalWrite(1,HIGH);digitalWrite(4,LOW);digitalWrite(5,HIGH);digitalWrite(6,LOW)
#define MOTOR_GO_BACK      digitalWrite(4,HIGH);digitalWrite(1,LOW);digitalWrite(6,HIGH);digitalWrite(5,LOW)
#define MOTOR_GO_RIGHT     digitalWrite(1,HIGH);digitalWrite(4,LOW);digitalWrite(6,HIGH);digitalWrite(5,LOW)
#define MOTOR_GO_LEFT      digitalWrite(4,HIGH);digitalWrite(1,LOW);digitalWrite(5,HIGH);digitalWrite(6,LOW)
#define MOTOR_GO_STOP      digitalWrite(1, LOW);digitalWrite(4,LOW);digitalWrite(5, LOW);digitalWrite(6,LOW)


static int isdanger = 0;



void        motion_call_back(const node_motion::motion::ConstPtr & motionmsg)
{
	if (DANGER == motionmsg->type) {//后边应该加上新的东西允许出现危险紧急停止信息，之后发来调整信息 允许其调整小车的位姿
		MOTOR_GO_STOP;
		isdanger = 1;
		return;
	}

	if (SAFE == motionmsg->type) {
		MOTOR_GO_STOP;
		isdanger = 0;
		return;
	}
	
    ROS_INFO("LF_FORWARD PWM = %d\n", motionmsg->lf_forward);
    ROS_INFO("RI_FORWARD PWM = %d\n", motionmsg->ri_forward);
    ROS_INFO("LF_BACK PWM = %d\n", motionmsg->lf_back);
    ROS_INFO("RI_BACK PWM = %d\n", motionmsg->ri_back);

    //
    softPwmWrite(PINONE, motionmsg->lf_forward);
	softPwmWrite(PINTWO, motionmsg->ri_forward); //4
    softPwmWrite(PINTHREE, motionmsg->lf_back); //5
    softPwmWrite(PINFOUR, motionmsg->ri_back); //6
    //
}

void        type_call_back(const node_motion::motion::ConstPtr & tpemsg)
{
	//motion_type BACK
	unsigned short type = 0;
	
	type = tpemsg->type;
	
	
	if (1 == isdanger && 1 != tpemsg->isadj_frame) {//有危险的时候可以允许手动调整的数据通过
		//MOTOR_GO_STOP;//可以给Client 上报数据
		return;
	}
	
	switch (type)
	{
		case COMM_LEFT:
			MOTOR_GO_LEFT;
		break;

		case COMM_RIGHT:
			MOTOR_GO_RIGHT;
		break;

		case COMM_FORWARD:
			MOTOR_GO_FORWARD;
		break;

		case COMM_BACK:
			MOTOR_GO_BACK;
		break;

		case COMM_STOP:
			MOTOR_GO_STOP;
		break;
	}
	ROS_INFO("type %d\n", type);
}


void        motionPiinit()
{

    wiringPiSetup();

    pinMode(PINONE, OUTPUT);    //IN1
    pinMode(PINTWO, OUTPUT);    //IN2
    pinMode(PINTHREE, OUTPUT);    //IN3
    pinMode(PINFOUR, OUTPUT);    //IN4

    softPwmCreate(PINONE, PWMMIN, PWMMAX);   
    softPwmCreate(PINTWO, PWMMIN, PWMMAX);
    softPwmCreate(PINTHREE, PWMMIN, PWMMAX);
    softPwmCreate(PINFOUR, PWMMIN, PWMMAX);

}

#ifdef USE_TF

int broadcase_wheel_left_front()
{
	static tf::Transform_Broadcaster br;
	tf::TransFrom tf;

	tf.setOrigin(tf::vector3(5.0, 5.0, 0.0));
	tf.setRotation(tf::Quaternion(0, 0, 0, 1));
	br.sendTransform(tf::StampedTransform(tf, ros::Time::now(), "base_link", "wheel_left_front_link"));

	return 0;
}

int broadcase_wheel_left_back()
{
	static tf::Transform_Broadcaster br;
	tf::TransFrom tf;

	tf.setOrigin(tf::vector3(-5.0, 5.0, 0.0));
	tf.setRotation(tf::Quaternion(0, 0, 0, 1));
	br.sendTransform(tf::StampedTransform(tf, ros::Time::now(), "base_link", "wheel_left_back_link"));

	return 0;
}

int broadcase_wheel_right_front()
{
	static tf::Transform_Broadcaster br;
	tf::TransFrom tf;

	tf.setOrigin(tf::vector3(5.0, -5.0, 0.0));
	tf.setRotation(tf::Quaternion(0, 0, 0, 1));
	br.sendTransform(tf::StampedTransform(tf, ros::Time::now(), "base_link", "wheel_right_front_link"));

	return 0;
}

int broadcase_wheel_right_back()
{
	static tf::Transform_Broadcaster br;
	tf::TransFrom tf;

	tf.setOrigin(tf::vector3(-5.0, -5.0, 0.0));
	tf.setRotation(tf::Quaternion(0, 0, 0, 1));
	br.sendTransform(tf::StampedTransform(tf, ros::Time::now(), "base_link", "wheel_right_back_link"));

	return 0;
}

int broadcase_wheel_link()
{
	broadcase_wheel_left_front();
	broadcase_wheel_left_back();
	broadcase_wheel_right_front();
	broadcase_wheel_right_back();
	return 0;
}

#endif

int         main(int argc, const char **argv)
{
    ros::init(argc, (char **)argv, "node_motion");

    motionPiinit();

    ros::NodeHandle n;
    ros::Subscriber sub_motion;	
    ros::Subscriber sub_type;
	ros::Rate r(100);

	sub_motion = n.subscribe("motionmess", 1000, motion_call_back);	
	sub_type = n.subscribe("clientmotiontmess", 1000, type_call_back);

	while (ros::ok())
	{
		ROS_INFO("node_motion ...\n");		
#ifdef USE_TF
		broadcase_wheel_link();
#endif
		ros::spinOnce();
		r.sleep();
	}
    return 0;
}
