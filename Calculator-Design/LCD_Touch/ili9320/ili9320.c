/*
================================================================================
================================================================================
-Module: ILI 9320 驱动
-Doc   : ili9320.c
         ili9320.h
-Author: 欧俊炯
-Date  : 2012-4-21

-Versions: v0.0
-Contents: 
-Updates :
================================================================================
================================================================================
*/
#include "ili9320.h"
//包含ascii——8*16
#include "ili9320_font.h"


/****************************************************************************
* 名    称：ili9320_WriteRegister(unsigned char index,unsigned int val)
* 功    能：在指定的地址写入寄存器数据
* 调用参数：index    要写寄存器的地址
*         ：val      写入寄存器的值
* 返回参数：无
* 说    明：内部函数
* 调用方法：ili9320_WriteRegister(0x00,0x0001);//在0x00的地址写入0x0001的值
****************************************************************************/
void ili9320_WriteRegister(unsigned char index,unsigned int val)
{
 /************************************************************************
  **                                                                    **
  ** nCS       ----\__________________________________________/-------  **
  ** RS        ------\____________/-----------------------------------  **
  ** nRD       -------------------------------------------------------  **
  ** nWR       --------\_______/--------\_____/-----------------------  **
  ** DB[0:15]  ---------[index]----------[data]-----------------------  **
  **                                                                    **
  ************************************************************************/
   //选通LCD以及复位一次（清除残余显示）
   //下列控制逻辑根据上图时序控制
   LCD_CS_L();
   LCD_RS_L();
   LCD_WR_L();
   //往数据线写入地址数据
     DATA_L8() =index;
     DATA_M8() =0;
   //对应时序
   LCD_WR_H();
   LCD_RS_H();
   
   LCD_WR_L();
   //往数据线写入寄存器值    
     DATA_L8() =val;
     DATA_M8() =(val>>8);
   LCD_WR_H();   
   LCD_CS_H();
}


/****************************************************************************
* 名    称：unsigned int ili9320_ReadRegister(unsigned char index)
* 功    能：读取指定地址寄存器的值
* 入口参数：index    寄存器地址
* 出口参数：寄存器值
* 说    明：内部函数
* 调用方法：i=ili9320_ReadRegister(0x22);
****************************************************************************/
unsigned int ili9320_ReadRegister(unsigned char index)
{
//由于手上没有对应Datesheet，相关时序下次更新
	unsigned int ret;
        
	LCD_CS_L();
	LCD_RS_L();

        DATA_L8()=(unsigned char)index;
        DATA_M8()=0;       
        
	
	LCD_WR_L();
	LCD_WR_H();
	LCD_RS_H();
        
	DATA_INPUT();//总线设置为输入
        
        LCD_RD_L();
        LCD_RD_H();
        LCD_RD_L();

        ret =  DATA_M8();
        ret <<= 8;
        ret += DATA_L8();

	LCD_RD_H();
	LCD_CS_H();
        
        DATA_OUTPUT();
        
	return ret;

}


/****************************************************************************
* 名    称：ili9320_LCD_Write_Start
* 功    能：LCD写使能
* 入口参数：NULL
* 出口参数：无
* 说    明：
* 调用方法：
****************************************************************************/
void ili9320_LCD_Write_Start(void)
{
	LCD_CS_L();
	LCD_RS_L();
	
     LCD_WR_L(); 
        DATA_L8()=0x22;
        DATA_M8()=0x00;
     LCD_WR_H();

	LCD_RS_H();
}


/****************************************************************************
* 名    称：ili9320_LCD_Write_Data
* 功    能：往数据线写入指定数据
* 入口参数：写入的数据
* 出口参数：无
* 说    明：封装为函数，往数据线写入对应数据
* 调用方法：ili9320_LCD_Write_Data（1）;//写入1
****************************************************************************/
void ili9320_LCD_Write_Data(unsigned int val)
{
  LCD_WR_L();       
    DATA_L8()=val;
    DATA_M8()=(val>>8);
  LCD_WR_H();

}


/****************************************************************************
* 名    称：ili9320_LCD_Write_End
* 功    能：LCD关闭使能
* 入口参数：NULL
* 出口参数：NULL
* 说    明：禁止对LCD的所以操作，关闭使用
* 调用方法：ili9320_LCD_Write_End（）
****************************************************************************/
void ili9320_LCD_Write_End(void)
{
	LCD_CS_H();
}


