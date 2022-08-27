#include "reg51.h"
// #include "uart.h"
#include "pca.h"
#include "screen.h"
#include "key.h"
#include "tools.h"
#define FOSC 11059200L
#define T100Hz (FOSC / 12 / 800)

typedef unsigned char BYTE;
typedef unsigned int WORD;
unsigned char dig2bit[8] = { 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80 };
xdata int year=2019,month=12,day=24,hour = 14,min = 30,sec = 30, week = 2, ds = 0 ;
xdata int set_press = 0;

xdata int set_sec = 0, set_min = 6, set_hour = 10, set_day = 6, set_month = 10, set_year = 2019, set_choice = 0, set_sign = 0;
xdata int send_message[28];

xdata int times = 0;

extern char tmodesign;

extern int sign;

unsigned int cnt=800;
sbit clock0 = P2^4;

extern char clock_sign;
extern char clock_hour;
extern char clock_min;
extern char reset_timer;
extern char clock0_hour, clock0_min, clock0_sign, clock1_hour, clock1_min, clock1_sign;

xdata int index = 0;
xdata char message[5];

xdata int lunar_month = 11, lunar_day = 29, lunar_year = 36, lunar_month_sign = 0, lunar_year_sign = 0;

extern char clock_week, clock0_week, clock1_week;

xdata int hlj = 0;

/*Declare SFR associated with the PCA */
code monthdays[13]={0,31,28,31,30,31,30,31,31,30,31,30,31};
sfr CCON = 0xD8; //PCA control register
sbit CCF0 = CCON^0; //PCA module-0 interrupt flag
sbit CCF1 = CCON^1; //PCA module-1 interrupt flag
sbit CR = CCON^6; //PCA timer run control bit
sbit CF = CCON^7; //PCA timer overflow flag
sfr CMOD = 0xD9; //PCA mode register
sfr CL = 0xE9; //PCA base timer LOW
sfr CH = 0xF9; //PCA base timer HIGH
sfr CCAPM0 = 0xDA; //PCA module-0 mode register
sfr CCAP0L = 0xEA; //PCA module-0 capture register LOW
sfr CCAP0H = 0xFA; //PCA module-0 capture register HIGH
sfr CCAPM1 = 0xDB; //PCA module-1 mode register
sfr CCAP1L = 0xEB; //PCA module-1 capture register LOW
sfr CCAP1H = 0xFB; //PCA module-1 capture register HIGH
sfr PCAPWM0 = 0xf2;
sfr PCAPWM1 = 0xf3;
sbit PCA_LED = P0^0; //PCA test LED
//extern BYTE recvst;
WORD value;
// void disp(void);


void PCA_isr(void) interrupt 7 //using 1
{
	CCF0 = 0; //Clear interrupt flag
	CCAP0L = value;  // https://img-blog.csdn.net/20141220210609781?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGl4aWFuZ21pbmdoYXRl/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center
	CCAP0H = value >> 8; //Update compare value
	value += T100Hz;
	// disp();

	week = GetWeek(year, month, day);
	// DecRecvSt();
	//if (recvst)recvst--;
	timer_run();
	judge_mode();
	judge_clock();
	if(day == 25)
		lunar_day = 30;
	
	
	// show();
	
	screen_show(hlj);
	hlj++;
	if(hlj == 8)
		hlj = 7;
	
}


void serial(void) interrupt 4
{
	char tmp = 0;
	ES = 0;
	RI = 0;
	tmp = SBUF;
	message[index] = SBUF;
	
	index++;
	if(index == 5)
	{
		index = 0;
		check_message();
//		for(tmp = 0; tmp < 5; tmp++)
//		{
//			SBUF = message[tmp];
//			while(TI == 0);
//			TI = 0;
//		}
//		SBUF = '\n';
//		while(TI == 0);
//		TI = 0;
	}
	ES = 1;
}	


void PCA_Init(void)
{
	CCON = 0; //Initial PCA control register
				//PCA timer stop running
				//Clear CF flag
				//Clear all module interrupt flag
	CL = 0; //Reset PCA base timer
	CH = 0;
	CMOD = 0x00; //Set PCA timer clock source as Fosc/12
				//Disable PCA timer overflow interrupt
	value = T100Hz;
	CCAP0L = value;
	CCAP0H = value >> 8; //Initial PCA module-0
	value += T100Hz;
	CCAPM0 = 0x49; //PCA module-0 work in 16-bit timer mode
					//and enable PCA interrupt
	CR = 1; //PCA timer start run
	EA = 1;
	cnt = 0;
}


