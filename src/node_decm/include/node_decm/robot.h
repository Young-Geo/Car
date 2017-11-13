#ifndef __ROBOT_H__
#define __ROBOT_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "afx.h"
#include "xlist.h"

typedef struct _Robot_t {
	u8	left, right;
	float distance;
} robot_t;

robot_t * 	robot_malloc();

int		 	robot_init(robot_t *robot);

int			robot_loop(robot_t *robot, void *arg);

int			robot_reinit(robot_t *robot);

int			robot_exit(robot_t *robot);


#endif
