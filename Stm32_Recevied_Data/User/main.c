/**
  ******************************************************************************
  * @file    main.c
  * @author  fire
  * @version V1.0
  * @date    2015-xx-xx
  * @brief   LTDC-液晶显示英文，字库在内部FLASH
  ******************************************************************************
  * @attention
  *
  * 实验平台:秉火  STM32 F429 开发板  
  * 论坛    :http://www.firebbs.cn
  * 淘宝    :https://fire-stm32.taobao.com
  *
  ******************************************************************************
  */
  
#include "stm32f4xx.h"
#include "./led/bsp_led.h"
#include "./sdram/bsp_sdram.h"
#include "./lcd/bsp_lcd.h"
#include <string.h>
#include "./usart/bsp_debug_usart.h"

void Delay(__IO u32 nCount); 
void LCD_Test(void);
/**
  * @brief  主函数
  * @param  无
  * @retval 无
  */
int main(void)
{
	char ch;
	int  a1;
	int  b1;
	int  c1;
	int  a2;
	int  b2;
	int  c2;
	
//  int m1,m2,m3;
//	int n1,n2,n3;
  char s1_1 ,s1_2,s1_3,s1_4,s1_5,s1_6,s1_7,s1_8,s1_9;
	char s2_1,s2_2,s2_3,s2_4,s2_5,s2_6;
	char s3_1,s3_2,s3_3,s3_4,s3_5,s3_6,s3_7;
	
	char c1_1,c1_2,c1_3;
	char c2_1,c2_2,c2_3,c2_4,c2_5;
	char c3_1,c3_2,c3_3,c3_4,c3_5,c3_6;
	char c4_1,c4_2,c4_3,c4_4;
	
	
	char shape1[20],shape2[20];
	char shape3[20];
	
	char color1[20],color2[20];
	char color3[20],color4[20];
//	char color5[20],color6[20];
//	char color7[20],color8[20];
		char dispBuffx[100];
	char dispBuffy[100];
//	char mx[100],ny[100];
	                                                                                                          
	/* LED 端口初始化 */
	LED_GPIO_Config();	 
  
  /*初始化液晶屏*/
  LCD_Init();
  LCD_LayerInit();
  LTDC_Cmd(ENABLE);
	
	/*把背景层刷黑色*/
  LCD_SetLayer(LCD_BACKGROUND_LAYER);  
	LCD_Clear(LCD_COLOR_BLACK);
	
  /*初始化后默认使用前景层*/
	LCD_SetLayer(LCD_FOREGROUND_LAYER); 
	/*默认设置不透明	，该函数参数为不透明度，范围 0-0xff ，0为全透明，0xff为不透明*/
  LCD_SetTransparency(0xFF);
	LCD_Clear(LCD_COLOR_BLACK);
	/*经过LCD_SetLayer(LCD_FOREGROUND_LAYER)函数后，
	以下液晶操作都在前景层刷新，除非重新调用过LCD_SetLayer函数设置背景层*/		
	
  LED_BLUE;    

  Delay(0xfff);  
  Debug_USART_Config();
  while(1)
	{
	
//	  ch=ch-48;
//		sprintf(dispBuff,"Display value demo: testCount = %d ",ch);
//  		LCD_ClearLine(LINE(7));
//	/*然后显示该字符串即可，其它变量也是这样处理*/
// 		LCD_DisplayStringLine(LINE(7),(uint8_t* )dispBuff);
		ch=getchar();
    printf("接收到字符：%c\n",ch);
		 switch(ch)
    {
      case 'x':
      a1=getchar();
			b1=getchar();

			c1=getchar();
//			getchar();
			a1=a1-48;  //字符0的ascll码为48，将字符转化为数字
			b1=b1-48;
			c1=c1-48;
			sprintf(dispBuffx,"Display value demo: x = %d%d%d",a1,b1,c1);
		  LCD_ClearLine(LINE(7));
			
//	/*然后显示该字符串即可，其它变量也是这样处理*/
 		  LCD_DisplayStringLine(LINE(7),(uint8_t* )dispBuffx);
      break;
      case 'y':
      a2=getchar();

			b2=getchar();

			c2=getchar();
	
	  	a2=a2-48;
			b2=b2-48;
			c2=c2-48;
			sprintf(dispBuffy,"Display value demo: y = %d%d%d",a2,b2,c2);
		  LCD_ClearLine(LINE(8));
//	/*然后显示该字符串即可，其它变量也是这样处理*/
 		  LCD_DisplayStringLine(LINE(8),(uint8_t* )dispBuffy);			
      break;
			case 'r':
	
			 s1_1 = getchar();
			 Delay(10);
			 s1_2 = getchar();
			 Delay(10);
			 s1_3 = getchar();
			 Delay(10);
			 s1_4 = getchar();
			 Delay(10);
			 s1_5 = getchar();
			 Delay(10);
			 s1_6 = getchar();
			 Delay(10);
			 s1_7 = getchar();
			 Delay(10);
			 s1_8 = getchar();
			 Delay(10);
			 s1_9 = getchar();
			 Delay(50);
			sprintf(shape1,"This is: %c%c%c%c%c%c%c%c%c",s1_1 ,s1_2,s1_3,s1_4,s1_5,s1_6,s1_7,s1_8,s1_9);
			 LCD_ClearLine(LINE(9));
			 LCD_DisplayStringLine(LINE(9),(uint8_t* )shape1);			
			break;
			
		case 'c':
			 s2_1 = getchar();
		    Delay(10);
			 s2_2 = getchar();
		   Delay(10);
			 s2_3 = getchar();
		   Delay(10);
			 s2_4 = getchar();
		   Delay(10);
			 s2_5 = getchar();
		   Delay(10);
		   s2_6 = getchar();
		   Delay(50);
		//	 s1_6 = getchar();
		//	 s1_7 = getchar();
		//	 s1_8 = getchar();
		//	 s1_9 = getchar();
			
			 sprintf(shape2,"This is: %c%c%c%c%c%c",s2_1,s2_2,s2_3,s2_4,s2_5,s2_6);
			 LCD_ClearLine(LINE(9));
			 LCD_DisplayStringLine(LINE(9),(uint8_t* )shape2);			
			break;	
			case 't':
			 s3_1 = getchar();
		    Delay(10);
			 s3_2 = getchar();
		   Delay(10);
			 s3_3 = getchar();
		   Delay(10);
			 s3_4 = getchar();
		   Delay(10);
			 s3_5 = getchar();
		   Delay(10);
		   s3_6 = getchar();
			 Delay(10);
			 s3_7 = getchar();
		   Delay(50);
		//	 s1_6 = getchar();
		//	 s1_7 = getchar();
		//	 s1_8 = getchar();
		//	 s1_9 = getchar();
			
			 sprintf(shape3,"This is: %c%c%c%c%c%c%c",s3_1,s3_2,s3_3,s3_4,s3_5,s3_6,s3_7);
			 LCD_ClearLine(LINE(9));
			 LCD_DisplayStringLine(LINE(9),(uint8_t* )shape3);			
			break;	
		case 'e':
			 c1_1 = getchar();
		   Delay(10);
			 c1_2 = getchar();
		   Delay(10);
			 c1_3 = getchar();
		   Delay(50);
		//	 s2_4 = getchar();
		//	 s2_5 = getchar();
		//	 s1_6 = getchar();
		//	 s1_7 = getchar();
		//	 s1_8 = getchar();
		//	 s1_9 = getchar();
			
			 sprintf(color1,"This color is: %c%c%c",c1_1,c1_2,c1_3);
			 LCD_ClearLine(LINE(10));
			 LCD_DisplayStringLine(LINE(10),(uint8_t* )color1);			
			break;	
	case 'g':
			 c2_1 = getchar();
		   Delay(10);
			 c2_2 = getchar();
		   Delay(10);	
			 c2_3 = getchar();
			  Delay(10);
			 c2_4 = getchar();
			   Delay(10);
			 c2_5 = getchar();
			   Delay(50);
		//	 s1_6 = getchar();
		//	 s1_7 = getchar();
		//	 s1_8 = getchar();
		//	 s1_9 = getchar();
			
			 sprintf(color2,"This color is: %c%c%c%c%c",c2_1,c2_2,c2_3,c2_4,c2_5);
			 LCD_ClearLine(LINE(10));
			 LCD_DisplayStringLine(LINE(10),(uint8_t* )color2);			
			break;	
		
		case 'w':
			 c3_1 = getchar();
				  Delay(10);
		
			 c3_2 = getchar();
				  Delay(10);
			 c3_3 = getchar();
				  Delay(10);
			 c3_4 = getchar();
				  Delay(10);
			 c3_5 = getchar();
				  Delay(10);
			 c3_6 = getchar();
				  Delay(50);
		//	 s1_7 = getchar();
		//	 s1_8 = getchar();
		//	 s1_9 = getchar();
			
			 sprintf(color3,"This color is: %c%c%c%c%c%c", c3_1,c3_2,c3_3,c3_4,c3_5,c3_6);
			 LCD_ClearLine(LINE(10));
			 LCD_DisplayStringLine(LINE(10),(uint8_t* )color3);			
			break;
		
		case 'b':
			 c4_1 = getchar();
				  Delay(10);
			 c4_2 = getchar();
				  Delay(10);
			 c4_3 = getchar();
				  Delay(10);
			 c4_4 = getchar(); 
		//	 s2_5 = getchar();
		//	 s1_6 = getchar();
		//	 s1_7 = getchar();
		//	 s1_8 = getchar();
		//	 s1_9 = getchar();
			
			 sprintf(color4,"This color is: %c%c%c%c", c4_1,c4_2,c4_3,c4_4);
			 LCD_ClearLine(LINE(10));
			 LCD_DisplayStringLine(LINE(10),(uint8_t* )color4);			
			break;	
//		 case 'm':
//      m1=getchar();

//			m2=getchar();
////			getchar();
//			m3=getchar();
////			getchar();
//			m1=m1-48;  //字符0的ascll码为48，将字符转化为数字
//			m2=m2-48;
//			m3=m3-48;
//			sprintf(mx,"Display value demo: mx = %d%d%d",m1,m2,m3);
//		  LCD_ClearLine(LINE(11));
//			
////	/*然后显示该字符串即可，其它变量也是这样处理*/
// 		  LCD_DisplayStringLine(LINE(11),(uint8_t* )mx);
//      break;
//      case 'n':
//      n1=getchar();

//			n2=getchar();

//			n3=getchar();
//	
//	  	n1=n1-48;
//			n2=n2-48;
//			n3=n3-48;
//			sprintf(ny,"Display value demo: ny = %d%d%d",n1,n2,n3);
//		  LCD_ClearLine(LINE(12));
////	/*然后显示该字符串即可，其它变量也是这样处理*/
// 		  LCD_DisplayStringLine(LINE(12),(uint8_t* )ny);			
//      break;
			default:
      break; 
		}
	}  

}



