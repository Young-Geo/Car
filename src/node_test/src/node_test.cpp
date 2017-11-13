#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <time.h>
#include <signal.h>

#include <wiringPi.h>
#include <softPwm.h>

#include "ros/ros.h"

#define PINONE 1
#define PINTWO 4
#define PINTHREE 5
#define PINFOUR 6
#define PWMMIN 1
#define PWMMAX 500
#define ZERO 0

void        testPiinit()
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

int			siginit(int signum, void (*call)(int, siginfo_t *, void *))
{
	struct sigaction act, old;

	if (signum < 1 || signum > 64) {
		ROS_DEBUG("%s\n", "signum error");
		return -1;
	}
	if (!call) {
		ROS_DEBUG("%s\n", "call NULL");
		return -1;
	}
	
	sigemptyset(&act.sa_mask); 
	act.sa_flags = SA_SIGINFO;
 	act.sa_sigaction = call;
 	sigaction(signum, &act, &old);
	return 0;
}

void		SIGINT_BACK(int signum, siginfo_t *siginfo, void *arg)
{
	
	printf("SIGINT_BACK signum %d\n", signum);
	softPwmWrite(PINONE, ZERO);
	softPwmWrite(PINTWO, ZERO); //4
    softPwmWrite(PINTHREE, ZERO);
    softPwmWrite(PINFOUR, ZERO);
	exit(0);
}


int         main(int argc, const char **argv)
{
    ros::init(argc, (char **)argv, "node_test");

    testPiinit();
	siginit(SIGINT, SIGINT_BACK);

    ros::NodeHandle n;
	ros::Rate r(10);

	while (ros::ok())
	{
		softPwmWrite(PINONE, 1);
		ROS_INFO("sleep\n");
		//softPwmWrite(PINTWO, PWMMAX); //4
    	//softPwmWrite(PINTHREE, PWMMAX);
    	//softPwmWrite(PINFOUR, PWMMAX);
		r.sleep();
		//usleep(500);
	}

	printf("exit");

	softPwmWrite(PINONE, ZERO);
	softPwmWrite(PINTWO, ZERO); //4
    softPwmWrite(PINTHREE, ZERO);
    softPwmWrite(PINFOUR, ZERO);
	
	return 0;
}

