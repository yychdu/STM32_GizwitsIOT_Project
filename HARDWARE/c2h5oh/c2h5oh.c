#include "stm32f10x.h"
#include "key.h"
#include "sys.h" 
#include "delay.h"
#include "c2h5oh.h"
//////////////////////////////////////////////////////////////////////////////////	 
  
								
void c2h5oh_init(void) //IO初始化
{ 
 	GPIO_InitTypeDef GPIO_InitStructure;
 
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOF,ENABLE);//使能PORTF时钟

	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_2;//PF2---------------------------------------------------------
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //设置成下拉输入
 	GPIO_Init(GPIOF, &GPIO_InitStructure);//初始化GPIOF0


}