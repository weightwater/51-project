#include <reg51.h>
#include "pca.h"
#include "key.h"
#include "screen.h"
#include "tools.h"

char tmodesign = 0;
sbit Xdata = P2^0;	// 串行数据输入
sbit oe = P2^1;			// 0时发送数据
sbit rck = P2^2;		// 数据锁存 
sbit sck = P2^3;		// 上升沿送数据
char mode = 0;
sbit clock0 = P2^4;
// int tones[7] = { 
xdata int time = 0, t_sign = -1;

int sign = 0;

xdata int tone = 0, timer = 0;

sfr ISP_DATA = 0xe2;
sfr ISP_ADDRH = 0xe3;
sfr ISP_ADDRL = 0xe4;
sfr ISP_CMD = 0xe5;
sfr ISP_TRIG = 0xe6;
sfr ISP_CONTR = 0xe7;

sbit LED1 = P0^1;
sbit LED2 = P0^2;
sbit KEY1 = P1^1;
sbit KEY2 = P1^2;

//void cc(unsigned int addr);
//void xcx(unsigned int addr, unsigned char dat);
//unsigned char dcx(unsigned int addr);
//void Q0(void);


void main(void)
{
	
//	unsigned char V;
//	V = dcx(0x2002);
//	if(V == 2)
//	{
//		LED1 = 0;
//		LED2 = 1;
//	}
//	else if(V == 6)
//	{
//		LED1 = 0;
//		LED2 = 0;
//	}
//	
//	while(1)
//	{
//		if(!K1);
//		{	
//			while(!K1);
//			LED1 = 0;LED2=1;
//			cc(0x2000);
//			xcx(0x2002, 2);
//		}
//		if(!K2)
//		{
//			while(!K2);
//			LED1 = 0; LED2 = 0;
//			cc(0x2000);
//			xcx(0x2002, 6);
//		}
//	}
//	
//	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	oe = 0;
	rck = 0;
	sck = 0;
	serial_Init();
	PCA_Init();
	int0_Init();
//	
//	ET0 = 1;
//	TMOD |= 0x01;
//	ET0 = 1;
//	TR0 = 1;
//	TH0 = 0xfb;
//	TL0 = 0xe8;
	
	SBUF = '1';
	while(TI == 0);
	TI = 0;
	while(1);
}


//void timer0(void) interrupt 1
//{
//		if(sign == 1)
//		{	
//			time++;
//			if(time == 100000)
//			{
//				time = 0;
//				tone++;
//				if(tone == 2)
//					tone = 0;
//			}
//			clock0 = ~clock0;
//			if(tone == 0)
//			{
//				TH0 = 0xff;
//				TL0 = 0xf6;
//			}
//			else if(tone == 1)
//			{
//				TH0 = 0xff;
//				TL0 = 0xec;
//			}
//			else if(tone == 2)
//			{
//				TH0 = 0xfd;
//				TL0 = 0x45;
//			}
//			else if(tone == 3)
//			{
//				TH0 = 0xfd;
//				TL0 = 0xd5;
//			}
//			else if(tone == 4)
//			{
//				TH0 = 0xfd;
//				TL0 = 0xd5;
//			}
//			else if(tone == 5)
//			{
//				TH0 = 0xfd;
//				TL0 = 0x91;
//			}
//			else if(tone == 6)
//			{
//				TH0 = 0xfd;
//				TL0 = 0x45;
//			}
//			else if(tone == 7)
//			{
//				TH0 = 0xfd;
//				TL0 = 0xd5;
//			}
//			else if(tone == 8)
//			{
//				TH0 = 0xfd;
//				TL0 = 0x45;
//			}
//			else if(tone == 9)
//			{
//				TH0 = 0xfc;
//				TL0 = 0xef;
//			}
//			else if(tone == 10)
//			{
//				TH0 = 0xfc;
//				TL0 = 0xc1;
//			}
//			else if(tone == 11)
//			{
//				TH0 = 0xfd;
//				TL0 = 0x45;
//			}
//			else if(tone == 12)
//			{
//				TH0 = 0xfc;
//				TL0 = 0xef;
//			}
//			else if(tone == 13)
//			{
//				TH0 = 0xfd;
//				TL0 = 0x45;
//			}
//			else if(tone == 14)
//			{
//				TH0 = 0xfc;
//				TL0 = 0xef;
//			}
//			else if(tone == 15)
//			{
//				TH0 = 0xfc;
//				TL0 = 0xc1;
//			}
//		}
//		else
//			clock0 = 1;
//}
//unsigned char dcx(unsigned int addr)
//{
//	unsigned char dat;
//	ISP_CONTR = 0x81;
//	ISP_CMD = 0x01;
//	ISP_ADDRL = addr;
//	ISP_ADDRH = addr >> 8;
//	EA = 0;
//	ISP_TRIG = 0x46;
//	ISP_TRIG = 0xB9;
//	_nop_();
//	dat = ISP_DATA;
//	Q0();
//	return dat;
//}


//void Q0(void)
//{
//	ISP_CONTR = 0;
//	ISP_CMD = 0;
//	ISP_TRIG = 0;
//}

//void cc(unsgined int addr)
//{
//	ISP_CONTR = 0x81;
//	ISP_CMD = 0x03;
//	ISP_ADDRL = addr;
//	ISP_ADDRH = addr>>8;
//	EA = 0;
//	ISP_TRIG = 0x46;
//	ISP_TRIG = 0xb9;
//	_nop_();
//	Q0();
//}
//	
//void xcx(unsigned int addr, unsigned char dat)
//{
//	ISP_CONTR = 0x81;
//	ISP_CMD = 0x02;
//	ISP_ADDRL = addr;
//	ISP_ADDRH = addr >> 8;
//	ISP_DATA = dat;
//	EA = 0;
//	ISP_TRIG = 0x46;
//	ISP_TRIG = 0xb9;
//	_nop();
//	Q0();
//}

