#ifndef _ADS7843DRV_H_
#define _ADS7843DRV_H_

// A/D ͨ��ѡ�������ֺ͹����Ĵ���
#define	CHX 	0x90 	//ͨ��Y+��ѡ�������	
#define	CHY 	0xD0	//ͨ��X+��ѡ�������	


#define TOUCH_AD_LEFT       452
#define TOUCH_AD_RIGHT      3845
#define TOUCH_AD_TOP        260
#define TOUCH_AD_BOTTOM     3800

//�������Ŷ���    

extern uint16 touchx,touchy,touchflag;

#define TP_CS_1			PTM_PTM3=1
#define TP_DCLK_1		PTM_PTM5=1	
#define TP_DIN_1		PTM_PTM4=1

#define TP_CS_0			PTM_PTM3=0
#define TP_DCLK_0		PTM_PTM5=0
#define TP_DIN_0		PTM_PTM4=0

#define TP_DOUT_Value	(PTM_PTM2)


		
void touch_Init(void);
unsigned int read_trouch (unsigned char cmd);
void touch_GetAdXY(unsigned int *x,unsigned int *y); 


#endif


