#include "led.h"
#include "delay.h"
#include "key.h"
#include "sys.h"
#include "usart.h"
#include "timer.h"
#include "usart3.h"
#include "dht11.h"
#include "lcd.h"
#include "gizwits_product.h" 
 #include	"beep.h"
#include "dht11.h"
#include "fire.h"
#include "c2h5oh.h"
/************************************************
 ALIENTEK战舰V3\精英STM32开发板
 机智云-IOT DHT11温湿度检测报警实验
 技术支持：www.openedv.com
 淘宝店铺：http://eboard.taobao.com 
 关注微信公众平台微信号："正点原子"，免费获取STM32资料。
 广州市星翼电子科技有限公司  
 作者：正点原子 @ALIENTEK
************************************************/


/* 用户区当前设备状态结构体*/
dataPoint_t currentDataPoint;

//WIFI连接状态
//wifi_sta 0: 断开
//         1: 已连接
u8 wifi_sta=1;

//协议初始化
void Gizwits_Init(void)
{	
	TIM3_Int_Init(9,7199);//1MS系统定时
    usart3_init(9600);//WIFI初始化
	memset((uint8_t*)&currentDataPoint, 0, sizeof(dataPoint_t));//设备状态结构体初始化
	gizwitsInit();//缓冲区初始化
}


#define Temp_alarm  32   //温度上限（单位：C）
#define Humi_alarm  90   //湿度上限（单位：%）
#define alarm_num 3  //报警次数


//数据采集
void userHandle(void)
{
    static u8 t=0;
	static u8 temp,hum;
    static u8 temp_num=0;
	static u8 humi_num=0;
	if(wifi_sta)
	{
		 if(t==10)//每2S读取一次
		 {
			 t=0;
			 //
			 //
			 //fire_sensor
			 
			 currentDataPoint.valuefire_sensor=PFin(0);
			 
			 //
			  currentDataPoint.valueC2H5OH=PFin(2);
			 
			 DHT11_Read_Data(&temp,&hum);//读取DHT11传感器
			 LCD_ShowxNum(70,150+60+10,temp,3,16,0); 
			 LCD_ShowxNum(70,175+60+10,hum,3,16,0); 
			 currentDataPoint.valuetemp = temp ;//温度数据
			 currentDataPoint.valuehumi = hum;//湿度数据 
			 if(temp>=Temp_alarm)//温度>报警值 
			 {  
				 temp_num++;
				 if(temp_num>=alarm_num)//报警达到一定次数
				 {
					 temp_num=0;
					 currentDataPoint.valuetemp_alert=1;//温度达上限报警
				     LCD_Fill(120,150+60+10,140,165+60+10,RED);
				 }else
                 {
					 if(currentDataPoint.valuetemp_alert==0)
					  LCD_Fill(120,150+60+10,140,165+60+10,GREEN);
				 }  
				 
			 }else  
			 {
				 currentDataPoint.valuetemp_alert=0;
				 LCD_Fill(120,150+60+10,140,165+60+10,GREEN);
				 
			 }
			 if(hum>=Humi_alarm)//湿度>报警值
			 {
				 humi_num++;
				 if(humi_num>=alarm_num)//报警达到一定次数
				 {
					 humi_num=0;
					 currentDataPoint.valuehumi_alert=1;//湿度达上限报警
				     LCD_Fill(120,175+60+10,140,190+60+10,RED);
				 }
				 
			 }else
			 { 
				 if(currentDataPoint.valuetemp_alert==0)
				   LCD_Fill(120,175+60+10,140,190+60+10,GREEN);
			 }
			 if(!PFin(0))
		{
		PBout(8)=1;
		delay_ms(500);
		PBout(8)=0;
			delay_ms(500);
		}
		if(!PFin(2))
		{
		PBout(8)=1;
		delay_ms(500);
		PBout(8)=0;
			delay_ms(500);
		}
		 }
		 if(t%2)  LED0=!LED0;
		 
		  t++;
	}
	else
	{
		if(temp_num!=0||humi_num!=0) 
		{
			temp_num=0;humi_num=0;
		}
	}
	
	
}
//主函数
 int main(void)
 {		
    int key;
	u8 wifi_con=0;//记录wifi连接状态 1:连接 0:断开
	u8 buff[20]={0};
	delay_init();	    	 //延时函数初始化	  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //设置NVIC中断分组2:2位抢占优先级，2位响应优先级
	uart_init(115200);	    //串口初始化为115200
 	LED_Init();			    //LED端口初始化
	KEY_Init();             //按键初始化  
	LCD_Init();			   	//初始化LCD 
	fire_init();
	c2h5oh_init();
	BEEP_init();
	Gizwits_Init();         //协议初始化
//	POINT_COLOR=RED;		//设置字体为红色 
//	LCD_ShowString(30,50,200,16,16,"WarShip STM32");	
//	LCD_ShowString(30,70,200,16,16,"IOT DHT11 TEST");	
//	LCD_ShowString(30,90,200,16,16,"ATOM@ALIENTEK");
//	LCD_ShowString(30,110,200,16,16,"2017/6/1");
	POINT_COLOR=BLUE;		//设置字体为蓝色 
	while(DHT11_Init())	//DHT11初始化	
	{
		LCD_ShowString(30,130,200,16,16,"DHT11 Error");
		delay_ms(200);
		LCD_Fill(30,130,239,130+16,WHITE);
 		delay_ms(200);
	}
	LCD_ShowString(30,130,200,16,16,"DHT11 OK");
    sprintf((char*)buff,"Temp alarm: %dC",Temp_alarm);
	LCD_ShowString(30,150,200,16,16,buff);
	memset(buff,0x00,20);
	sprintf((char*)buff,"Humi alarm: %d%%",Humi_alarm);
	LCD_ShowString(30,150+20,200,16,16,buff);
	POINT_COLOR=RED;		//设置字体为红色
//	LCD_ShowString(30,150+40+10,200,16,16,"wifi: close   ");
	POINT_COLOR=BLUE;		//设置字体为蓝色
	LCD_ShowString(30,150+60+10,200,16,16,"Temp:  0C");
    LCD_ShowString(30,175+60+10,200,16,16,"Humi:  0%");	
	printf("--------机智云IOT-DHT11温湿度检测报警实验----------\r\n");
	printf("KEY1:AirLink连接模式\t KEY_UP:复位\r\n\r\n");
	
   	while(1)
	{		
	  
	   userHandle();//用户采集
         
       gizwitsHandle((dataPoint_t *)&currentDataPoint);//协议处理
 		
	    key = KEY_Scan(0);
		if(key==KEY1_PRES)//KEY1按键
		{
			printf("WIFI进入AirLink连接模式\r\n");
			gizwitsSetMode(WIFI_AIRLINK_MODE);//Air-link模式接入
		}			
		if(key==WKUP_PRES)//KEY_UP按键
		{  
			printf("WIFI复位，请重新配置连接\r\n");
			gizwitsSetMode(WIFI_RESET_MODE);//WIFI复位
			LCD_Fill(120,150+60+10,140,165+60+10,WHITE);
			LCD_Fill(120,175+60+10,140,190+60+10,WHITE);
			POINT_COLOR=BLUE;		//设置字体为蓝色 
			LCD_ShowxNum(70,150+60+10,0,3,16,0); 
			LCD_ShowxNum(70,175+60+10,0,3,16,0); 
			wifi_sta=0;//标志wifi已断开
			LED0=1;
		}
		delay_ms(200);
		LED1=!LED1;
	}	 

} 
 
