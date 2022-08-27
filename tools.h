#ifndef 		_TOOLS_H_
#define			_TOOLS_H_

#include "screen.h"

extern unsigned char dig2bit[8];

void int0_Init(void);
void timer_fuction(void);
void report_punctually(void);
void serial_Init(void);
extern char mode;
int get_lunar_year(int year);
int get_lunar_month(int month, int year);
void sing(int cnt, int tone);
int GetWeek(int year,int month,int day);

#endif
