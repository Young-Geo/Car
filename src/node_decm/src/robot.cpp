#include "node_decm/robot.h"

robot_t * 	robot_malloc()
{
	robot_t *robot = NULL;
	xassert((robot = (robot_t *)xcalloc(1, sizeof(robot_t))));
	xassert((robot->data = xlist_init()));
	xassert(robot->model = model_init());

	//////
	///应该注册所有工作
	//////
	
	return robot;
}

int		 	robot_init(robot_t *robot)
{
	xassert(robot);
	xzero(robot, sizeof(robot_t));
	xassert((robot->data = xlist_init()));
	xassert(robot->model = model_init());
	
	return 0;
}

int			robot_loopOnce(robot_t *robot, void *data)
{
	xassert(robot);
	
	return 0;
}

int			robot_reinit(robot_t *robot)
{	
	xassert(robot);
	xassert(robot->data);
	xassert(robot->model);

	xlist_clean(&robot->data);
	model_exit(robot->model);

	xassert((robot->data = xlist_init()));
	xassert(robot->model = model_init());
	return 0;
}

int			robot_exit(robot_t *robot)
{	
	xassert(robot);
	xassert(robot->data);
	xassert(robot->model);

	xlist_clean(&robot->data);
	model_exit(robot->model);

	xfree(robot);
	
	return 0;
}



































