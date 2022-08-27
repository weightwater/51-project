#ifndef _SCREEN_H_
#define _SCREEN_H_

#include <reg51.h>


void screen_show(int segment);
void set_screen_day(int sec, int min, int hour);
void set_screen_year(int day, int month, int year);
void set_screen_lunar(int day, int month, int year);
void set_screen_timer(int ds, int cnt1);
void set_screen_clock(int week);
void reset_screen(void);
void setup_screen_clock(int week);
void set_change_screen(int choice, int year, int month, int day, int hour, int min, int sec);


#endif
