
#ifndef _ili9320_H_
#define _ili9320_H_
#include  "includes.h"

typedef unsigned long           u32;
typedef unsigned short          u16;
typedef unsigned char           u8;
typedef signed   long           s32;
typedef signed   short          s16;
typedef signed   char           s8;
typedef volatile unsigned long  vu32;

/*
 液晶屏管脚分布-(无触摸)         液晶屏管脚分布-(有触摸)
 1 - DB0      2 - DB1            1 - DB0      2 - DB1      
 3 - DB2      4 - DB3            3 - DB2      4 - DB3      
 5 - DB4      6 - DB5            5 - DB4      6 - DB5
 7 - DB6      8 - DB7            7 - DB6      8 - DB7
 9 - DB8     10 - DB9            9 - DB8     10 - DB9     
11 - DB10    12 - DB11          11 - DB10    12 - DB11 
13 - DB12    14 - DB13          13 - DB12    14 - DB13  
15 - DB14    16 - DB15          15 - DB14    16 - DB15
17 - nRD     18 - RS            17 - nCS     18 - RS
19 - nWR     20 - nCS           19 - nWR     20 - NC 
21 - NC      22 - nReset        21 - nRD     22 - nReset
23 - VCC     24 - VCC           23 - VCC     24 - VCC
25 - VCC     26 - VCC           25 - VCC     26 - VCC
27 - NC      28 - NC            27 - NC      28 - BL 
29 - GND     30 - GND           29 - GND     30 - GND
31 - BK1     32 - BK2           31 - GND     32 - GND
33 - BK3     34 - BK4           33 - TS_CLK  34 - TS_VBAT 
35 - GND     36 - GND           35 - TS_nCS  36 - TS_AUX
37 - X-      38 - Y-            37 - TS_DIN  38 - TS_DOUT
39 - X+      40 - Y+            39 - TS_BUSY 40 - TS_nPENIRQ
*/




#define DATA_INPUT()    DDRA=0;DDRB=0
                         
						
#define DATA_OUTPUT()   DDRA=0xff;DDRB=0xff
                        

#define DATA_L8()        PORTA
#define DATA_M8()        PORTB   
					                         
#define LCD_CS_H()       PTJ|=(1<<6)
#define LCD_CS_L()       PTJ&=~(1<<6);

#define LCD_RS_H()      PORTE|=(1<<3)
#define LCD_RS_L()      PORTE&=~(1<<3)

#define LCD_WR_H()      PORTE|=(1<<2)
#define LCD_WR_L()      PORTE&=~(1<<2)
 
#define LCD_RD_H()      PORTE|=(1<<5)
#define LCD_RD_L()       PORTE&=~(1<<5)

#define LCD_RST_H()     PORTK|=(1<<3)
#define LCD_RST_L()     PORTE&=~(1<<3)


#define PORT_INIT()     DDRJ_DDRJ6=1;\
                        DDRE_DDRE2=1;\
                        DDRE_DDRE3=1;\
                        DDRE_DDRE5=1;\
                        DDRK_DDRK3=1;\
                        DATA_OUTPUT()



/***************************************************
//颜色数值
//红色0xF800，绿色0x07E0，蓝色0x001F，黄色0xFFE0，黑色0x0000，白色0xFFFF，天蓝色0x07FF，紫色0xF81F
****************************************************/
#define red 	0xF800
#define green   0x07e0
#define blue    0x001f
#define yellow  0xffe0
#define black	0x0000
#define white	0xffff
#define lightblue 0x07ff
#define purple	0xf81f
#define purple_2 0xFC3E
#define colour2 0XFC3E

#define DisplayOff() ili9320_WriteRegister(0x07,0x0172)//display off
#define DisplayOn() ili9320_WriteRegister(0x07,0x0173)//display on
/*初始化液晶*/
void ili9320_Initializtion(void);
/*复位和清屏*/
void ili9320_Reset(void);
void ili9320_Clear(u16 idx);
/*写寄存器和读寄存器*/
void ili9320_WriteRegister(unsigned char index,unsigned int val);
unsigned int ili9320_ReadRegister(unsigned char index);
/*写数据和读数据*/
void ili9320_LCD_Write_Start(void);
void ili9320_LCD_Write_Data(unsigned int val);
void ili9320_LCD_Write_End(void);
void ili9320_LCD_Read_Start(void);
unsigned int ili9320_LCD_RD_DATA(void);
void ili9320_LCD_Read_End(void);
/*写点和读点*/
void ili9320_SetPoint(u16 x,u16 y,u16 point);
u16  ili9320_GetPoint(u16 x,u16 y);
/*设置坐标*/
void ili9320_SetCursor(u16 x,u16 y);
/*BGR转RGB*/
u16 ili9320_BGR2RGB(u16 c);
/*设置窗口区域*/
void ili9320_SetWindows(u16 StartX,u16 StartY,u16 EndX,u16 EndY);
void ili9320_LCD_SetWindowToFullScreen(void);
/*在指定座标显示一个8x16点阵的ascii字符*/
void ili9320_PutChar(u16 x,u16 y,u8 c,u16 charColor,u16 bkColor);
/*在指定座标范围显示一副图片*/
void ili9320_DrawPictureu16(u16 StartX,u16 StartY,u16 sizeX,u16 sizeY,u16 *pic);

void ili9320_LCD_Write_Datau8(const unsigned char val,const unsigned char val1);
void ili9320_DrawPictureu8(u16 StartX,u16 StartY,u16 sizeX,u16 sizeY,const u8 *pic);
void ili9320_DrawPicture2(u16 StartX,u16 StartY,u16 sizeX,u16 sizeY,const u8 *pic);
void ili9320_Test(void);
u16  ili9320_GetCode(void);
void ili9320_Delay(unsigned int MS);
void ili9320_BackLight(u8 status);


/*API函数*/

u16  ili9320_Color565(u32 RGB);  // 24位RGB颜色转为16位(565)
void ili9320_Text(u16 x, u16 y, u8 *str, u16 len,u16 Color, u16 bkColor); //写ASCII
void ili9320_Line(u16 x0, u16 y0, u16 x1, u16 y1,u16 color);              //画线
void ili9320_Circle(u16 cx,u16 cy,u16 r,u16 color,u8 fill);               //画园
void ili9320_Rectangle(u16 x0, u16 y0, u16 x1, u16 y1,u16 color,u8 fill); //画矩形
void ili9320_Square(u16 x0, u16 y0, u16 with, u16 color,u8 fill);         //画正方形


//void Touch_Initializtion();
//u16  Touch_GetPhyX(void);
//u16  Touch_GetPhyY(void);
//u16  Touch_MeasurementX(void);
//u16  Touch_MeasurementY(void);

#endif