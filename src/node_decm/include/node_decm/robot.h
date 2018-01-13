#ifndef __ROBOT_H__
#define __ROBOT_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "afx.h"
#include "xlist.h"
#include "model.h"

typedef struct _Robot_t {
	u8	left, right;
	float distance;//³¬Éù²¨Êý¾Ý
	xlist *data;
	model_t *model;
} robot_t;

robot_t * 	robot_malloc();

int		 	robot_init(robot_t *robot);

int			robot_loopOnce(robot_t *robot, void *arg);

int			robot_reinit(robot_t *robot);

int			robot_exit(robot_t *robot);


#endif
