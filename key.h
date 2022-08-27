#ifndef _KEY_H_
#define _KEY_H_

#define CHANGE_MODE 0xfe
#define TIMER_US		0xfd
#define CLOCK				0xfb
#define SET					0xf7
#define NEXT_POS		0xef
#define OK					0xdf
#define PLUS				0xbf
#define MINUS				0x7f

sbit change_mode = P1^0;
sbit timer_ms = P1^1;
sbit clock = P1^2;
sbit set = P1^3;
sbit next_pos = P1^4;
sbit ok = P1^5;
sbit plus = P1^6;
sbit minus = P1^7;

extern char tmodesign;
extern char mode;

void time_mode_change(void);
void timer_fuction(void);
void clock_fuction(void);
void set_fuction(void);

#endif
