#ifndef __MODEL_H__
#define __MODEL_H__

#include <sys/time.h>
#include "xlist.h"
#include "xlog.h"

#define HANDLE 	void
#define PARA 	void

typedef void * (*mode_init_function)(void *);
typedef int	   (*mode_argv_function)(void *, void *);
typedef int	   (*mode_loop_function)(void *);
typedef int    (*mode_exit_function)(void *);

///////////////////////////////////可以独立继续独立一层++每个模式的功能，例如每个模式的执行间隔

typedef struct _function_t {
	mode_init_function init;
	mode_argv_function argv;
	mode_loop_function loop;
	mode_exit_function exit;
	HANDLE *handle;
	long timestamp, millis;
} function_t;

function_t *				function_init(mode_init_function init, mode_argv_function argv, mode_loop_function loop, mode_exit_function exit);
int							function_call_init(function_t *function);
int							function_call_argv(function_t *function, PARA *arg);
int							function_call_loop(function_t *function);
int							function_call_exit(function_t *function);

////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////

enum {
	MODEL_RET_CONTINUE = -44,
	MODE_RET_FINISH = 44,
};

typedef struct _model_t {
	xlist *stack, *regis;
	int moderet, active;
	long timestamp;
} model_t;

model_t *			model_init();

int					model_register(model_t *model, char *name, mode_init_function init, mode_argv_function argv, mode_loop_function loop, mode_exit_function exit);

int					model_call_init(model_t *model, char *name, void *robot, void *arg);

int					model_call_loop(model_t *model);

int					model_exit(model_t *model);


#endif
