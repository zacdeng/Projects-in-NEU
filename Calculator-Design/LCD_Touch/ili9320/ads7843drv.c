//=========================================================================================================================================
#include "includes.h"


//====================================================================================

#define NOP() _asm(nop)

uint16 touchx=0,touchy=0,touchflag=0;

void Delays(void) 
{
    NOP(); NOP();NOP();
  
}



//从ADS7843中读取数据
static unsigned short RD_AD(void) 
{
	unsigned int temp=0;
  unsigned char i;
		
  TP_DIN_0;			
  TP_DCLK_1;
  
    for(i=0;i<12;i++) 
    {
        TP_DCLK_0;         
        if(TP_DOUT_Value) temp++;
        temp<<=1;
        TP_DCLK_1;
    }

  TP_CS_1;
  return(temp);
}


//发送读取命令，返回读出单坐标
unsigned int read_trouch (unsigned char cmd) 
{
    unsigned char i;
		
    TP_CS_1;
    TP_DIN_0;
    TP_DCLK_0;
    TP_CS_0;
    for(i=0;i<8;i++) 
    {
        if(cmd&0x80) TP_DIN_1;
        else  TP_DIN_0;
        cmd=cmd<<1;
        TP_DCLK_1;
		    Delays();
        TP_DCLK_0;
    }
	return RD_AD();
}

/*
//读取当前触点坐标
void touch_GetAdXY(unsigned int *x,unsigned int *y) 
{
    *x=read_trouch(CHX);
    *y=read_trouch(CHY);
}

*/
uint16  TOUCH_X_MeasureX(void) 
{
	unsigned char t=0,t1,count=0;
	unsigned short int databuffer[10]={5,7,9,3,2,6,4,0,3,1};//数据组
	unsigned short temp=0,X=0;	
 	
	while(count<10)//循环读数10次
	{	   	  
		databuffer[count]=read_trouch(CHX);;
		count++; 
	}  
	if(count==10)//一定要读到10次数据,否则丢弃
	{  
	    do//将数据X升序排列
		{	
			t1=0;		  
			for(t=0;t<count-1;t++)
			{
				if(databuffer[t]>databuffer[t+1])//升序排列
				{
					temp=databuffer[t+1];
					databuffer[t+1]=databuffer[t];
					databuffer[t]=temp;
					t1=1; 
				}  
			}
		}while(t1); 	    		 	 		  
		
		X=(databuffer[3]+databuffer[4]+databuffer[5])/3;	  
//		if(X<=3730&&Y<=3730) //个人的屏根据初始参数修改.
//		{
//			if(X>=330)X-=330;
//			else X=0;
//			if(Y>=420)Y-=420;
//			else Y=0;  
//			drawbigpoint(240-X/14,320-Y/10);	 
//		}  
	}
	return(X);  
}

uint16  TOUCH_X_MeasureY(void) {
  	unsigned char t=0,t1,count=0;
	unsigned short int databuffer[10]={5,7,9,3,2,6,4,0,3,1};//数据组
	unsigned short temp=0,Y=0;	
 
    while(count<10)	//循环读数10次
	{	   	  
		databuffer[count]=read_trouch(CHY);;
		count++;  
	}  
	if(count==10)//一定要读到10次数据,否则丢弃
	{  
	    do//将数据X升序排列
		{	
			t1=0;		  
			for(t=0;t<count-1;t++)
			{
				if(databuffer[t]>databuffer[t+1])//升序排列
				{
					temp=databuffer[t+1];
					databuffer[t+1]=databuffer[t];
					databuffer[t]=temp;
					t1=1; 
				}  
			}
		}while(t1); 	    		 	 		  
		Y=(databuffer[3]+databuffer[4]+databuffer[5])/3;	  
//		if(X<=3730&&Y<=3730) //个人的屏根据初始参数修改.
//		{
//			if(X>=330)X-=330;
//			else X=0;
//			if(Y>=420)Y-=420;
//			else Y=0;  
//			drawbigpoint(240-X/14,320-Y/10);	 
//		}   
	}
	return(Y); 
}


  


uint16 _AD2X(uint16 adx)
{
  int16 sx=0;
  int32 r = adx - TOUCH_AD_LEFT;
  r *= 319;
  sx=r / (TOUCH_AD_RIGHT - TOUCH_AD_LEFT);
  if (sx<=0 || sx>320)
    return 0;
  return sx;
}


uint16 _AD2Y(uint16 ady)
{
  int16 sy=0;
  int32 r = ady - TOUCH_AD_TOP;
  r *= 239;
  sy=r/(TOUCH_AD_BOTTOM - TOUCH_AD_TOP);
  if (sy<=0 || sy>240)
    return 0;
  return sy;
}


void touch_GetAdXY2(unsigned int *x,unsigned int *y) 
{
    *x=_AD2X(TOUCH_X_MeasureX());           //记录触屏坐标的X      
    *y=_AD2Y(TOUCH_X_MeasureY());           //记录触屏坐标的Y
}