/****************************************************************************
* 名    称：ili9320_LCD_Read_Start
* 功    能：LCD读使能
* 入口参数：无
* 出口参数：NULL
* 说    明：内部函数
* 调用方法：
****************************************************************************/
void ili9320_LCD_Read_Start(void)
{
	LCD_CS_L();
	LCD_RS_L();
  
  	LCD_WR_L();
      DATA_L8()=(unsigned char)0x22;
      DATA_M8()=(unsigned char)0x00;       
   	LCD_WR_H();
   	
	LCD_RS_H();
	//指IO口模式为输入，有歧义      
	DATA_INPUT();
        
	LCD_RD_L();//dummy read
	LCD_RD_H();
}


/****************************************************************************
* 名    称：ili9320_LCD_Read_Data
* 功    能：读取数据
* 入口参数：NULL
* 出口参数：读取的数据
* 说    明：封装为函数，读取总线上的数据
* 调用方法：value=ili9320_Read_Data（）;//把总线上的数据保存在value
****************************************************************************/
unsigned int ili9320_LCD_Read_Data(void)
{
	unsigned int ret;
	LCD_RD_L();

        ret =  DATA_M8();
        ret <<= 8;
        ret += DATA_L8();       
        
	LCD_RD_H();
        
	return ret;	
}


/****************************************************************************
* 名    称：ili9320_LCD_Read_End
* 功    能：LCD关闭使能
* 入口参数：NULL
* 出口参数：NULL
* 说    明：禁止对LCD的所以操作，关闭使用
* 调用方法：ili9320_LCD_Read_End（）
****************************************************************************/
void ili9320_LCD_Read_End(void)
{
	LCD_CS_H();
        DATA_OUTPUT();
}


