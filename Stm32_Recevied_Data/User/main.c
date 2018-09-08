/**
  ******************************************************************************
  * @file    main.c
  * @author  fire
  * @version V1.0
  * @date    2015-xx-xx
  * @brief   LTDC-Һ����ʾӢ�ģ��ֿ����ڲ�FLASH
  ******************************************************************************
  * @attention
  *
  * ʵ��ƽ̨:����  STM32 F429 ������  
  * ��̳    :http://www.firebbs.cn
  * �Ա�    :https://fire-stm32.taobao.com
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
  * @brief  ������
  * @param  ��
  * @retval ��
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
	                                                                                                          
	/* LED �˿ڳ�ʼ�� */
	LED_GPIO_Config();	 
  
  /*��ʼ��Һ����*/
  LCD_Init();
  LCD_LayerInit();
  LTDC_Cmd(ENABLE);
	
	/*�ѱ�����ˢ��ɫ*/
  LCD_SetLayer(LCD_BACKGROUND_LAYER);  
	LCD_Clear(LCD_COLOR_BLACK);
	
  /*��ʼ����Ĭ��ʹ��ǰ����*/
	LCD_SetLayer(LCD_FOREGROUND_LAYER); 
	/*Ĭ�����ò�͸��	���ú�������Ϊ��͸���ȣ���Χ 0-0xff ��0Ϊȫ͸����0xffΪ��͸��*/
  LCD_SetTransparency(0xFF);
	LCD_Clear(LCD_COLOR_BLACK);
	/*����LCD_SetLayer(LCD_FOREGROUND_LAYER)������
	����Һ����������ǰ����ˢ�£��������µ��ù�LCD_SetLayer�������ñ�����*/		
	
  LED_BLUE;    

  Delay(0xfff);  
  Debug_USART_Config();
  while(1)
	{
	
//	  ch=ch-48;
//		sprintf(dispBuff,"Display value demo: testCount = %d ",ch);
//  		LCD_ClearLine(LINE(7));
//	/*Ȼ����ʾ���ַ������ɣ���������Ҳ����������*/
// 		LCD_DisplayStringLine(LINE(7),(uint8_t* )dispBuff);
		ch=getchar();
    printf("���յ��ַ���%c\n",ch);
		 switch(ch)
    {
      case 'x':
      a1=getchar();
			b1=getchar();

			c1=getchar();
//			getchar();
			a1=a1-48;  //�ַ�0��ascll��Ϊ48�����ַ�ת��Ϊ����
			b1=b1-48;
			c1=c1-48;
			sprintf(dispBuffx,"Display value demo: x = %d%d%d",a1,b1,c1);
		  LCD_ClearLine(LINE(7));
			
//	/*Ȼ����ʾ���ַ������ɣ���������Ҳ����������*/
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
//	/*Ȼ����ʾ���ַ������ɣ���������Ҳ����������*/
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
//			m1=m1-48;  //�ַ�0��ascll��Ϊ48�����ַ�ת��Ϊ����
//			m2=m2-48;
//			m3=m3-48;
//			sprintf(mx,"Display value demo: mx = %d%d%d",m1,m2,m3);
//		  LCD_ClearLine(LINE(11));
//			
////	/*Ȼ����ʾ���ַ������ɣ���������Ҳ����������*/
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
////	/*Ȼ����ʾ���ַ������ɣ���������Ҳ����������*/
// 		  LCD_DisplayStringLine(LINE(12),(uint8_t* )ny);			
//      break;
			default:
      break; 
		}
	}  

}



/*���ڲ��Ը���Һ���ĺ���*/
void LCD_Test(void)
{
	/*��ʾ��ʾ����*/
	static uint8_t testCNT = 0;	
	char dispBuff[100];
	
	testCNT++;
	
	/*ʹ�ò�͸��ǰ����*/
	LCD_SetLayer(LCD_FOREGROUND_LAYER);  
  LCD_SetTransparency(0xff);
	
  LCD_Clear(LCD_COLOR_BLACK);	/* ��������ʾȫ�� */

	/*����������ɫ������ı�����ɫ(�˴��ı�������ָLCD�ı����㣡ע������)*/
  LCD_SetColors(LCD_COLOR_WHITE,LCD_COLOR_BLACK);
	
	/*ѡ������*/
  LCD_SetFont(&Font16x24);

  LCD_DisplayStringLine(LINE(1),(uint8_t* )"BH 5.0 inch LCD para:");
  LCD_DisplayStringLine(LINE(2),(uint8_t* )"Image resolution:800x480 px");
  LCD_DisplayStringLine(LINE(3),(uint8_t* )"Touch pad:5 point touch supported");
  LCD_DisplayStringLine(LINE(4),(uint8_t* )"Use STM32-LTDC directed driver,");
	LCD_DisplayStringLine(LINE(5),(uint8_t* )"no extern lcd driver needed,RGB888,24bits data bus");
  LCD_DisplayStringLine(LINE(6),(uint8_t* )"Touch pad use IIC to communicate");
  
	/*ʹ��c��׼��ѱ���ת�����ַ���*/
	sprintf(dispBuff,"Display value demo: testCount = %d ",testCNT);
	LCD_ClearLine(LINE(7));
	
	/*Ȼ����ʾ���ַ������ɣ���������Ҳ����������*/
	LCD_DisplayStringLine(LINE(7),(uint8_t* )dispBuff);


//  /* ��ֱ�� */
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
//  /*������*/
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

  /*������*/
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
//  /* ��Բ */
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


//  /*���Բ*/
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
////���ֲ���Ч�����л������󣬲��Ƽ�ʹ�á�һ�㱳�����ʹ��ARGB��ʽ����ɫ�����	
////	/*͸��Ч�� ǰ�������*/
////	{
////		LCD_SetTextColor(LCD_COLOR_BLUE);
////		LCD_ClearLine(LINE(8));
////		LCD_DisplayStringLine(LINE(8),(uint8_t* )"Transparency effect:");
////		
////		/*����ǰ���㲻͸����*/
////		LCD_SetLayer(LCD_FOREGROUND_LAYER); 	
////		LCD_SetTransparency(200);
////		
////		/*��ǰ����һ����ɫԲ*/
////		LCD_SetColors(LCD_COLOR_RED,LCD_COLOR_BLACK);
////		LCD_DrawFullCircle(400,350,75);
////	
////	}
////	
////	/*͸��Ч�� ���������*/
////	{		
////		/*���ñ����㲻͸��*/
////		LCD_SetLayer(LCD_BACKGROUND_LAYER); 	
////		LCD_SetTransparency(0xff);
////		LCD_Clear(LCD_COLOR_BLACK);

////		/*�ڱ�����һ����ɫԲ*/
////		LCD_SetColors(LCD_COLOR_GREEN,LCD_COLOR_BLACK);
////		LCD_DrawFullCircle(450,350,75);
////		
////		/*�ڱ�����һ����ɫԲ*/
////		LCD_SetColors(LCD_COLOR_BLUE,LCD_COLOR_BLACK);
////		LCD_DrawFullCircle(350,350,75);
////	}
//  
  Delay(0xFFFFFF);
	
	LCD_SetColors(LCD_COLOR_BLACK,LCD_COLOR_BLACK);
  LCD_DrawFullRect(0,200,LCD_PIXEL_WIDTH,LCD_PIXEL_HEIGHT-200);


}

void Delay(__IO uint32_t nCount)	 //�򵥵���ʱ����
{
	for(; nCount != 0; nCount--);
}
/*********************************************END OF FILE**********************/

