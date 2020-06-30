#include <hidef.h>      /* common defines and macros */
#include "derivative.h"      /* derivative-specific definitions */
#include "includes.h"

uint8 main_select=0;        //既做菜单的编号也做显示选择框的标志位,与key_value相应
byte Key=16;
extern uint16 touchx,touchy,touchflag;

uint16 main_selectxy[22][2]=         //界面的20个选项的XY坐标
{
	10 ,210,   //0
		10 ,165,   //1
		70 ,165,   //2
		130,165,   //3
		10 ,120,   //4
		70 ,120,   //5
		130,120,   //6
		10 ,75 ,   //7
		70 ,75 ,   //8
		130,75 ,   //9
		
		10 ,30 ,   //10(C)
		250,165,   //11(=)
		
		190,75 ,   //12(+)
		190,120,   //13(-)
		190,165,   //14(*)
		190,210,   //15(/)
		130,30 ,   //16(DEL)
		250,30 ,   //17(^)
		190,30 ,   //18(B)
		130,210,   //19(.)
		250,75,   //20(%)求余数
		250,120,   //21(1/x)求倒数
};


/*
* 函数名：SetBusCLK_40M
* 功能：初始化40M时钟频率
* 参数：NULL
*/
void SetBusCLK_40M(void)
{   
    CLKSEL=0X00;				//disengage PLL to system
    PLLCTL_PLLON=1;			//turn on PLL
    SYNR =0xc0 | 0x04;                        
    REFDV=0x80 | 0x01; 
    POSTDIV=0x00;       //pllclock=2*osc*(1+SYNR)/(1+REFDV)=80MHz;
    _asm(nop);          //BUS CLOCK=40M
    _asm(nop);
    while(!(CRGFLG_LOCK==1));	  //when pll is steady ,then use it;
    CLKSEL_PLLSEL =1;		        //engage PLL to system; 
}

/*
* 函数名：touch_Init
* 功能：初始化触摸屏，用于接受数据
* 参数：NULL
*/
void touch_Init(void)
{
	DDRH_DDRH3=1;
	PTH_PTH3=0;
	
	DDRH_DDRH3=0;
	PERH_PERH3=1;
	
	DDRM_DDRM2=0;
	PERM_PERM2=1;
	
	DDRM_DDRM3=1;
	DDRM_DDRM4=1;
	DDRM_DDRM5=1;
}

//-----------------------------------------------------------------------------  
//
//寻找触屏位置
//
//-----------------------------------------------------------------------------  
void Touch(void)
{
    
	touch_GetAdXY2(&touchx,&touchy); //把触屏位置记录在指针touchx,touchy中
	touchflag=1;                     //触屏标志位置1
	
}

/*
* 函数名：EXT_PH3_Init
* 功能：初始化外部中断，接受触摸屏的中断输入
* 参数：NULL
*/
void EXT_PH3_Init(void) 
{               
	DDRH_DDRH3=0;  //设置为输入
	
	PERH_PERH3=1;  //上拉电阻或下拉电阻开启
	PPSH_PPSH3=0;  //选择上拉电阻，下降沿触发 
	PIEH_PIEH3=1;  //开启外部中断
}

//-------------------------------------------------------------------------
//
//形参：
//     beginx:区域的X开始
//     beginy:区域的Y开始
//     sizex: 区域的长x
//     sizey: 区域的宽y
//     touchx:触屏的位置x
//     touchy:触屏的位置y
//
//-------------------------------------------------------------------------
uint8 touch_select(uint16 beginx,uint16 beginy,uint16 sizex,uint16 sizey,
                   uint16 touchx,uint16 touchy)
{
	if(   (touchx<=(beginx+sizex)&&touchx>=beginx) 
		&&(touchy<=(beginy+sizey)&&touchy>=beginy)  )
		return 1;
	else
		return 0;
}


//****************************************************************************************
/*
//延时函数//40MHz--1ms  
*/
//****************************************************************************************
void delay_ms(uint ms) 
{   
    int ii,jj;
    if (ms<1) ms=1;
    for(ii=0;ii<ms;ii++)
        for(jj=0;jj<3338;jj++);    //40MHz--1ms      
}



