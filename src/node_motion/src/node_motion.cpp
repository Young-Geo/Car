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
#include "node_motion/motion.h"

#define PINONE 1
#define PINTWO 4
#define PINTHREE 5
#define PINFOUR 6
#define PWMMIN 1
#define PWMMAX 500


void        motion_call(const node_motion::motion::ConstPtr & motionmsg)
{
    // if
    ROS_INFO("LF_FORWARD PWM = %d\n", motionmsg->lf_forward);
    ROS_INFO("RI_FORWARD PWM = %d\n", motionmsg->ri_forward);
    ROS_INFO("LF_BACK PWM = %d\n", motionmsg->lf_back);
    ROS_INFO("RI_BACK PWM = %d\n", motionmsg->ri_back);

    //
    softPwmWrite(PINONE, motionmsg->lf_forward); //1
    softPwmWrite(PINTWO, motionmsg->ri_forward); //4
    softPwmWrite(PINTHREE, motionmsg->lf_back); //5
    softPwmWrite(PINFOUR, motionmsg->ri_back); //6
    //
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

int         main(int argc, const char **argv)
{
    ros::init(argc, (char **)argv, "node_motion");

    motionPiinit();

    ros::NodeHandle n;
    ros::Subscriber sub = n.subscribe("motion", 1000, motion_call);


    ros::spin();
    return 0;
}
