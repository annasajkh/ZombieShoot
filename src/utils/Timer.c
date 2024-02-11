#include "Timer.h"
#include "stdlib.h"
#include "raylib.h"


void timer_init(Timer* timer, float wait_time, bool is_oneshot, bool is_on_the_heap, Timeout timeout_func)
{
	if (timer == NULL)
	{
		printf("Timer should already be allocated either on the stack or heap!\n");
		getchar();
		exit(1);
	}

	timer->time = 0;
	timer->is_paused = false;
	timer->is_oneshot = is_oneshot;
	timer->wait_time = wait_time;
	timer->is_on_the_heap = is_on_the_heap;

	timer->time_out = timeout_func;
}


void timer_start(Timer* timer)
{
	if (timer->is_paused)
	{
		timer->is_paused = false;
	}
}

void timer_update(Timer* timer, void* sender)
{
	if (!timer->is_paused)
	{
		timer->time += GetFrameTime();

		if (timer->time >= timer->wait_time)
		{
			timer->time_out(sender);
			timer->time = 0;

			if (timer->is_oneshot)
			{
				timer->is_paused = true;
			}
		}
	}
}

void timer_delete(Timer* timer)
{
	if (timer->is_on_the_heap)
	{
		free(timer);
	}
}