//**************************************************************************
//
//初始化PIT定时器
//
//**************************************************************************
void init_PIT0(void)     
{
	
    PITCFLMT_PITE=0;    //禁止PTI定时器
	
    PITTF=0X0F;         //清除中断标志位
	
    PITMUX_PMUX0=0;     
    PITINTE_PINTE0=1;   //time-out period = (PITMTLD + 1) * (PITLD + 1) / fBUS.
    
    PITMTLD0=255;       //base timer0自动装数寄存器 
	//Clock = 100K，对应100us
    
    PITLD0=5000;    
    PITCNT0=5000;       //定时164ms
    
    PITCE_PCE0=1;       //开启通道0
    
    PITFLT_PFLT0=1;     
    
    PITCFLMT_PITE=1;    //开启PIT模块//使能PTI 
	
}

int powd(double m,int n)    //自设的幂函数
{
	int i;
	double res=1;
	for(i=0;i<n;i++)
		res=res*m;
	return res;
}
//**************************************************
int remainder(double m,double n){   //取余函数
	int r;
	r = (int)m/(int)n;
	return (int)m - (int)n*r;
}
//**************************************************


//**************************************************
double daoshu(double n){   //取倒数
	double r;
	r = 1.0/n;
	return r;
} 
//**************************************************

//**************************************************

