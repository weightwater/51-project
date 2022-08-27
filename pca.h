#ifndef _PCA_H_
#define _PCA_H_

void PCA_isr(void);
void PCA_Init(void);
void timer_run(void);
void judge_mode(void);
void show(void);
void judge_clock(void);
void set_screen_set(void);
void set_time(void);
void check_message(void);

extern char mode;


#endif
