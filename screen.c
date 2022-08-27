#include "screen.h"
#include "key.h"
#include "pca.h"

sbit Xdata = P2^0;	// 串行数据输入
sbit oe = P2^1;			// 0时发送数据
sbit rck = P2^2;		// 数据锁存 
sbit sck = P2^3;		// 上升沿送数据

char reset_timer = 1;

char clock_hour = 0;
char clock_sign = 0;
char clock_min = 1;
char clock_week = 0;
char seg_index = 0;
char screen[8];
char count_min = 0;
char count_sec = 0;
char count_ds = 0;
char stop = 1;
char cnt0 = 800;
char clock_choice = 0;
char judge_press = 0;
char clock0_hour = 0, clock0_min = 0, clock0_sign = 0, clock0_week = 1;
char clock1_hour = 0, clock1_min = 0, clock1_sign = 0, clock1_week = 1;

unsigned char num[]={0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f, 0x6f};
char w_num[] = {0xfe,0xfd,0xfb,0xf7,0xef,0xdf,0xbf,0x7f};
char test[8] = {  0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x09 };

extern int sign;

/*
	0x01  上横
	0x02  右上竖
	0x04	右下竖
	0x08  下横
	0x10  左下竖
	0x20  左上竖
	0x40  中横
	0x80  点
*/


void screen_show(int segment)
{
	unsigned int seg = w_num[segment], dat, i = 0;
	if(seg_index == 8)
		seg_index = 0;
	seg = seg & 0x00ff;
	dat = (seg << 8) | screen[seg_index];
	for(i = 0; i < 16; i++)
	{
		Xdata = (0x8000 & dat)? 1:0;
		sck = 1;
		sck = 0;
		dat <<= 1;
	}
	rck = 1;
	rck = 0;
	seg_index++;
}


void set_screen_day(int sec, int min, int hour)
{
	screen[0] = num[sec%10];
	screen[1] = num[sec/10];
	screen[2] = 0x09;
	screen[3] = num[min%10];
	screen[4] = num[min/10];
	screen[5] = 0x09;
	screen[6] = num[hour%10];
	screen[7] = num[hour/10];
}
void set_screen_year(int day, int month, int year)
{
	screen[0] = num[day%10];
	screen[1] = num[day/10];
	screen[2] = num[month%10];
	screen[3] = num[month/10];
	screen[4] = num[year%10];
	screen[5] = num[(year/10)%10];
	screen[6] = num[(year/100)%10];
	screen[7] = num[year/1000];
}


void set_screen_timer(int ds, int cnt1)
{
	if(stop == 0 && reset_timer == 0)
	{
		if(cnt1 == 0)
		{
			count_sec++;
			if(count_sec == 60)
			{
				count_sec -=60;
				count_min++;
				if(count_min == 60)
					count_min -= 60;
			}
		}
		count_ds = ds;
		screen[0] = num[ds%10];
		screen[1] = num[ds/10];
		screen[2] = 0x09;
		screen[3] = num[count_sec%10];
		screen[4] = num[count_sec/10];
		screen[5] = 0x09;
		screen[6] = num[count_min%10];
		screen[7] = num[count_min/10];
		if(P1 == MINUS)
		{
			stop = 1;
			P0 = 0xfe;
		}
	}
	else if(stop == 1 && reset_timer == 0)
	{
		screen[0] = num[count_ds%10];
		screen[1] = num[count_ds/10];
		screen[2] = 0x09;
		screen[3] = num[count_sec%10];
		screen[4] = num[count_sec/10];
		screen[5] = 0x09;
		screen[6] = num[count_min%10];
		screen[7] = num[count_min/10];
		if(P1 == PLUS)
		{
			stop = 0;
			P0 = 0xfd;
		}
	}
	else
	{
		reset_screen();
		stop = 1;
		ds = 0;
		count_sec = 0;
		count_min = 0;
		if(P1 == PLUS)
		{
			reset_timer = 0;
			stop = 0;
		}
	}
}


void reset_screen(void)
{
	int i = 0;
	for(i = 0; i < 8; i++)
		screen[i] = num[0];
	screen[2] = 0x09;
	screen[5] = 0x09;
}


