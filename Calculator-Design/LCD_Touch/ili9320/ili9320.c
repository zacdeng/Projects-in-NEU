/*
================================================================================
================================================================================
-Module: ILI 9320 ����
-Doc   : ili9320.c
         ili9320.h
-Author: ŷ����
-Date  : 2012-4-21

-Versions: v0.0
-Contents: 
-Updates :
================================================================================
================================================================================
*/
#include "ili9320.h"
//����ascii����8*16
#include "ili9320_font.h"


/****************************************************************************
* ��    �ƣ�ili9320_WriteRegister(unsigned char index,unsigned int val)
* ��    �ܣ���ָ���ĵ�ַд��Ĵ�������
* ���ò�����index    Ҫд�Ĵ����ĵ�ַ
*         ��val      д��Ĵ�����ֵ
* ���ز�������
* ˵    �����ڲ�����
* ���÷�����ili9320_WriteRegister(0x00,0x0001);//��0x00�ĵ�ַд��0x0001��ֵ
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
   //ѡͨLCD�Լ���λһ�Σ����������ʾ��
   //���п����߼�������ͼʱ�����
   LCD_CS_L();
   LCD_RS_L();
   LCD_WR_L();
   //��������д���ַ����
     DATA_L8() =index;
     DATA_M8() =0;
   //��Ӧʱ��
   LCD_WR_H();
   LCD_RS_H();
   
   LCD_WR_L();
   //��������д��Ĵ���ֵ    
     DATA_L8() =val;
     DATA_M8() =(val>>8);
   LCD_WR_H();   
   LCD_CS_H();
}


/****************************************************************************
* ��    �ƣ�unsigned int ili9320_ReadRegister(unsigned char index)
* ��    �ܣ���ȡָ����ַ�Ĵ�����ֵ
* ��ڲ�����index    �Ĵ�����ַ
* ���ڲ������Ĵ���ֵ
* ˵    �����ڲ�����
* ���÷�����i=ili9320_ReadRegister(0x22);
****************************************************************************/
unsigned int ili9320_ReadRegister(unsigned char index)
{
//��������û�ж�ӦDatesheet�����ʱ���´θ���
	unsigned int ret;
        
	LCD_CS_L();
	LCD_RS_L();

        DATA_L8()=(unsigned char)index;
        DATA_M8()=0;       
        
	
	LCD_WR_L();
	LCD_WR_H();
	LCD_RS_H();
        
	DATA_INPUT();//��������Ϊ����
        
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
* ��    �ƣ�ili9320_LCD_Write_Start
* ��    �ܣ�LCDдʹ��
* ��ڲ�����NULL
* ���ڲ�������
* ˵    ����
* ���÷�����
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
* ��    �ƣ�ili9320_LCD_Write_Data
* ��    �ܣ���������д��ָ������
* ��ڲ�����д�������
* ���ڲ�������
* ˵    ������װΪ��������������д���Ӧ����
* ���÷�����ili9320_LCD_Write_Data��1��;//д��1
****************************************************************************/
void ili9320_LCD_Write_Data(unsigned int val)
{
  LCD_WR_L();       
    DATA_L8()=val;
    DATA_M8()=(val>>8);
  LCD_WR_H();

}


/****************************************************************************
* ��    �ƣ�ili9320_LCD_Write_End
* ��    �ܣ�LCD�ر�ʹ��
* ��ڲ�����NULL
* ���ڲ�����NULL
* ˵    ������ֹ��LCD�����Բ������ر�ʹ��
* ���÷�����ili9320_LCD_Write_End����
****************************************************************************/
void ili9320_LCD_Write_End(void)
{
	LCD_CS_H();
}


/****************************************************************************
* ��    �ƣ�ili9320_LCD_Read_Start
* ��    �ܣ�LCD��ʹ��
* ��ڲ�������
* ���ڲ�����NULL
* ˵    �����ڲ�����
* ���÷�����
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
	//ָIO��ģʽΪ���룬������      
	DATA_INPUT();
        
	LCD_RD_L();//dummy read
	LCD_RD_H();
}


/****************************************************************************
* ��    �ƣ�ili9320_LCD_Read_Data
* ��    �ܣ���ȡ����
* ��ڲ�����NULL
* ���ڲ�������ȡ������
* ˵    ������װΪ��������ȡ�����ϵ�����
* ���÷�����value=ili9320_Read_Data����;//�������ϵ����ݱ�����value
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
* ��    �ƣ�ili9320_LCD_Read_End
* ��    �ܣ�LCD�ر�ʹ��
* ��ڲ�����NULL
* ���ڲ�����NULL
* ˵    ������ֹ��LCD�����Բ������ر�ʹ��
* ���÷�����ili9320_LCD_Read_End����
****************************************************************************/
void ili9320_LCD_Read_End(void)
{
	LCD_CS_H();
        DATA_OUTPUT();
}


/****************************************************************************
* ��    �ƣ�void ili9320_Initializtion()
* ��    �ܣ���ʼ�� ILI9320 ������
* ��ڲ�������
* ���ڲ�������
* ˵    ����
* ���÷�����ili9320_Initializtion();
****************************************************************************/
//ע�⣺�´θ�����ע����ʱ��
void ili9320_Initializtion()
{
  /*****************************
  **    Ӳ������˵��          **
  ** STM32         ili9320    **
  ** PE0~15 <----> DB0~15     **
  ** PD15   <----> nRD        **
  ** PD14   <----> RS         **
  ** PD13   <----> nWR        **
  ** PD12   <----> nCS        **
  ** PD11   <----> nReset     **
  ** PC0    <----> BK_LED     **
  ******************************/
  PORT_INIT();//��ʼ���˿�
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
	ili9320_WriteRegister(0x00,0x0001);//�����ڲ�OSC
	ili9320_WriteRegister(0x01,0x0100);//0x0100
		ili9320_Delay(200);//Delay 200ms
	ili9320_WriteRegister(0x02,0x0700);//0x0700//Line inversion  
	ili9320_WriteRegister(0x03,0x1018);//1030 1038//TFM=0,TRI=0,SWAP=1,16λsystem interface дGRAMʱ��תRGB���ݵ�BRG����
	ili9320_WriteRegister(0x04,0x0000);//������������
	ili9320_WriteRegister(0x08,0x0207);//202
	ili9320_WriteRegister(0x09,0x0000);
	ili9320_WriteRegister(0x0a,0x0000);
	ili9320_WriteRegister(0x0c,0x0001);//system�ӿ� ��ʾ��ֹ����
	ili9320_WriteRegister(0x0d,0x0000);
	ili9320_WriteRegister(0x0f,0x0000);//RBG �ӿڼ�������
	
  ili9320_WriteRegister(0x60,0x2700);//Gɨ������ ��G1��ʼ
	ili9320_WriteRegister(0x61,0x0001);
	ili9320_WriteRegister(0x6a,0x0000);//��ʹ�þ�������
	
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

	//power setting function��Դ����
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
	
	ili9320_WriteRegister(0x50,0x0000);//ˮƽGRAM��ʼλ��
	ili9320_WriteRegister(0x51,0x00ef);//ˮƽGRAM��ֹλ��
	ili9320_WriteRegister(0x52,0x0000);//��ֱGRAM��ʼλ��
	ili9320_WriteRegister(0x53,0x013f);//��ֱGRAM��ֹλ��
	

	
	ili9320_WriteRegister(0x07,0x0173);//display on  173
  ili9320_Clear(0xffff);//����
        
}


/****************************************************************************
* ��    �ƣ�void ili9320_SetCursor(u16 x,u16 y)
* ��    �ܣ�������Ļ����
* ��ڲ�����x      ������
*           y      ������
* ���ڲ�������
* ˵    ������ili9320��ʼ��ģʽ�йأ��������239-x��319-x�ȵ�ת��
* ���÷�����ili9320_SetCursor(10,10);
****************************************************************************/
void ili9320_SetCursor(u16 x,u16 y)
{
//����
   ili9320_WriteRegister(0x20,y);     //��
   ili9320_WriteRegister(0x21,319-x); //��
}


/****************************************************************************
* ��    �ƣ�void ili9320_SetWindows(u16 StartX,u16 StartY,u16 EndX,u16 EndY)
* ��    �ܣ����ô�������
* ��ڲ�����StartX     ����ʼ����
*           StartY     ����ʼ����
*           EndX       �н�������
*           EndY       �н�������
* ���ڲ�������
* ˵    ����
* ���÷�����ili9320_SetWindows(0,0,100,100)��
****************************************************************************/
//ע�⣺�´��޸�Ϊָ��������ʾ�����
void ili9320_SetWindows(u16 StartX,u16 StartY,u16 EndX,u16 EndY)
{
  ili9320_WriteRegister(0x20,StartX); // ��
  ili9320_WriteRegister(0x21,StartY); // ��
  
  ili9320_WriteRegister(0x50, StartX);//ˮƽGRAM��ʼλ��
  ili9320_WriteRegister(0x52, StartY);//��ֱGRAM��ʼλ��
  ili9320_WriteRegister(0x51, EndX);  //ˮƽGRAM��ֹλ��
  ili9320_WriteRegister(0x53, EndY);  //��ֱGRAM��ֹλ�� 
}

//ѡ�����TFTҺ����
void ili9320_LCD_SetWindowToFullScreen(void)
{
  ili9320_WriteRegister(0x20,0); // ��
  ili9320_WriteRegister(0x21,0); // ��
  
  ili9320_WriteRegister(0x50,0x0000);
  ili9320_WriteRegister(0x52,0x0000);
  ili9320_WriteRegister(0x51,0x00ef);
  ili9320_WriteRegister(0x53,0x013f);		
}


/****************************************************************************
* ��    �ƣ�void ili9320_Clear(u16 dat)
* ��    �ܣ�����Ļ����ָ������ɫ��������������� 0xffff
* ��ڲ�����dat      ���ֵ
* ���ڲ�������
* ˵    ����
* ���÷�����ili9320_Clear(0xffff);
****************************************************************************/

void ili9320_Clear(u16 idx)
{
  u32  i;

  ili9320_WriteRegister(0x20,0); // ��
  ili9320_WriteRegister(0x21,0); // ��
  
  ili9320_WriteRegister(0x0050,0x00);//ˮƽGRAM��ʼλ��
  ili9320_WriteRegister(0x0051,239); //ˮƽGRAM��ֹλ��
  ili9320_WriteRegister(0x0052,0x00);//��ֱGRAM��ʼλ��
  ili9320_WriteRegister(0x0053,319); //��ֱGRAM��ֹλ��   

  ili9320_LCD_Write_Start();
  for(i=0;i<76800;i++)
    ili9320_LCD_Write_Data(idx);
  ili9320_LCD_Write_End();
}

/****************************************************************************
* ��    �ƣ�u16 ili9320_GetPoint(u16 x,u16 y)
* ��    �ܣ���ȡָ���������ɫֵ
* ��ڲ�����x      ������
*           y      ������
* ���ڲ�������ǰ������ɫֵ
* ˵    ����
* ���÷�����i=ili9320_GetPoint(10,10);
****************************************************************************/
u16 ili9320_GetPoint(u16 x,u16 y)
{

  ili9320_WriteRegister(0x20,x); // ��
  ili9320_WriteRegister(0x21,y); // ��
  return (ili9320_BGR2RGB(ili9320_ReadRegister(0x22)));
}


/****************************************************************************
* ��    �ƣ�void ili9320_SetPoint(u16 x,u16 y,u16 point)
* ��    �ܣ���ָ�����껭��
* ��ڲ�����x      ������
*           y      ������
*           point  �����ɫ
* ���ڲ�������
* ˵    ����
* ���÷�����ili9320_SetPoint(10,10,0x0fe0);
****************************************************************************/
void ili9320_SetPoint(u16 x,u16 y,u16 point)
{
  //���Ϸ�Χ�ж�
//  if ( (x>240)||(y>320) ) return;
  ili9320_SetCursor(x,y);
  
  ili9320_LCD_Write_Start();
  ili9320_LCD_Write_Data(point);
  ili9320_LCD_Write_End();
}

void ili9320_SetPoint2(u16 x,u16 y,u16 point)
{
  //���Ϸ�Χ�ж�
//  if ( (x>240)||(y>320) ) return;
 // ili9320_SetCursor(x,y);
  ili9320_WriteRegister(0x20,239-y); // ��
  ili9320_WriteRegister(0x21,319-x); // ��
  
  
  ili9320_LCD_Write_Start();
  ili9320_LCD_Write_Data(point);
  ili9320_LCD_Write_End();
}

/****************************************************************************
* ��    �ƣ�void ili9320_DrawPicture(u16 StartX,u16 StartY,u16 EndX,u16 EndY,u16 *pic)
* ��    �ܣ���ָ�����귶Χ��ʾһ��ͼƬ
* ��ڲ�����StartX     ����ʼ����
*           StartY     ����ʼ����
*           EndX       �н�������
*           EndY       �н�������
            pic        ͼƬͷָ��
* ���ڲ�������
* ˵    ����ͼƬȡģ��ʽΪˮƽɨ�裬16λ��ɫģʽ
* ���÷�����ili9320_DrawPicture(0,0,100,100,(u16*)demo);
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
      StartY++;//����
      }
  
  ili9320_LCD_Write_End();
  
}



/****************************************************************************
* ��    �ƣ�void ili9320_PutChar(u16 x,u16 y,u8 c,u16 charColor,u16 bkColor)
* ��    �ܣ���ָ��������ʾһ��8x16�����ascii�ַ�
* ��ڲ�����x          ������
*           y          ������
*           charColor  �ַ�����ɫ
*           bkColor    �ַ�������ɫ
* ���ڲ�������
* ˵    ������ʾ��Χ�޶�Ϊ����ʾ��ascii��
* ���÷�����ili9320_PutChar(10,10,'a',0x0000,0xffff);
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
          ili9320_SetPoint(x+j,y+i,charColor); // �ַ���ɫ
        }
        else
        {
          ili9320_SetPoint(x+j,y+i,bkColor); // ������ɫ
        }
    }
  }
}


/****************************************************************************
* ��    �ƣ�void ili9320_Test()
* ��    �ܣ�����Һ����
* ��ڲ�������
* ���ڲ�������
* ˵    ������ʾ����������Һ�����Ƿ���������
* ���÷�����ili9320_Test();
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
          ili9320_SetPoint(x+j,y+i,charColor); // �ַ���ɫ
        }
        else
        {
          ili9320_SetPoint(x+j,y+i,bkColor); // ������ɫ
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
* ��    �ƣ�u16 ili9320_BGR2RGB(u16 c)
* ��    �ܣ�RRRRRGGGGGGBBBBB ��Ϊ BBBBBGGGGGGRRRRR ��ʽ
* ��ڲ�����c      BRG ��ɫֵ
* ���ڲ�����RGB ��ɫֵ
* ˵    �����ڲ���������
* ���÷�����
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
* ��    �ƣ�void ili9320_Reset()
* ��    �ܣ���λ ili9320 ������
* ��ڲ�������
* ���ڲ�������
* ˵    ������λ���������ڲ�����
* ���÷�����ili9320_Reset()
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
* ��    �ƣ�void ili9320_BackLight(u8 status)
* ��    �ܣ�������Һ������
* ��ڲ�����status     1:���⿪  0:�����
* ���ڲ�������
* ˵    ����
* ���÷�����ili9320_BackLight(1);
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
* ��    �ƣ�void ili9320_Delay(vu32 nCount)
* ��    �ܣ���ʱ
* ��ڲ�����nCount   ��ʱֵ
* ���ڲ�������
* ˵    ����
* ���÷�����ili9320_Delay(10000);
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