/*用于测试各种液晶的函数*/
void LCD_Test(void)
{
	/*演示显示变量*/
	static uint8_t testCNT = 0;	
	char dispBuff[100];
	
	testCNT++;
	
	/*使用不透明前景层*/
	LCD_SetLayer(LCD_FOREGROUND_LAYER);  
  LCD_SetTransparency(0xff);
	
  LCD_Clear(LCD_COLOR_BLACK);	/* 清屏，显示全黑 */

	/*设置字体颜色及字体的背景颜色(此处的背景不是指LCD的背景层！注意区分)*/
  LCD_SetColors(LCD_COLOR_WHITE,LCD_COLOR_BLACK);
	
	/*选择字体*/
  LCD_SetFont(&Font16x24);

  LCD_DisplayStringLine(LINE(1),(uint8_t* )"BH 5.0 inch LCD para:");
  LCD_DisplayStringLine(LINE(2),(uint8_t* )"Image resolution:800x480 px");
  LCD_DisplayStringLine(LINE(3),(uint8_t* )"Touch pad:5 point touch supported");
  LCD_DisplayStringLine(LINE(4),(uint8_t* )"Use STM32-LTDC directed driver,");
	LCD_DisplayStringLine(LINE(5),(uint8_t* )"no extern lcd driver needed,RGB888,24bits data bus");
  LCD_DisplayStringLine(LINE(6),(uint8_t* )"Touch pad use IIC to communicate");
  
	/*使用c标准库把变量转化成字符串*/
	sprintf(dispBuff,"Display value demo: testCount = %d ",testCNT);
	LCD_ClearLine(LINE(7));
	
	/*然后显示该字符串即可，其它变量也是这样处理*/
	LCD_DisplayStringLine(LINE(7),(uint8_t* )dispBuff);


//  /* 画直线 */
//  LCD_SetTextColor(LCD_COLOR_BLUE);
//  
//	LCD_ClearLine(LINE(8));
//  LCD_DisplayStringLine(LINE(8),(uint8_t* )"Draw line:");
//  
//	LCD_SetColors(LCD_COLOR_RED,LCD_COLOR_BLACK);
//  LCD_DrawUniLine(50,250,750,250);  
//  LCD_DrawUniLine(50,300,750,300);
//  
//	LCD_SetColors(LCD_COLOR_GREEN,LCD_COLOR_BLACK);
//  LCD_DrawUniLine(300,250,400,400);  
//  LCD_DrawUniLine(600,250,600,400);
//  
//  Delay(0xFFFFFF);
//  
//	LCD_SetColors(LCD_COLOR_BLACK,LCD_COLOR_BLACK);
//  LCD_DrawFullRect(0,200,LCD_PIXEL_WIDTH,LCD_PIXEL_HEIGHT-200);
//  
//  
//  /*画矩形*/
//  LCD_SetTextColor(LCD_COLOR_BLUE);
//	LCD_ClearLine(LINE(8));
//  LCD_DisplayStringLine(LINE(8),(uint8_t* )"Draw Rect:");
//	
//	LCD_SetColors(LCD_COLOR_RED,LCD_COLOR_BLACK);
//  LCD_DrawRect(200,250,200,100);
//	
//	LCD_SetColors(LCD_COLOR_GREEN,LCD_COLOR_BLACK);
//  LCD_DrawRect(350,250,200,50);
//	
//	LCD_SetColors(LCD_COLOR_BLUE,LCD_COLOR_BLACK);
//  LCD_DrawRect(200,350,50,200);
//  
//  Delay(0xFFFFFF);
//  
//  
//	LCD_SetColors(LCD_COLOR_BLACK,LCD_COLOR_BLACK);
//  LCD_DrawFullRect(0,200,LCD_PIXEL_WIDTH,LCD_PIXEL_HEIGHT-200);
//  

  /*填充矩形*/
//  LCD_SetTextColor(LCD_COLOR_BLUE);
//	LCD_ClearLine(LINE(8));
//	LCD_DisplayStringLine(LINE(8),(uint8_t* )"Draw Full Rect:");
// 	LCD_SetColors(LCD_COLOR_RED,LCD_COLOR_BLACK);
//  LCD_DrawFullRect(200,250,200,100);
//	
//	LCD_SetColors(LCD_COLOR_GREEN,LCD_COLOR_BLACK);  LCD_DrawFullRect(350,250,200,50);
//	
//  LCD_SetColors(LCD_COLOR_BLUE,LCD_COLOR_BLACK);
//  LCD_DrawFullRect(200,350,50,200);
//  
//  Delay(0xFFFFFF);
//  
//	LCD_SetColors(LCD_COLOR_BLACK,LCD_COLOR_BLACK);
//  LCD_DrawFullRect(0,200,LCD_PIXEL_WIDTH,LCD_PIXEL_HEIGHT-200);
//	
//  /* 画圆 */
//	LCD_SetTextColor(LCD_COLOR_BLUE);
//	LCD_ClearLine(LINE(8));
//	LCD_DisplayStringLine(LINE(8),(uint8_t* )"Draw circle:");
//  
//	LCD_SetColors(LCD_COLOR_RED,LCD_COLOR_RED);
//  LCD_DrawCircle(200,350,50);
//	
//	LCD_SetColors(LCD_COLOR_GREEN,LCD_COLOR_GREEN);
//  LCD_DrawCircle(350,350,75);
//  
//  Delay(0xFFFFFF);
//  
//	LCD_SetColors(LCD_COLOR_BLACK,LCD_COLOR_BLACK);
//  LCD_DrawFullRect(0,200,LCD_PIXEL_WIDTH,LCD_PIXEL_HEIGHT-200);


//  /*填充圆*/
//	LCD_SetTextColor(LCD_COLOR_BLUE);
//	LCD_ClearLine(LINE(8));
//	LCD_DisplayStringLine(LINE(8),(uint8_t* )"Draw full circle:");
//  
//	LCD_SetColors(LCD_COLOR_RED,LCD_COLOR_BLACK);
//  LCD_DrawFullCircle(300,350,50);
//	
//	LCD_SetColors(LCD_COLOR_GREEN,LCD_COLOR_BLACK);
//  LCD_DrawFullCircle(450,350,75);
//  
//  Delay(0xFFFFFF);
//  
//	LCD_SetColors(LCD_COLOR_BLACK,LCD_COLOR_BLACK);
//  LCD_DrawFullRect(0,200,LCD_PIXEL_WIDTH,LCD_PIXEL_HEIGHT-200);
//	
//	LCD_ClearLine(LINE(8));
//	
////这种操作效果会有花屏现象，不推荐使用。一般背景混合使用ARGB格式的颜色来混合	
////	/*透明效果 前景层操作*/
////	{
////		LCD_SetTextColor(LCD_COLOR_BLUE);
////		LCD_ClearLine(LINE(8));
////		LCD_DisplayStringLine(LINE(8),(uint8_t* )"Transparency effect:");
////		
////		/*设置前景层不透明度*/
////		LCD_SetLayer(LCD_FOREGROUND_LAYER); 	
////		LCD_SetTransparency(200);
////		
////		/*在前景画一个红色圆*/
////		LCD_SetColors(LCD_COLOR_RED,LCD_COLOR_BLACK);
////		LCD_DrawFullCircle(400,350,75);
////	
////	}
////	
////	/*透明效果 背景层操作*/
////	{		
////		/*设置背景层不透明*/
////		LCD_SetLayer(LCD_BACKGROUND_LAYER); 	
////		LCD_SetTransparency(0xff);
////		LCD_Clear(LCD_COLOR_BLACK);

////		/*在背景画一个绿色圆*/
////		LCD_SetColors(LCD_COLOR_GREEN,LCD_COLOR_BLACK);
////		LCD_DrawFullCircle(450,350,75);
////		
////		/*在背景画一个蓝色圆*/
////		LCD_SetColors(LCD_COLOR_BLUE,LCD_COLOR_BLACK);
////		LCD_DrawFullCircle(350,350,75);
////	}
//  
  Delay(0xFFFFFF);
	
	LCD_SetColors(LCD_COLOR_BLACK,LCD_COLOR_BLACK);
  LCD_DrawFullRect(0,200,LCD_PIXEL_WIDTH,LCD_PIXEL_HEIGHT-200);


}

void Delay(__IO uint32_t nCount)	 //简单的延时函数
{
	for(; nCount != 0; nCount--);
}
/*********************************************END OF FILE**********************/