/****************************************************************************
* 名    称：void ili9320_Initializtion()
* 功    能：初始化 ILI9320 控制器
* 入口参数：无
* 出口参数：无
* 说    明：
* 调用方法：ili9320_Initializtion();
****************************************************************************/
//注意：下次更新再注释上时序
void ili9320_Initializtion()
{
  /*****************************
  **    硬件连接说明          **
  ** STM32         ili9320    **
  ** PE0~15 <----> DB0~15     **
  ** PD15   <----> nRD        **
  ** PD14   <----> RS         **
  ** PD13   <----> nWR        **
  ** PD12   <----> nCS        **
  ** PD11   <----> nReset     **
  ** PC0    <----> BK_LED     **
  ******************************/
  PORT_INIT();//初始化端口
  PORTA=0xff;
  PORTB=0xff;
  LCD_CS_H();
	LCD_RS_H();
	LCD_WR_H();
	LCD_RD_H();
	LCD_RST_H();
	ili9320_Delay(10);	
	LCD_RST_L();
	ili9320_Delay(10);
	LCD_RST_H();
	ili9320_Delay(10);

	//initializing funciton 1
	ili9320_WriteRegister(0xe5,0x8000);
	ili9320_WriteRegister(0x00,0x0001);//开启内部OSC
	ili9320_WriteRegister(0x01,0x0100);//0x0100
		ili9320_Delay(200);//Delay 200ms
	ili9320_WriteRegister(0x02,0x0700);//0x0700//Line inversion  
	ili9320_WriteRegister(0x03,0x1018);//1030 1038//TFM=0,TRI=0,SWAP=1,16位system interface 写GRAM时翻转RGB数据到BRG数据
	ili9320_WriteRegister(0x04,0x0000);//比例缩放设置
	ili9320_WriteRegister(0x08,0x0207);//202
	ili9320_WriteRegister(0x09,0x0000);
	ili9320_WriteRegister(0x0a,0x0000);
	ili9320_WriteRegister(0x0c,0x0001);//system接口 显示静止画面
	ili9320_WriteRegister(0x0d,0x0000);
	ili9320_WriteRegister(0x0f,0x0000);//RBG 接口极性设置
	
  ili9320_WriteRegister(0x60,0x2700);//G扫描设置 从G1开始
	ili9320_WriteRegister(0x61,0x0001);
	ili9320_WriteRegister(0x6a,0x0000);//不使用卷曲功能
	
  ili9320_WriteRegister(0x80,0x0000);//R80,Partial image disabled(6 registers)
	ili9320_WriteRegister(0x81,0x0000);
	ili9320_WriteRegister(0x82,0x0000);
	ili9320_WriteRegister(0x83,0x0000);
	ili9320_WriteRegister(0x84,0x0000);
	ili9320_WriteRegister(0x85,0x0000);
	
  ili9320_WriteRegister(0x90,0x0010);//R90,Panel control setup
	ili9320_WriteRegister(0x92,0x0000);
	ili9320_WriteRegister(0x93,0x0003);
	ili9320_WriteRegister(0x95,0x0110);
	ili9320_WriteRegister(0x97,0x0000);
	ili9320_WriteRegister(0x98,0x0000);

	//power setting function电源设置
	ili9320_WriteRegister(0x10,0x0000);//R10,	Output disabled
	ili9320_WriteRegister(0x11,0x0000);//R11,
	ili9320_WriteRegister(0x12,0x0000);//R12,
	ili9320_WriteRegister(0x13,0x0000);//R13,
	ili9320_Delay(200);//Delay 200ms
	ili9320_WriteRegister(0x10,0x17b0);//R10,	Source/Drived enabled
	ili9320_WriteRegister(0x11,0x0137);//R11,	Reference voltage set  0004
	ili9320_Delay(50);
	ili9320_WriteRegister(0x12,0x0139);//R12,	Gray control 013e
	ili9320_Delay(50);
	ili9320_WriteRegister(0x13,0x1700);//1f00//R13,	Amplitude select
	ili9320_WriteRegister(0x29,0x000c);//000f//R29,	Voltage control
		ili9320_WriteRegister(0x2b,0x0010);
	ili9320_Delay(50);
	
	ili9320_WriteRegister(0x20,0x0000);//R20,	GRAM x position set
	ili9320_WriteRegister(0x21,0x0000);//R21,	GRAM y position set
       
  ili9320_WriteRegister(0x0030, 0x0006);//0x0000);
  ili9320_WriteRegister(0x0031, 0x0101);//0x0507);
  ili9320_WriteRegister(0x0032, 0x0003);//0x0104);
  ili9320_WriteRegister(0x0035, 0x0106);//0x0105);
  ili9320_WriteRegister(0x0036, 0x0b02);//0x0404);
  ili9320_WriteRegister(0x0037, 0x0302);//0x0603);
  ili9320_WriteRegister(0x0038, 0x0707);//0x0004);
  ili9320_WriteRegister(0x0039, 0x0007);//0x0007);
  ili9320_WriteRegister(0x003C, 0x0600);//0x0501);
  ili9320_WriteRegister(0x003D, 0x020b);//0x0404);
  
  ili9320_WriteRegister(0x30,0x0000);
	ili9320_WriteRegister(0x31,0x0507);
	ili9320_WriteRegister(0x32,0x0104);
	ili9320_WriteRegister(0x35,0x0105);
	ili9320_WriteRegister(0x36,0x0404);
	ili9320_WriteRegister(0x37,0x0603);
	ili9320_WriteRegister(0x38,0x0004);
	ili9320_WriteRegister(0x39,0x0007);
	ili9320_WriteRegister(0x3c,0x0501);
	ili9320_WriteRegister(0x3d,0x0404);
        
  ili9320_WriteRegister(0x30,0x0000);	//R30,	Gamma polarity adjustment
	ili9320_WriteRegister(0x31,0x0507);	//R31,	
	ili9320_WriteRegister(0x32,0x0104);	//R32,	
	ili9320_WriteRegister(0x35,0x0105); //R35,	
	ili9320_WriteRegister(0x36,0x0404);	//R36,	
	ili9320_WriteRegister(0x37,0x0603);	//R37,	
	ili9320_WriteRegister(0x38,0x0004);	//R38,	
	ili9320_WriteRegister(0x39,0x0007);	//R39,	
	ili9320_WriteRegister(0x3C,0x0501);	//R3C,	
	ili9320_WriteRegister(0x3D,0x0404);	//R3D,
	
	ili9320_WriteRegister(0x50,0x0000);//水平GRAM起始位置
	ili9320_WriteRegister(0x51,0x00ef);//水平GRAM终止位置
	ili9320_WriteRegister(0x52,0x0000);//垂直GRAM起始位置
	ili9320_WriteRegister(0x53,0x013f);//垂直GRAM终止位置
	

	
	ili9320_WriteRegister(0x07,0x0173);//display on  173
  ili9320_Clear(0xffff);//清屏
        
}