void set_screen_clock(int week)
{
	if(clock_choice < 3)
	{
		screen[0] = num[week];
		screen[1] = num[clock_week];
		screen[2] = 0x09;
		screen[3] = num[clock_min%10];
		screen[4] = num[clock_min/10];
		screen[5] = 0x09;
		screen[6] = num[clock_hour%10];
		screen[7] = num[clock_hour/10];
	}
	else if(clock_choice < 6)
	{
		screen[0] = num[week];
		screen[1] = num[clock0_week];
		screen[2] = 0x09;
		screen[3] = num[clock0_min%10];
		screen[4] = num[clock0_min/10];
		screen[5] = 0x09;
		screen[6] = num[clock0_hour%10];
		screen[7] = num[clock0_hour/10];
	}
	else
	{
		screen[0] = num[week];
		screen[1] = num[clock1_week];
		screen[2] = 0x09;
		screen[3] = num[clock1_min%10];
		screen[4] = num[clock1_min/10];
		screen[5] = 0x09;
		screen[6] = num[clock1_hour%10];
		screen[7] = num[clock1_hour/10];
	}
}


void setup_screen_clock(int week)
{
	if(P1 == PLUS && judge_press == 1)
	{
		if(clock_choice == 0)
		{
			clock_week++;
			if(clock_week == 8)
				clock_week = 0;
		}
		if(clock_choice == 1)
		{
			clock_min++;
			if(clock_min == 60)
				clock_min = 0;
		}
		if(clock_choice == 2)
		{
			clock_hour++;
			if(clock_hour == 24)
				clock_hour = 0;
		}
		if(clock_choice == 3)
		{
			clock0_week++;
			if(clock0_week == 8)
				clock0_week = 0;
		}
		if(clock_choice == 4)
		{
			clock0_min++;
			if(clock0_min == 60)
				clock0_min = 0;
		}
		if(clock_choice == 5)
		{
			clock0_hour++;
			if(clock0_hour == 24)
				clock0_hour = 0;
		}
		if(clock_choice == 6)
		{
			clock1_week++;
			if(clock1_week == 8)
				clock1_week = 0;
		}
		if(clock_choice == 7)
		{
			clock1_min++;
			if(clock1_min == 60)
				clock1_min = 0;
		}
		if(clock_choice == 8)
		{
			clock1_hour++;
			if(clock1_hour == 24)
				clock1_hour = 0;
		}
		judge_press = 0;
	}
	if(P1 == MINUS && judge_press == 1)
	{
		if(clock_choice == 0)
		{
			clock_week--;
			if(clock_week== -1)
				clock_week = 7;
		}
		else if(clock_choice == 1)
		{
			clock_min--;
			if(clock_min == -1)
				clock_min = 59;
		}
		else if(clock_choice == 2)
		{
			clock_hour--;
			if(clock_hour == -1)
				clock_hour = 23;
		}
		else if(clock_choice == 3)
		{
			clock0_week--;
			if(clock0_week == -1)
				clock0_week = 7;
		}
		else if(clock_choice == 4)
		{
			clock0_min--;
			if(clock0_min == -1)
				clock0_min = 59;
		}
		else if(clock_choice == 5)
		{
			clock0_hour--;
			if(clock0_hour == -1)
				clock0_hour = 23;
		}
		else if(clock_choice == 6)
		{
			clock1_week--;
			if(clock1_week == -1)
				clock1_week = 7;
		}
		else if(clock_choice == 7)
		{
			clock1_min--;
			if(clock1_min == -1)
				clock1_min = 59;
		}
		else if(clock_choice == 8)
		{
			clock1_hour--;
			if(clock1_hour == -1)
				clock1_hour = 23;
		}
		judge_press = 0;
	}
	if(P1 == OK && judge_press == 1)
	{
		if(clock_choice < 3)
		{
			clock_sign++;
			if(clock_sign == 2)
				clock_sign = 0;
		}
		else if(clock_choice < 6)
		{
			clock0_sign++;
			if(clock0_sign == 2)
				clock0_sign = 0;
		}
		else 
		{
			clock1_sign++;
			if(clock1_sign == 2)
				clock1_sign = 0;
		}
		judge_press = 0;
	}
	if(P1 == NEXT_POS && judge_press == 1)
	{
		clock_choice++;
		if(clock_choice == 9)
			clock_choice = 0;
		judge_press = 0;
	}
	if(P1 == 0xff)
	{
		judge_press = 1;
	}
	set_screen_clock(week);
}


void set_change_screen(int choice, int year, int month, int day, int hour, int min, int sec)
{
	if(choice < 3)
		set_screen_day(sec, min, hour);
	else
		set_screen_year(day, month, year);
}



void set_screen_lunar(int day, int month, int year)
{
	screen[0] = num[day%10];
	screen[1] = num[day/10];
	screen[2] = 0x09;
	screen[3] = num[month%10];
	screen[4] = num[month/10];
	screen[5] = 0x09;
	screen[6] = num[year%10];
	screen[7] = num[year/10];
}







