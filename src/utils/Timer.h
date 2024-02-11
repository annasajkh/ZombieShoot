#pragma once

#include "stdbool.h"

typedef void (*Timeout)(void* sender);

typedef struct Timer
{
	float wait_time;
	float time;
	bool is_paused;
	bool is_oneshot;

	bool is_on_the_heap;

	Timeout time_out;
} Timer;

void timer_init(Timer* timer, float wait_time, bool oneshot, bool is_on_the_heap, Timeout timeout_func);

void timer_start(Timer* timer);

void timer_update(Timer* timer, void* sender);

void timer_delete(Timer* timer);