/****************************************************************************
* 名    称：void ili9320_SetCursor(u16 x,u16 y)
* 功    能：设置屏幕座标
* 入口参数：x      行座标
*           y      列座标
* 出口参数：无
* 说    明：与ili9320初始化模式有关，灵活运用239-x、319-x等等转换
* 调用方法：ili9320_SetCursor(10,10);
****************************************************************************/
void ili9320_SetCursor(u16 x,u16 y)
{
//横屏
   ili9320_WriteRegister(0x20,y);     //行
   ili9320_WriteRegister(0x21,319-x); //列
}


/****************************************************************************
* 名    称：void ili9320_SetWindows(u16 StartX,u16 StartY,u16 EndX,u16 EndY)
* 功    能：设置窗口区域
* 入口参数：StartX     行起始座标
*           StartY     列起始座标
*           EndX       行结束座标
*           EndY       列结束座标
* 出口参数：无
* 说    明：
* 调用方法：ili9320_SetWindows(0,0,100,100)；
****************************************************************************/
//注意：下次修改为指定窗口显示，填充
void ili9320_SetWindows(u16 StartX,u16 StartY,u16 EndX,u16 EndY)
{
  ili9320_WriteRegister(0x20,StartX); // 行
  ili9320_WriteRegister(0x21,StartY); // 列
  
  ili9320_WriteRegister(0x50, StartX);//水平GRAM起始位置
  ili9320_WriteRegister(0x52, StartY);//垂直GRAM起始位置
  ili9320_WriteRegister(0x51, EndX);  //水平GRAM终止位置
  ili9320_WriteRegister(0x53, EndY);  //垂直GRAM终止位置 
}

//选择这个TFT液晶屏
void ili9320_LCD_SetWindowToFullScreen(void)
{
  ili9320_WriteRegister(0x20,0); // 行
  ili9320_WriteRegister(0x21,0); // 列
  
  ili9320_WriteRegister(0x50,0x0000);
  ili9320_WriteRegister(0x52,0x0000);
  ili9320_WriteRegister(0x51,0x00ef);
  ili9320_WriteRegister(0x53,0x013f);		
}


/****************************************************************************
* 名    称：void ili9320_Clear(u16 dat)
* 功    能：将屏幕填充成指定的颜色，如清屏，则填充 0xffff
* 入口参数：dat      填充值
* 出口参数：无
* 说    明：
* 调用方法：ili9320_Clear(0xffff);
****************************************************************************/

void ili9320_Clear(u16 idx)
{
  u32  i;

  ili9320_WriteRegister(0x20,0); // 行
  ili9320_WriteRegister(0x21,0); // 列
  
  ili9320_WriteRegister(0x0050,0x00);//水平GRAM起始位置
  ili9320_WriteRegister(0x0051,239); //水平GRAM终止位置
  ili9320_WriteRegister(0x0052,0x00);//垂直GRAM起始位置
  ili9320_WriteRegister(0x0053,319); //垂直GRAM终止位置   

  ili9320_LCD_Write_Start();
  for(i=0;i<76800;i++)
    ili9320_LCD_Write_Data(idx);
  ili9320_LCD_Write_End();
}

/****************************************************************************
* 名    称：u16 ili9320_GetPoint(u16 x,u16 y)
* 功    能：获取指定座标的颜色值
* 入口参数：x      行座标
*           y      列座标
* 出口参数：当前座标颜色值
* 说    明：
* 调用方法：i=ili9320_GetPoint(10,10);
****************************************************************************/
u16 ili9320_GetPoint(u16 x,u16 y)
{

  ili9320_WriteRegister(0x20,x); // 行
  ili9320_WriteRegister(0x21,y); // 列
  return (ili9320_BGR2RGB(ili9320_ReadRegister(0x22)));
}