//**************************************************
long result=0 ;
double cin1=0,cin2=0;
//*****主函数开始*********
void main(void) 
{
	u8 u;
	u16 flag=0,sign=0,pflag=0,flag1=0,flag2=0,flag3=0,flag4=0;
	long res;
	u16 max=10;
	u8 pastkey[40];//={"                                        "};
	u16 x,y,m,n,p,q,l=0;
	u16 i;//记录小数点位置
	u16 j;//记录运算符的位置
	u16 eflag=0,bflag=0;
	u16 yflag=0;
	u8 num[]={"0123456789"},RS[20],point[8];
	u16 flagcal[9]={0,0,0,0,0,0,0,0,0};    //运算符号标志位数组，[6]小数点标志位
	int re_f;
	double temp,temp1;
	int i1;
	int bres[20],bcin;
	double dao;
	int yunflag = 1;
	int kongx=10;
	
	/* put your own code here */
	IRQCR_IRQEN=0;  //关闭IRQ
	IRQCR_IRQE=0;
	EnableInterrupts;;            //关中断  
	SetBusCLK_40M();							//设置总线时钟频率，40MHz
	touch_Init();							    //触摸模块初始化,用于接受触摸屏输入的AD值数据，LCD显示时不需要
	init_PIT0();
	EXT_PH3_Init(); 					  	//初始化外部中断，用于触摸输入，只显示LCD时不需要
	ili9320_Initializtion();			//液晶屏初始化
	ili9320_Test();								//液晶测试，显示彩色条纹
	
	//延时一段时间后，清屏
	delay_ms(2000);
	ili9320_Clear(0xFFFF);
	//画框
	ili9320_Rectangle(5 ,2 ,6,240,black,0); 
	ili9320_Rectangle(309,2,310,240,black,0);
	ili9320_Rectangle(5 ,2,310,3,black,0); 
	
	
	ili9320_Rectangle(10 ,210,125,240,colour2,0); 
	ili9320_PutChar((10+125)/2-4,(210+240)/2-8,'0',0x0000,purple_2);
	ili9320_Rectangle(10 ,165,65 ,205,colour2,1); 
	ili9320_PutChar((10+65)/2-4,(165+205)/2-8,'1',0x0000,purple_2);
	ili9320_Rectangle(10 ,120,65 ,160,colour2,4); 
	ili9320_PutChar((10+65)/2-4,(120+160)/2-8,'4',0x0000,purple_2);
	
	ili9320_Rectangle(10 ,75 ,65 ,115,colour2,7); 
	ili9320_PutChar((10+65)/2-4,(75+115)/2-8,'7',0x0000,purple_2);
	ili9320_Rectangle(10 ,37 ,125,70 ,lightblue,10);  //<-   
	// ili9320_PutChar((10+125)/2-4,(37+70)/2-8,'<',0x0000,colour2);
	ili9320_Text2((10+125)/2-8,(37+70)/2-8,"<-",black,lightblue);
	
	ili9320_Rectangle(70 ,165,125,205,colour2,2); 
	ili9320_PutChar((70+125)/2-4,(165+205)/2-8,'2',0x0000,purple_2);
	ili9320_Rectangle(70 ,120,125,160,colour2,5); 
	ili9320_PutChar((70+125)/2-4,(120+160)/2-8,'5',0x0000,purple_2);
	ili9320_Rectangle(70 ,75 ,125,115,colour2,8); 
	ili9320_PutChar((70+125)/2-4,(75+115)/2-8,'8',0x0000,purple_2);
	
	ili9320_Rectangle(130,210,185,250,colour2,19);  //.
	ili9320_PutChar((130+185)/2-4,(210+250)/2-8,'.',0x0000,purple_2);
	ili9320_Rectangle(130,165,185,205,colour2,3); 
	ili9320_PutChar((130+185)/2-4,(165+205)/2-8,'3',0x0000,purple_2);
	ili9320_Rectangle(130,120,185,160,colour2,6); 
	ili9320_PutChar((130+185)/2-4,(120+160)/2-8,'6',0x0000,purple_2);
	ili9320_Rectangle(130,75 ,185,115,colour2,9); 
	ili9320_PutChar((130+185)/2-4,(75+115)/2-8,'9',0x0000,purple_2);
	ili9320_Rectangle(130,37 ,185,70 ,lightblue,16);  //DEL
	ili9320_Text2((130+185)/2-6,(37+70)/2-8,"DEL",0x0000,lightblue);
	
	ili9320_Rectangle(190,210,245,250,yellow,15);  // /
	ili9320_PutChar((190+245)/2-4,(210+250)/2-8,'/',0x0000,yellow);
	
	ili9320_Rectangle(190,165,245,205,yellow,14);  //*
	ili9320_PutChar((190+245)/2-4,(165+205)/2-8,'*',0x0000,yellow);
	
	ili9320_Rectangle(190,120,245,160,yellow,13);  //-
	ili9320_PutChar((190+245)/2-4,(120+160)/2-8,'-',0x0000,yellow);
	
	ili9320_Rectangle(190,75 ,245,115,yellow,12);  //+
	ili9320_PutChar((190+245)/2-4,(75+115)/2-8,'+',0x0000,yellow);
	
	ili9320_Rectangle(190,37 ,245,70 ,yellow,18);  // B
	ili9320_PutChar((190+245)/2-4,(37+70)/2-8,'B',0x0000,yellow);
	
	
	ili9320_Rectangle(250,165,305,250,lightblue,11);  //=
	ili9320_PutChar((250+305)/2-4,(165+250)/2-8,'=',0x0000,lightblue);
	
	ili9320_Rectangle(250,120,305,160,yellow,21);  // 1/x
	
    ili9320_Text2((250+305)/2-4,(120+160)/2-8,"1/x",black,yellow);
    
	
	ili9320_Rectangle(250,75 ,305,115,yellow,20);  //%
	ili9320_PutChar((250+305)/2-4,(75+115)/2-8,'%',0x0000,yellow);
	
	
	ili9320_Rectangle(250,37 ,305,70 ,yellow,17);  //^
	ili9320_PutChar((250+305)/2-4,(37+70)/2-8,'^',0x0000,yellow);
	
	while(1)                                                              
	{
    /*ili9320_PutChar(100,5,num[flagcal[0]],0x0000,0XFC00);
    ili9320_PutChar(108,5,num[flagcal[1]],0x0000,0XFC00);
    ili9320_PutChar(116,5,num[flagcal[2]],0x0000,0XFC00);
		ili9320_PutChar(124,5,num[flagcal[3]],0x0000,0XFC00);*/
        
        if(Key<10) //如果输入的是数字：0-9
        {
			
			if(eflag==1||bflag==1||yflag==1)//二次计算初始化
			{
				ili9320_Text2(kongx,5,"                                  ",white,white);//清空输入框
				ili9320_Text2(kongx,21,"                               ",white,white);//清空结果显示框
				for(p=0;p<40;p++)
					pastkey[p]=' ';
				for(p=0;p<20;p++)
					bres[p]=0;
				flag=0;
				cin1=0;
				bcin=0;
				eflag=0;
				bflag=0;
				yflag=0;
				yunflag=0;
			}
			if((flagcal[0]==0)&&(flagcal[1]==0)&&(flagcal[2]==0)&&(flagcal[3]==0)&&(flagcal[4]==0)&&(flagcal[5]==0)&&(flagcal[7]==0))
			{
				if(flagcal[6]==0) cin1=cin1*10+Key;
				else if(flagcal[6]==1)//小数点标志位
				{
					for(i=0;i<flag;i++) //i记录第一个变量的小数点位置
					{
						if(pastkey[i]=='.')break;
					}
					cin1=cin1+(double)Key/powd(10,(flag-i));
				}
				
			} 
			else if((flagcal[0]+flagcal[1]+flagcal[2]+flagcal[3]+flagcal[4]+flagcal[5]+flagcal[7])==1)//只有一个运算符被按下，开始记录第二个变量
			{
				for(i=flag;i>0;i--) //i记录第二个变量的小数点位置
				{
					if(pastkey[i]=='.')break;
				}
				for(j=0;j<flag;j++) 
				{
					if(pastkey[j]=='+'||pastkey[j]=='-'||pastkey[j]=='*'||pastkey[j]=='/')break; //j记录"+""-""*""/"的位置
				}
				//if(i==0)cin2=cin2*10+Key;
				if(i>j)cin2=cin2+(double)Key/powd(10,(flag-i));
				else if(i<j) cin2=cin2*10+Key;
				
			} 
			else
			{
				;
			}
			ili9320_PutChar2(8*flag+kongx,5,&num[Key],black,white);       //显示输入的数字
			// ili9320_Text2(0,37,"                                  ",white,white);
			ili9320_Text2(kongx,21,"                                    ",white,white);
			pastkey[flag]=num[Key];//存储输入的数字符号
			flag++;
			Key=50;
			yunflag=0;
        } 
        else if(Key==10 && eflag==0) //退格
		{
			//ili9320_Text2(0,37,"                                                ",white,white);
			ili9320_Text2(kongx,21,"                                    ",white,white);
			if(flag==0)
			{
				;
			} 
			else 
			{
				flag--;
				ili9320_Text2(8*flag+kongx,5," ",white,white);
				if(pastkey[flag]=='+')
				{
					flagcal[0]--;
                    yunflag=0;
				} 
				else if(pastkey[flag]=='-')
				{
					flagcal[1]--;
                    yunflag=0;
				}
				else if(pastkey[flag]=='*')
				{
					flagcal[2]--;
                    yunflag=0;
				}
				else if(pastkey[flag]=='/')
				{
					flagcal[3]--;
                    yunflag=0;
				} 
				else if(pastkey[flag]=='^')
				{
					flagcal[4]--;
                    yunflag=0;
				}
                
				else if(pastkey[flag]=='.')
				{
					flagcal[6]--;
                    yunflag=0;
				} 
				//********************************
				else if(pastkey[flag]=='%'){
					flagcal[7]--;
					yunflag=0;
				}
				//********************************
				else if((pastkey[flag]>='0')&&(pastkey[flag]<='9'))
				{
					if((flagcal[0]==0)&&(flagcal[1]==0)&&(flagcal[2]==0)&&(flagcal[3]==0)&&(flagcal[4]==0)&&(flagcal[5]==0)&&(flagcal[7]==0)) 
					{
						if(flagcal[6]==0)
							cin1=(int)cin1/10; //如输入整数，则直接除以10再取整
						else if(flagcal[6]==1)
						{
							for(i=0;i<flag;i++) 
							{
								if(pastkey[i]=='.')break;
							}
							cin1=cin1-(double)pastkey[flag]/(double)powd(10,(flag-i));   //如输入小数，关键在于判断小数位置与最后一位数的距离
						}
						
					}
					else if((flagcal[0]+flagcal[1]+flagcal[2]+flagcal[3]+flagcal[4]+flagcal[5]+flagcal[7])==1) //退格的是第二个变量
					{
						for(i=flag;i>0;i--) 
						{
							if(pastkey[i]=='.')break;
						}
						for(j=0;j<flag;j++)   //记录运算符的位置
						{
							if(pastkey[j]=='+'||pastkey[j]=='-'||pastkey[j]=='*'||pastkey[j]=='/'||pastkey[j]=='%')
								break;
						}
						//if(i==0)cin2=cin2/10;
						if(i>j)cin2=cin2-(double)pastkey[flag]/(double)powd(10,(flag-i));
						else if(i<j)cin2=(int)cin2/10;
					}
					else
					{
						;
					}
				}
				pastkey[flag]=' ';
			}
			Key=50;
		}  
		else if(Key==11 && eflag==0 && yunflag==0) //等号
		{
		/*if(eflag==1)//等号标志位。连续按下=号
		{  
		//ili9320_Text2(0,37,"                                  ",white,white);
		ili9320_Text2(0,21,"                                  ",white,white);
		ili9320_Text2(0,5,"                                  ",white,white);
		ili9320_Text2(0,5,"0",black,white);
		ili9320_Text2(0,21,"=0",black,white);
		
		  ili9320_Text2(0,5,"                                  ",white,white);
		  temp = result + cin2;
		  result = temp;
		  temp1 = temp;   
		  
			} 
			*/
			if ((flagcal[0]+flagcal[1]+flagcal[2]+flagcal[3]+flagcal[4]+flagcal[5]+flagcal[7])>=2) 
			{
				//ili9320_Text2(0,37,"Oh!You gave too much + - * / !!!",black,white);
				ili9320_Text2(kongx,5,"                                    ",white,white);
				ili9320_Text2(kongx,21,"ERROR!",red,white);  
			} 
			else if(pastkey[flag-1]=='+'||pastkey[flag-1]=='-'||pastkey[flag-1]=='*'||pastkey[flag-1]=='/'||pastkey[flag-1]=='^')
			{
				// ili9320_Text2(0,37,"last sign is + - * /",black,white);
				ili9320_Text2(kongx,5,"                                   ",white,white);
				
			} 
			else if((flagcal[0]+flagcal[1]+flagcal[2]+flagcal[3]+flagcal[4]+flagcal[5]+flagcal[7])==1)
			{
				ili9320_Text2(kongx,21,"                               ",white,white);
				//ili9320_Text2(0,37,"                                  ",white,white);
				ili9320_Text2(kongx,21,"=",black,white);
				if(flagcal[0]==1)
				{
                    temp=cin1+cin2;
                    result=temp;
                    temp1=temp;
				}
				else if(flagcal[1]==1)
				{
                    temp=cin1-cin2;
                    result=temp;
                    temp1=temp;
				}
				else if(flagcal[2]==1) 
				{
                    temp=cin1*cin2;
                    result=temp;
                    temp1=temp;
				}
				else if((flagcal[3]==1)&&(cin2!=0)) 
				{
                    temp=cin1/cin2;
                    result=temp;
                    temp1=temp;
				} 
				else if(flagcal[4]==1)
				{
                    temp=powd(cin1,cin2);
                    result=temp;
                    temp1=temp;
				} 
				//*************************
				else if(flagcal[7]==1){  //取余数
                    //temp=remainder(cin1,cin2);
                    temp = (int)cin1%(int)cin2;                    
                    result=temp;
                    temp1=temp;
				}
				/*else if(flagcal[5]==1)
				{
				result=sqrt(cin2);
				temp=result;
			}*/ 
				else 
				{
                    //ili9320_Text2(0,37,"error",black,white);
				}
				//****************************************************************
				//负数变换成正数                 
				if(temp<0) 
				{
					ili9320_Text2(8+kongx,21,"-",black,white);
					sign=1;  //增加符号位的空间
					result=0-result;
					temp1=0-temp;
				}
				if(result==0)
				{
                    ili9320_Text2(8*(sign+1)+kongx,21,"0",black,white); 
				}
				re_f=0;    //结果的位数
				res=result;
				//计算位数
				while(res>=1) 
				{
					res=res/10;
					re_f++;
				}
				for(l=0;l<re_f;l++) //输出计算结果
				{
					RS[l]=result/(powd(10,re_f-l-1))%10+'0';
					ili9320_PutChar2(8*(l+1+sign)+kongx,21,&RS[l],black,white);
				}
				
				if((temp1-result)!=0)     //对小数进行处理
				{
					int z=0;
					if(result==0){
						ili9320_Text2(8*(l+1+sign)+kongx,21,"0",black,white);
						z++;
					}
                    for(q=0;q<4;q++)     //保留四位小数
					{
						point[q]=((long)((temp1+0.0000-result)*(powd(10,q+1))))%10+'0';
						
						ili9320_Text2(8*(l+1+sign+z)+kongx,21,".",black,white);
						ili9320_PutChar2(8*(l+sign+2+q+z)+kongx,21,&point[q],black,white);
					}
				}
				
			} 
			else     //只输入一个变量
			{
				ili9320_Text2(kongx,21,"                                 ",white,white);
				//ili9320_Text2(0,37,"                                  ",white,white);
				ili9320_Text2(kongx,21,"=",black,white);
				temp=cin1; 
				result=temp;
				if(result==0)
				{
                    ili9320_Text2(8*(sign+1)+kongx,21,"0",black,white); 
				}
				re_f=0;    //结果的位数
				res=result;
				//计算位数
				while(res>=1) 
				{
					res=res/10;
					re_f++;
				}
				
				for(l=0;l<re_f;l++) 
				{
					RS[l]=result/(powd(10,re_f-l-1))%10+'0';
					ili9320_PutChar2(8*(l+1+1+sign)+kongx,21,&RS[l],black,white);
				}
				if((temp-result)!=0)
				{
                    for(q=0;q<4;q++)
					{
						point[q]=((long)((temp+0.00010-result)*(powd(10,q+1))))%10+'0';
						ili9320_Text2(8*(l+sign+1)+kongx,21,".",black,white);
						ili9320_PutChar2(8*(l+1+sign+2+q)+kongx,21,&point[q],black,white);
					}
				}
			}
			//运算符标志位清零
			flagcal[0]=0;
			flagcal[1]=0;
			flagcal[2]=0;
			flagcal[3]=0;
			flagcal[4]=0;
			flagcal[5]=0;
			flagcal[6]=0;
			flagcal[7]=0;
			for(n=0;n<40;n++)
				pastkey[n]=' ' ;
			flag=0;
			eflag=1;//等号按下标志
			yflag=0;
			yunflag=1;
			cin1=0;
			cin2=0;
			Key=50;
			result=0;
             }//等号
			 else if(Key==12 && yunflag==0)  //加法
             {
                 pastkey[flag]='+';
                 flagcal[0]++;
                 ili9320_Text2(8*flag+kongx,5,"+",black,white);
                 //flag1=flag;
                 flag++;
                 Key=50;
                 yunflag=1;
             }
			 else if(Key==13&& yunflag==0)   //减法
             {
                 pastkey[flag]='-';
                 flagcal[1]++;
                 ili9320_Text2(8*flag+kongx,5,"-",black,white);
                 flag2=flag;
                 flag++;
                 Key=50;
                 yunflag=1;                 
             }
			 else if(Key==14&& yunflag==0)   //乘法
             {
                 pastkey[flag]='*';
                 flagcal[2]++;
                 ili9320_Text2(8*flag+kongx,5,"*",black,white);
                 flag3=flag;
                 flag++;
				 Key=50; 
				 yunflag=1;                
             }   
			 else if(Key==15&& yunflag==0)  // 除法
             {
                 pastkey[flag]='/';
                 flagcal[3]++;
                 ili9320_Text2(8*flag+kongx,5,"/",black,white);
                 flag4=flag;
                 flag++;
				 Key=50;
				 yunflag=1;
             } 
			 else if(Key==16) //清空
             {
				 //ili9320_Text2(0,37,"                                  ",white,white);
				 ili9320_Text2(kongx,21,"                               ",white,white);
				 ili9320_Text2(kongx,5,"                                  ",white,white); 
				 ili9320_Text2(kongx,5,"0",black,white);
				 flagcal[0]=0;
				 flagcal[1]=0;
				 flagcal[2]=0;
				 flagcal[3]=0;
				 flagcal[4]=0;
				 //flagcal[5]=0;
				 flagcal[6]=0;
				 flagcal[7]=0;
				 for(n=0;n<40;n++)
					 pastkey[n]=' ' ;
				 flag=0;
				 eflag=0;
				 yflag=0;
				 yunflag=1;
				 cin1=0;
				 cin2=0;
				 Key=50;
				 result=0; 
             } 
			 else if(Key==17&& yunflag==0) //乘方
             {
                 pastkey[flag]='^';
                 flagcal[4]++;
                 ili9320_Text2(8*flag+kongx,5,"^",black,white);
                 flag++;
				 Key=50;
				 yunflag=1;
             } 
			 else if(Key==18&&yunflag==0) //二进制
             {
                 bcin=cin1;
                 for(i=0;bcin>0;i++)
                 {
					 bres[i]=bcin%2;
					 bcin=bcin/2;
                 }
                 for(j=0;j<i;j++)
					 ili9320_PutChar2(8*j+kongx,21,&num[bres[i-j-1]],black,white);
                 bflag=1;//二进制标志位
				 Key=50;
				 yunflag=1;
             }  
			 else if(Key==19&& yunflag==0) //小数点
             {
                 pastkey[flag]='.';
                 flagcal[6]++;
                 ili9320_Text2(8*flag+kongx,5,".",black,white);
                 flag++;
				 Key=50;
				 yunflag=1;
             }
			 //**********************************
			 else if(Key==20&& yunflag==0)     //取余数
			 {
				 pastkey[flag]='%';
				 flagcal[7]++;
				 ili9320_Text2(8*flag+kongx,5,"%",black,white);
				 flag++;
				 Key=50;
				 yunflag=1;
			 } 
			 else if(Key==21) {     //取倒数
				 if(yunflag==0) {
					 
					 temp = daoshu(cin1);	 
					 result = temp;
					 temp1=temp;                
					 yflag = 1;
					 Key = 50;
					 ili9320_Text2(kongx,21,"=",black,white);
					 //*************************************************************************
					 //负数变换成正数                 
					 if(temp<0) 
					 {
						 ili9320_Text2(8+kongx,21,"-",black,white);
						 sign=1;  //增加符号位的空间
						 result=0-result;
						 temp1=0-temp;
					 }
					 if(result==0)
					 {
						 ili9320_Text2(8*(sign+1)+kongx,21,"0",black,white); 
					 }
					 re_f=0;    //结果的位数
					 res=result;
					 //计算位数
					 while(res>=1) 
					 {
						 res=res/10;
						 re_f++;
					 }
					 for(l=0;l<re_f;l++) //输出计算结果
					 {
						 RS[l]=result/(powd(10,re_f-l-1))%10+'0';
						 ili9320_PutChar2(8*(l+1+sign)+kongx,21,&RS[l],black,white);
					 }
					 
					 if((temp1-result)!=0)     //对小数进行处理
					 {
						 int z=0;
						 if(result==0){
							 ili9320_Text2(8*(l+1+sign)+kongx,21,"0",black,white);
							 z++;
						 }
						 for(q=0;q<4;q++)     //保留四位小数
						 {
							 point[q]=((long)((temp1+0.0000-result)*(powd(10,q+1))))%10+'0';
							 
							 ili9320_Text2(8*(l+1+sign+z)+kongx,21,".",black,white);
							 ili9320_PutChar2(8*(l+sign+2+q+z)+kongx,21,&point[q],black,white);
						 }
					 }
				 }
				 if(yunflag==1){
				   temp = daoshu(temp);
					 result = temp;
					 temp1=temp;                
					 yflag = 1;
					 Key = 50;
					 ili9320_Text2(kongx,21,"                               ",white,white);
				   ili9320_Text2(kongx,5,"                                  ",white,white); 
					 ili9320_Text2(kongx,21,"=",black,white);
					 //*************************************************************************
					 //负数变换成正数                 
					 if(temp<0) 
					 {
						 ili9320_Text2(8+kongx,21,"-",black,white);
						 sign=1;  //增加符号位的空间
						 result=0-result;
						 temp1=0-temp;
					 }
					 if(result==0)
					 {
						 ili9320_Text2(8*(sign+1)+kongx,21,"0",black,white); 
					 }
					 re_f=0;    //结果的位数
					 res=result;
					 //计算位数
					 while(res>=1) 
					 {
						 res=res/10;
						 re_f++;
					 }
					 for(l=0;l<re_f;l++) //输出计算结果
					 {
						 RS[l]=result/(powd(10,re_f-l-1))%10+'0';
						 ili9320_PutChar2(8*(l+1+sign)+kongx,21,&RS[l],black,white);
					 }
					 
					 if((temp1-result)!=0)     //对小数进行处理
					 {
						 int z=0;
						 if(result==0){
							 ili9320_Text2(8*(l+1+sign)+kongx,21,"0",black,white);
							 z++;
						 }
						 for(q=0;q<4;q++)     //保留四位小数
						 {
							 point[q]=((long)((temp1+0.0000-result)*(powd(10,q+1))))%10+'0';
							 
							 ili9320_Text2(8*(l+1+sign+z)+kongx,21,".",black,white);
							 ili9320_PutChar2(8*(l+sign+2+q+z)+kongx,21,&point[q],black,white);
						 }
					 }
				 }
				 //****************************************
				// yflag = 1;
				 //Key = 50;
				 // yunflag=0;
				 
			 } 
			 //*****************************************************************************
			 
			 
			 //***********************************   
			 else
			 {
				 ;
			 }
			 
			 delay_ms(1000);  
     }
	 
	 
}