void timer_run(void)
{
	if(cnt % 8 == 0)
	{
		ds++;
		if(ds == 100)
		{	
			ds = 0;
		}
	}
	if (cnt-- == 0)
	{
		cnt = 800; //Count 100 times
		PCA_LED = !PCA_LED; //Flash once per second
		if(min == 0)
		{
			if(times < 2*hour)
			{
				clock0 = !clock0;
				times++;
			}
		}
		sec++;
		if (sec>=60)
		{
			sec -=60;
			min++;
			times = 0;
			if (min>=60)
			{
				min-=60;
				hour++;
				if (hour>=24)
				{
					day++;
					week++;
					hour -= 24;
					if (week>7) week -=7;
					if (month!=2)
					{
						if (day>monthdays[month])
						{
							day-=monthdays[month];
							month++;
							if (month>12)
							{
								month-=12;
								year++;
							}

						}
					}
					else
					{
						if ((year%4)==0)
						{
							if (day>29)
							{
								day-=29;
								month++;
							}
						}
						else
						{
							if (day>28)
							{
								day-=28;
								month++;
							}
						}
					}
					
				/*---------------------------------------------------------------------------*/
					lunar_day++;
					lunar_month_sign = get_lunar_month(lunar_month, year);
					if(lunar_day == 31 && lunar_month_sign == 1)
					{
						lunar_month++;
						lunar_day = 1;
						lunar_year_sign = get_lunar_year(lunar_year);
						if(lunar_month == 13 && lunar_year_sign == 0)
						{
							lunar_year++;
							if(lunar_year == 61)
								lunar_year = 1;
							lunar_month = 1;
						}
						if(lunar_year = 14 && lunar_year_sign == 1)
						{
							lunar_year++;
							if(lunar_year == 61)
								lunar_year = 1;
							lunar_month = 1;
						}
						
					}
					else if(lunar_day == 30 && lunar_month_sign == 0)
					{	
						lunar_month++;
						lunar_day = 1;
						lunar_year_sign = get_lunar_year(lunar_year);
						if(lunar_month == 13 && lunar_year_sign == 0)
						{
							lunar_year++;
							if(lunar_year == 61)
								lunar_year = 1;
							lunar_month = 1;
						}
						if(lunar_year_sign = 1 && lunar_year == 14)
						{
							lunar_year++;
							if(lunar_year == 61)
								lunar_year = 1;
							lunar_month = 1;
						}
					}
					else if(lunar_month_sign == 1)
					{
						lunar_day = 1;
					}
					else if(lunar_month_sign == 0)
					{
						lunar_day = 0;
					}
				/*---------------------------------------------------------------------------*/	
				}
			}
		} 
	}
}


void judge_mode(void)
{
		if( mode == 0 && tmodesign == 0)
			set_screen_day(sec, min, hour);
		else if(mode == 0 && tmodesign == 1)
			set_screen_year(day, month, year);
		else if(mode == 0 && tmodesign == 2)
			set_screen_clock(week);
		else if(mode == 0 && tmodesign == 3)
			set_screen_lunar(lunar_day, lunar_month, lunar_year);
		else if(mode == 1)
			set_screen_timer(ds, cnt);
		else if(mode == 2)
			setup_screen_clock(week);
		else if(mode == 3)
			set_screen_set();
}


void show(void)
{	
	int i = 0;
	for(i = 0; i < 8; i++)
	{
		screen_show(i);
	}
}


void judge_clock(void)
{
	if((clock_sign == 1 && clock_week == week) || (clock_sign == 1 && clock_week == 0))
	{
		if(clock_hour == hour)
		{
			if(clock_min == min)
			{
					if(cnt == 0)
						clock0 = !clock0;
//					sign = 1;
			}
		}
	}
	if(((clock_sign == 1 && clock_week == week) || (clock_sign == 1 && clock_week == 0)))  
	{
		if(clock0_hour == hour)
		{
			if(clock0_min == min)
				if(cnt == 0)
					clock0 = !clock0;
		}
	}
	if(((clock_sign == 1 && clock_week == week) || (clock_sign == 1 && clock_week == 0)))
	{
		if(clock1_hour == hour)
		{
			if(clock1_min == min)
				if(cnt == 0)
					clock0 = !clock0;
		}
	}
	
}


