#include "node_decm/model.h"



function_t *				function_init(mode_init_function init, mode_argv_function argv, mode_loop_function loop, mode_exit_function exit)
{
	function_t *function = NULL;

	xassert(init);
	//xassert(argv);
	xassert(loop);
	xassert(exit);

	xassert(function = (function_t *)xmalloc(sizeof(function_t)));
	xzero(function, sizeof(function_t));

	function->init = init;
	function->argv = argv;	
	function->loop = loop;	
	function->exit = exit;
	
	return function;
}

int							function_call_init(function_t *function, void *robot, long millis)
{
	struct timeval tv;
	
	xassert(function);
	xassert(function->init);
	xassert(robot);
	
	xassert(function->handle = function->init(robot));

	function->millis = millis;
	
    gettimeofday(&tv, NULL);
	function->timestamp = tv.tv_sec*1000 + tv.tv_usec/1000;
	
	return 0;
}


int							function_call_argv(function_t *function, PARA *arg)
{
	xassert(function);
	xassert(function->argv);
	xassert(arg);
	
	function->argv(function, arg);
	
	return 0;
}
int							function_call_loop(function_t *function)
{
	struct timeval tv;
	int ret = 0;
	
	xassert(function);
	xassert(function->handle);
	xassert(function->loop);

	gettimeofday(&tv, NULL);
	
	if (function->millis > 0) {
		gettimeofday(&tv, NULL);
		if ((tv.tv_sec*1000 + tv.tv_usec/1000) - function->timestamp < function->millis) {
			return -1;
		}
		function->timestamp = tv.tv_sec*1000 + tv.tv_usec/1000;
	}
	
	return function->loop(function->handle);;
}
int							function_call_exit(function_t *function)
{
	xassert(function);
	xassert(function->handle);
	xassert(function->exit);

	function->exit(function->handle);
	function->handle = NULL;
	
	return 0;
}



////////////////////////////////////////////////////////
////////////////////////////////////////////////////////
////////////////////////////////////////////////////////
////////////////////////////////////////////////////////
////////////////////////////////////////////////////////

model_t *			model_init()
{
	model_t *model = NULL;

	xassert(model = (model_t *)xmalloc(sizeof(model_t)));

	xzero(model, sizeof(model_t));

	xassert(model->regis = xlist_init());
	xassert(model->stack = xlist_init());

	return model;
}

int					model_register(model_t *model, char *name, mode_init_function init, mode_argv_function argv, mode_loop_function loop, mode_exit_function exit)
{
	function_t *function = NULL;
	xlist *node = NULL;
	
	xassert(model);
	xassert(name && name[0]);
	xassert(model->stack);
	xassert(model->regis);
	xassert(init);
	//xassert(argv);
	xassert(loop);
	xassert(exit);
	
	xassert((function = function_init(init, argv, loop, exit)));

	if ((node = xlist_get(model->regis, name))) {
		xfree(node->value);
		node->value = (char *)function;
		//
	} else {
		xlist_add(model->regis, name, XLIST_STRING, (char *)function);
	}
	
	return 0;
}

int					model_call_init(model_t *model, char *name, void *robot, void *arg, long millis)
{
	function_t *function = NULL;
	
	xassert(model);
	xassert(model->regis);	
	xassert(model->stack);
	xassert(name);

	if (!(function = (function_t *)xlist_getv(model->regis, name))) {
		xdebug("model_call not find %s\n", name);
		return -1;
	}

	function_call_init(function, robot, millis);

	if (arg && function->argv) {
		//function->argv(function, arg);
		function_call_argv(function, arg);
	}

	xlist_push(model->stack, NULL, XLIST_STRING, (char *)function);
	return 0;
}

int					model_call_loop(model_t *model)
{
	function_t *function = NULL;
	xlist *node = NULL;
	
	xassert(model);
	xassert(model->stack);

	while (xlist_notempty(model->stack))
	{
		if ((function = (function_t *)xlist_firstv(model->stack)) && function->handle) {
			break;
		}
		node = xlist_pop(model->stack);
		xfree(node);
	}

	if (xlist_isempty(model->stack)) {
		xdebug("model stack empty\n");
		return -1;
	}

	model->moderet = function_call_loop(function);
	
	if (MODE_RET_FINISH == model->moderet) {
		function_call_exit(function);
	}

	return 0;
}


int					model_exit(model_t *model)
{
	xassert(model);
	xassert(model->stack);
	xassert(model->regis);

	xlist_clean(&model->stack);
	xlist_clean(&model->regis);

	xfree(model);
	
	return 0;
}

