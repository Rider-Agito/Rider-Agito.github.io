#include<reg52.h>
#include "intrins.h"
#define uchar unsigned char
#define uint  unsigned int
#define MotorData P0                    //接口定义
uchar phasecw[4] ={0x08,0x04,0x02,0x01};//正转 D-C-B-A
uchar phaseccw[4]={0x01,0x02,0x04,0x08};//反转 A-B-C-D
uint p;
uint speed;//类型不确定
uchar s;
uchar jzh;

//ms延时函数
void Delay_xms(uint x)
{
 uint i,j;
 for(i=0;i<x;i++)
  for(j=0;j<112;j++);
}
//顺时针转动
void MotorCW(void)
{
 uchar i;
 for(i=0;i<4;i++)
  {
   MotorData=phasecw[i];
   Delay_xms(speed);//转速调节
  }
}
//逆时针转动
void MotorCCW(void)
{
 uchar i;
 for(i=0;i<4;i++)
  {
   MotorData=phaseccw[i];
   Delay_xms(speed);//转速调节
  }
}
//停止转动
void MotorStop(void)
{
 MotorData=0x00;
}
//启动
void Setup(void)
{	
	speed=960/p;
	switch(s)
			{
				case 0:
					switch(p)
					{
						case 240:
						{
								for(p=180;p<238;p++)
							{
								speed=960/p;
								MotorCW();
							}
						}
						default:MotorCW();
					}
				break;
				case 1:
					switch(p)
					{
						case 240:
						{
								for(p=180;p<238;p++)
							{
								speed=960/p;
								MotorCCW();
							}
						}
						default:MotorCCW();
					}
				break;
				default: break;
			}
}
sbit IN0_4=P0^4;
sbit IN0_5=P0^5;
sbit IN0_6=P0^6;
sbit IN0_7=P0^7;
//数码管
uchar Nixietable[]={0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x90,0xc6,0xf0,0x00};
 
void Nixie(unsigned char Location,Number)//Location位选,Number段选
{
	switch(Location)
	{
		case 1:IN0_4=1;IN0_5=0;IN0_6=0;IN0_7=0;break;//LED1
		case 2:IN0_4=0;IN0_5=1;IN0_6=0;IN0_7=0;break;//LED2
		case 3:IN0_4=0;IN0_5=0;IN0_6=1;IN0_7=0;break;//LED3
		case 4:IN0_4=0;IN0_5=0;IN0_6=0;IN0_7=1;break;//LED4
	}
	P2=Nixietable[Number];
	Delay_xms(1);//延时1ms
	P2=0xff;//消音，P2口清零
}

//键盘
uchar keysearch()
{uchar k;
	P1=0xf0;
	k=P1;
	k=~k;
	k=k&0xf0;
	return k;
}

uchar keys()
{uchar a,c,kr,keynumb;
	a=keysearch();
	if(a==0)
		return 0xff;
	else
		Delay_xms(10);
	a=keysearch();
	if(a==0)
		return 0xff;
	else
		{a=0xfe;
			for(kr=0;kr<4;kr++)
			{P1=a;
				c=P1;
				if((c&0x10)==0)keynumb=kr+0x00;
				if((c&0x20)==0)keynumb=kr+0x04;
				if((c&0x40)==0)keynumb=kr+0x08;
				if((c&0x80)==0)keynumb=kr+0x0c;
				a=_crol_(a,1);
			}
		}
		do{
			a=keysearch();
		  }while(a!=0);
		return keynumb;
}

void keybranch(uchar k)
{switch(k)
	{case 0x00 ://1
			{
				p=30;
				while(k==0x00||k==0xff)
				{
					Nixie(2,0);
					Nixie(3,3);
					Nixie(4,0);
					jzh=keys();
					keybranch(jzh);
				}
			}
		break;
		case 0x04 ://2
			{
				p=60;
				while(k==0x04||k==0xff)
				{
					Nixie(2,0);
					Nixie(3,6);
					Nixie(4,0);
					jzh=keys();
					keybranch(jzh);
				}
			}
		break;
		case 0x08 ://3
			{
				p=90;
				while(k==0x08||k==0xff)
				{
					Nixie(2,0);
					Nixie(3,9);
					Nixie(4,0);
					jzh=keys();
					keybranch(jzh);
				}
			}
		break;
		case 0x0c ://启动
			while(k==0x0c||k==0xff)
				{				
					Setup();			
					jzh=keys();
					keybranch(jzh);
				}		
		break;
		case 0x01 ://4
			{
				p=120;
				while(k==0x01||k==0xff)
				{
					Nixie(2,1);
					Nixie(3,2);
					Nixie(4,0);
					jzh=keys();
					keybranch(jzh);
				}
			}
		break;
		case 0x05 ://5
			{
				p=150;
				while(k==0x05||k==0xff)
				{
					Nixie(2,1);
					Nixie(3,5);
					Nixie(4,0);
					jzh=keys();
					keybranch(jzh);
				}
			}
		break;
		case 0x09 ://6
			{
				p=180;
				while(k==0x09||k==0xff)
				{
					Nixie(2,1);
					Nixie(3,8);
					Nixie(4,0);
					jzh=keys();
					keybranch(jzh);
				}
			}
		break;
		case 0x0d ://停止
			{
			MotorStop();
			p=10;
			}
		break;
		case 0x02 ://7
			{
				p=210;
				while(k==0x02||k==0xff)
				{
					Nixie(2,2);
					Nixie(3,1);
					Nixie(4,0);
					jzh=keys();
					keybranch(jzh);
				}
			}
		break;
		case 0x06 ://8
			{
				p=240;
				while(k==0x06||k==0xff)
				{					
					Nixie(2,2);
					Nixie(3,4);
					Nixie(4,0);
					jzh=keys();
					keybranch(jzh);
				}
			}
		break;
		case 0x0e ://正转
			{
				s=0;
				while(k==0x0e||k==0xff)
				{
					Nixie(1,10);
					jzh=keys();
					keybranch(jzh);
				}
			}
		break;
		case 0x0f ://反转
			{
				s=1;
				while(k==0x0f||k==0xff)
				{
					Nixie(1,11);
					jzh=keys();
					keybranch(jzh);
				}
			}
		break;
		case 0x0b :P2=0xff;//清屏
		break;
		default: break;
	}
}



//主函数
void main(void)
{
	while(1)
	{
		jzh=keys();
		keybranch(jzh);
	}
	
}