/****************************************************************************
* 名    称：void ili9320_SetPoint(u16 x,u16 y,u16 point)
* 功    能：在指定座标画点
* 入口参数：x      行座标
*           y      列座标
*           point  点的颜色
* 出口参数：无
* 说    明：
* 调用方法：ili9320_SetPoint(10,10,0x0fe0);
****************************************************************************/
void ili9320_SetPoint(u16 x,u16 y,u16 point)
{
  //加上范围判断
//  if ( (x>240)||(y>320) ) return;
  ili9320_SetCursor(x,y);
  
  ili9320_LCD_Write_Start();
  ili9320_LCD_Write_Data(point);
  ili9320_LCD_Write_End();
}

void ili9320_SetPoint2(u16 x,u16 y,u16 point)
{
  //加上范围判断
//  if ( (x>240)||(y>320) ) return;
 // ili9320_SetCursor(x,y);
  ili9320_WriteRegister(0x20,239-y); // 行
  ili9320_WriteRegister(0x21,319-x); // 列
  
  
  ili9320_LCD_Write_Start();
  ili9320_LCD_Write_Data(point);
  ili9320_LCD_Write_End();
}

/****************************************************************************
* 名    称：void ili9320_DrawPicture(u16 StartX,u16 StartY,u16 EndX,u16 EndY,u16 *pic)
* 功    能：在指定座标范围显示一副图片
* 入口参数：StartX     行起始座标
*           StartY     列起始座标
*           EndX       行结束座标
*           EndY       列结束座标
            pic        图片头指针
* 出口参数：无
* 说    明：图片取模格式为水平扫描，16位颜色模式
* 调用方法：ili9320_DrawPicture(0,0,100,100,(u16*)demo);
****************************************************************************/
void ili9320_DrawPictureu16(u16 StartX,u16 StartY,u16 sizeX,u16 sizeY,u16 *pic)
{

  u16  i;
  ili9320_SetWindows(StartX,StartY,StartX+sizeX-1,StartY+sizeY-1);
  ili9320_SetCursor(StartX,StartY);
  ili9320_LCD_Write_Start();
  for (i=0;i<(sizeX*sizeY);i++)
  {
   ili9320_LCD_Write_Data(*pic++);
  }
  ili9320_LCD_Write_End();
  ili9320_LCD_SetWindowToFullScreen();
}





void ili9320_DrawPicture2(u16 StartX,u16 StartY,u16 sizeX,u16 sizeY,const u8 *pic)
{
  u16  i,j,color;

  for (j = 0; j<sizeY; j++)
      {
    	ili9320_SetCursor(StartX,StartY);
        ili9320_LCD_Write_Start();  
    	for(i = 0; i<sizeX; i++)
    	{color=*pic++;
         color=color<<8;
         ili9320_LCD_Write_Data(color|*pic++);
        }
      StartY++;//换行
      }
  
  ili9320_LCD_Write_End();
  
}



/****************************************************************************
* 名    称：void ili9320_PutChar(u16 x,u16 y,u8 c,u16 charColor,u16 bkColor)
* 功    能：在指定座标显示一个8x16点阵的ascii字符
* 入口参数：x          行座标
*           y          列座标
*           charColor  字符的颜色
*           bkColor    字符背景颜色
* 出口参数：无
* 说    明：显示范围限定为可显示的ascii码
* 调用方法：ili9320_PutChar(10,10,'a',0x0000,0xffff);
****************************************************************************/
void ili9320_PutChar(u16 x,u16 y,u8 c,u16 charColor,u16 bkColor)
{
  u16 i=0;
  u16 j=0;
  u8 tmp_char=0;

  for (i=0;i<16;i++)
  {
    tmp_char=ascii_8x16[((c-0x20)*16)+i];
    for (j=0;j<8;j++)
    {
      if ( (tmp_char >> 7-j) & 0x01 == 0x01)
        {
          ili9320_SetPoint(x+j,y+i,charColor); // 字符颜色
        }
        else
        {
          ili9320_SetPoint(x+j,y+i,bkColor); // 背景颜色
        }
    }
  }
}