void set_screen_set()
{
	if(set_press == 1 && P1 == PLUS)
	{
		if(set_choice == 0)
		{
			set_sec++;
			if(set_sec == 60)
				set_sec = 0;
		}
		if(set_choice == 1)
		{
			set_min++;
			if(set_min == 60)
				set_min = 0;
		}
		else if(set_choice == 2)
		{
			set_hour++;
			if(set_hour == 24)
				set_hour = 0;
		}
		else if(set_choice == 3)
		{
			set_day++;
			if(set_day == monthdays[set_month])
				set_day = 1;
		}
		else if(set_choice == 4)
		{
			set_month++;
			if(set_month == 13)
				set_month = 1;
		}
		else if(set_choice == 5)
		{
			set_year++;
		}
		set_press = 0;
	}
	else if(set_press == 1 && P1 == MINUS)
	{
		if(set_choice == 0)
		{
			set_sec--;
			if(set_sec == -1)
				set_sec = 59;
		}
		if(set_choice == 1)
		{
			set_min--;
			if(set_min == -1)
				set_min = 59;
		}
		else if(set_choice == 2)
		{
			set_hour--;
			if(set_hour == -1)
				set_hour = 23;
		}
		else if(set_choice == 3)
		{
			set_day--;
			if(set_day == 0)
				set_day = monthdays[set_month];
		}
		else if(set_choice == 4)
		{
			set_month--;
			if(set_month == 0)
				set_month = 12;
		}
		else if(set_choice == 5)
		{
			set_year--;
		}
		set_press = 0;
	}
	else if(P1 == NEXT_POS && set_press == 1)
	{
		set_choice++;
		if(set_choice == 6)
			set_choice = 0;
		set_press = 0;
	}
	else if(P1 == OK && set_press == 1)
	{
		set_sign++;
		if(set_sign == 2)
			set_sign = 0;
		set_press = 0;
	}
	if(P1 == 0xff)
		set_press = 1;
	if(set_sign == 1)
	{
		year = set_year;
		month = set_month;
		day = set_day;
		hour = set_hour;
		min = set_min;
		sec = set_sec;
	}
	set_change_screen(set_choice, set_year, set_month, set_day, set_hour, set_min, set_sec);
}


void check_message(void)
{
	int i = 0;
	if(message[2] == 'h')
	{
		hour = (message[3]-'0')*10 + (message[4] - '0');
	}
	else if(message[2] == 'm')
	{
		min = (message[3]-'0')*10 + (message[4] - '0');
	}
	else if(message[2] == 'M')
	{
		month = (message[3]-'0')*10 + (message[4] - '0');
	}
	else if(message[2] == 'y')
	{
		year = (message[3]-'0')*1000 + (message[4] - '0')*100 + (message[0]-'0')*10 + (message[1] - '0');
	}
	else if(message[2] == 'd')
	{
		day = (message[3]-'0')*10 + (message[4] - '0');
	}
	else if(message[2] == 'c')
	{
		clock_sign = 1;
		clock_hour = (message[3]-'0')*10 + (message[4] - '0');
		clock_min = (message[0]-'0')*10 + (message[1] - '0');
		clock_week++;
		if(clock_week == 8)
			clock_week = 0;
	}
	else if(message[2] == 'o')
	{
		clock_sign = 0;
		clock_hour = (message[3]-'0')*10 + (message[4] - '0');
		clock_min = (message[0]-'0')*10 + (message[1] - '0');
		clock_week++;
		if(clock_week == 8)
			clock_week = 0;
	}
	else if(message[2] == 'r')
	{
		send_message[0] = year/1000 + '0';
		send_message[1] = (year/100)%10 + '0';
		send_message[2] = (year/10)%10 + '0';
		send_message[3] = year%10 + '0';
		send_message[4] = '.';
		send_message[5] = month/10 + '0';
		send_message[6] = month%10 + '0';
		send_message[7] = '.';
		send_message[8] = day/10 + '0';
		send_message[9] = day%10 + '0';
		send_message[10] = '.';
		send_message[11] = hour/10 + '0';
		send_message[12] = hour%10 + '0';
		send_message[13] = '.';
		send_message[14] = min/10 + '0';
		send_message[15] = min%10 + '0';
		send_message[16] = '.';
		send_message[17] = clock_hour/10 + '0';
		send_message[18] = clock_hour%10 + '0';
		send_message[19] = '.';
		send_message[20] = clock_min/10 + '0';
		send_message[21] = clock_min%10 + '0';
		send_message[22] = '.';
		send_message[23] = clock_sign + '0';
		send_message[24] = '.';
		send_message[25] = week + '0';
		send_message[26] = ' ';
		send_message[27] = '\n';
		for(i = 0; i < 28; i++)
		{
			SBUF = send_message[i];
			while(TI == 0);
			TI = 0;
		}
	}
}

// rrrrr: 读取
// 读取前： 正常
// 读取后： 形式如 0h120 设置小时为12















