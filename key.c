#include <reg51.h>
#include "key.h"
#include "tools.h"

extern char reset_timer;

void int0(void) interrupt 0
{
		if(P1 == CHANGE_MODE)
		{
			time_mode_change();
		}
		else if(P1 == TIMER_US)
		{
			timer_fuction();
		}
		else if(P1 == CLOCK)
		{
			clock_fuction();
		}
		else if(P1 == SET)
		{
			set_fuction();
		}
}


void time_mode_change(void)
{
	P0 = CHANGE_MODE;
	tmodesign++;
	if(tmodesign == 4)
		tmodesign = 0;
	mode = 0;
}


void timer_fuction(void)
{
	P0 = TIMER_US;
	mode = 1;
	reset_timer = 1;
}


void clock_fuction(void)
{
	P0 = CLOCK;
	mode = 2;
}


void set_fuction(void)
{
	P0 = SET;
	mode = 3;
}