/****************************************************************************
* 名    称：void ili9320_Test()
* 功    能：测试液晶屏
* 入口参数：无
* 出口参数：无
* 说    明：显示彩条，测试液晶屏是否正常工作
* 调用方法：ili9320_Test();
****************************************************************************/
void ili9320_Test()
{
  u16 i,j;
  ili9320_SetCursor(0,0);
  ili9320_LCD_Write_Start();
  
  for(i=0;i<320;i++)
    for(j=0;j<240;j++)
    {
      if(i>279)ili9320_LCD_Write_Data(0x00);
      else if(i>239)ili9320_LCD_Write_Data(0x1f);
      else if(i>199)ili9320_LCD_Write_Data(0x07e0);
      else if(i>159)ili9320_LCD_Write_Data(0x07ff);
      else if(i>119)ili9320_LCD_Write_Data(0xf800);
      else if(i>79)ili9320_LCD_Write_Data(0xf81f);
      else if(i>39)ili9320_LCD_Write_Data(0xffe0);
      else ili9320_LCD_Write_Data(0xffff);
    }
  ili9320_LCD_Write_End();
  ili9320_SetCursor(0,0);
}



void ili9320_PutChar2(u16 x,u16 y,u8 *c,u16 charColor,u16 bkColor)
{
  u16 i=0;
  u16 j=0;
  u8 tmp_char=0;

  for (i=0;i<16;i++)
  {
    tmp_char=ascii_8x16[((*c-0x20)*16)+i];
    for (j=0;j<8;j++)
    {
      if ( (tmp_char >> 7-j) & 0x01 == 0x01)
        {
          ili9320_SetPoint(x+j,y+i,charColor); // 字符颜色
        }
        else
        {
          ili9320_SetPoint(x+j,y+i,bkColor); // 背景颜色
        }
    }
  }
}




void ili9320_Text2(u16 x, u16 y, u8 *str, u16 Color, u16 bkColor)
{
  u8 i;
  u16 len;
  len=strlen(str);
  
  for (i=0;i<len;i++)
  {
    ili9320_PutChar2((x+8*i),y,str++,Color,bkColor);
  }
}







/****************************************************************************
* 名    称：u16 ili9320_BGR2RGB(u16 c)
* 功    能：RRRRRGGGGGGBBBBB 改为 BBBBBGGGGGGRRRRR 格式
* 入口参数：c      BRG 颜色值
* 出口参数：RGB 颜色值
* 说    明：内部函数调用
* 调用方法：
****************************************************************************/
u16 ili9320_BGR2RGB(u16 c)
{
  u16  r, g, b;

  b = (c>>0)  & 0x1f;
  g = (c>>5)  & 0x3f;
  r = (c>>11) & 0x1f;
  
  return( (b<<11) + (g<<5) + (r<<0) );
}


/****************************************************************************
* 名    称：void ili9320_Reset()
* 功    能：复位 ili9320 控制器
* 入口参数：无
* 出口参数：无
* 说    明：复位控制器，内部函数
* 调用方法：ili9320_Reset()
****************************************************************************/
void ili9320_Reset()
{
  /***************************************
   **                                   **
   **  -------\______________/-------   **
   **         | <---Tres---> |          **
   **                                   **
   **  Tres: Min.1ms                    **
   ***************************************/
    
  LCD_RST_H();
    ili9320_Delay(10);
  LCD_RST_L();
    ili9320_Delay(10);
  LCD_RST_H();
    ili9320_Delay(10);
}

/****************************************************************************
* 名    称：void ili9320_BackLight(u8 status)
* 功    能：开、关液晶背光
* 入口参数：status     1:背光开  0:背光关
* 出口参数：无
* 说    明：
* 调用方法：ili9320_BackLight(1);
****************************************************************************/
void ili9320_BackLight(u8 status)
{
  if ( status >= 1 )
  {
    //GPIO_SetBits(GPIOC,LCD_BK);
  }
  else
  {
    //GPIO_ResetBits(GPIOC,LCD_BK);
  }
}

/****************************************************************************
* 名    称：void ili9320_Delay(vu32 nCount)
* 功    能：延时
* 入口参数：nCount   延时值
* 出口参数：无
* 说    明：
* 调用方法：ili9320_Delay(10000);
****************************************************************************/
void ili9320_Delay(unsigned int MS)
{ 
   unsigned int i,j; 
   for( j=0;j<MS;j++ ) 
   { 
      for( i=0;i<999;i++ ) 
      { 
         asm("nop"); 
      } 
   } 
} 