//---------------------------EXT_PH3中断函数------------------------------------
//
//
//
//---------------------------EXT_PH3中断函数------------------------------------
#pragma CODE_SEG __NEAR_SEG NON_BANKED 
void interrupt 25 EXT_PH3(void) 
{
	PIFH_PIFH3=1;     //清除标志位
	PIEH_PIEH3=0;     //关闭外部中断
	Touch();
	
} 
#pragma CODE_SEG DEFAULT


//**************************************************************************
//
//定时器PIT0的终端服务子程序
//
//**************************************************************************
#pragma CODE_SEG __NEAR_SEG NON_BANKED 
void interrupt 66  PIT0_ISR(void)  
{   
	
    byte i1;
    uchar Flag;
    uint key_num;    
    PITTF_PTF0=1;    //清除通道0中断标志  
    PIEH_PIEH3=1;    //开启外部中断
    
    //是否有触屏
    if(touchflag==1)//查看是否有触摸操作发生，为1时表示有，为0没有
    {
		touchflag=0; //把触摸发生标志位清零
		//delay_ms(100);
		
		for(i1=0;i1<22;i1++) //循环20次检查是触摸了哪个音符
		{
			if(touch_select(main_selectxy[i1][0],main_selectxy[i1][1],55,40,touchx,touchy))     //320*240
			{
				Key=i1;
				break;  
			} 
			else if(touchx>=65&&touchx<=125&&touchy>=37&&touchy<=70)
			{
				Key=10; break;
			}
            else if(touchx>=65&&touchx<=125&&touchy>=210)
			{
				Key=0; break;
			}        
			else if(touchx>=205&&touchy>=250&&touchy<=305)
			{
				Key=11;  break;
			}
			
			
			//不在范围内，或者没触屏 
			/*else 
			{ 
			Key=16;
        }   */
		}
    }
	
}

