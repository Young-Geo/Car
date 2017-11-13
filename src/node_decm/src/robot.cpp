#include "node_decm/robot.h"

robot_t * 	robot_malloc()
{
	robot_t *robot = NULL;
	robot = (robot_t *)xmalloc(sizeof(robot_t));
	XXNULL(robot, NULL);

	
	robot->distance = 0.0;
	robot->left = robot->right = 0;

	
	return robot;
}

int		 	robot_init(robot_t *robot)
{
	XXNULL(robot, -1);

	robot->distance = 0.0;
	robot->left = robot->right = 0;

	return 0;
}

int			robot_loop(robot_t *robot, void *arg)
{
	XXNULL(robot, -1);
	XXNULL(arg, -1);
	
	return 0;
}

int			robot_reinit(robot_t *robot)
{
	XXNULL(robot, -1);
	
	return 0;
}

int			robot_exit(robot_t *robot)
{	
	XXNULL(robot, -1);
	return 0;
}



































