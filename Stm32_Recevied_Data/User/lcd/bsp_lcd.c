/**
  ******************************************************************************
  * @file    bsp_sdram.c
  * @author  fire
  * @version V1.0
  * @date    2015-xx-xx
  * @brief   LCDӦ�ú����ӿڣ�֧��RGB888/565 (����������ʾ)
  ******************************************************************************
  * @attention
  *
  * ʵ��ƽ̨:����  STM32 F429 ������  
  * ��̳    :http://www.firebbs.cn
  * �Ա�    :https://fire-stm32.taobao.com
  *
  ******************************************************************************
  */
  

/* Includes ------------------------------------------------------------------*/
#include "./lcd/bsp_lcd.h"

/** @defgroup STM32F429I_DISCOVERY_LCD_Private_Defines
  * @{
  */

#define POLY_Y(Z)          ((int32_t)((Points + Z)->X))
#define POLY_X(Z)          ((int32_t)((Points + Z)->Y))   
/**
  * @}
  */ 

/** @defgroup STM32F429I_DISCOVERY_LCD_Private_Macros
  * @{
  */
#define ABS(X)  ((X) > 0 ? (X) : -(X))    
/**
  * @}
  */ 
	

#if  LCD_RGB_888
/****************RGB888����*****************************RGB888����*****************************RGB888����**********************************RGB888����**********************************RGB888����*********************************/
	
  
/** @defgroup STM32F429I_DISCOVERY_LCD_Private_Variables
  * @{
  */ 
/*���ڴ洢��ǰѡ��������ʽ*/
static sFONT *LCD_Currentfonts;
/* ���ڴ洢��ǰ������ɫ�����屳����ɫ�ı���*/
static uint32_t CurrentTextColor   = 0x000000;
static uint32_t CurrentBackColor   = 0xFFFFFF;
/* ���ڴ洢���Ӧ���Դ�ռ� �� ��ǰѡ��Ĳ�*/
static uint32_t CurrentFrameBuffer = LCD_FRAME_BUFFER;
static uint32_t CurrentLayer = LCD_BACKGROUND_LAYER;
/**
  * @}
  */ 

/** @defgroup STM32F429I_DISCOVERY_LCD_Private_FunctionPrototypes
  * @{
  */ 


static void LCD_PolyLineRelativeClosed(pPoint Points, uint16_t PointCount, uint16_t Closed);
static void LCD_GPIO_Config(void);



/**
  * @}
  */ 
/*ʵ�ʲ��Կ��õĽϿ������(5��)*/
//#define HBP  24		//HSYNC�����Ч����
//#define VBP   3		//VSYNC�����Ч����

//#define HSW   1		//HSYNC���
//#define VSW   1		//VSYNC���

//#define HFP  10		//HSYNCǰ����Ч����
//#define VFP   4		//VSYNCǰ����Ч����

/*����Һ�������ֲ�Ĳ�������*/
#define HBP  46		//HSYNC�����Ч����
#define VBP  23		//VSYNC�����Ч����

#define HSW   1		//HSYNC���
#define VSW   1		//VSYNC���

#define HFP  20		//HSYNCǰ����Ч����
#define VFP   22		//VSYNCǰ����Ч����


/**
  * @brief LCD��������
  * @note  ���������������LTDC����:
  *        1)��������ͬ��ʱ��CLK
  *        2)����LTDCʱ��������źż���
  * @retval  None 
  */
void LCD_Init(void)
{ 
  LTDC_InitTypeDef       LTDC_InitStruct;
  
  /* ʹ��LTDC����ʱ�� */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_LTDC, ENABLE);
  
  /* ʹ��DMA2Dʱ�� */
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2D, ENABLE);
	
	/* ��ʼ��LCD�Ŀ������� */
  LCD_GPIO_Config();   
	
	/* ��ʼ��SDRAM���Ա�ʹ��SDRAM���Դ� */
  SDRAM_Init();
	
	/* ���� PLLSAI ��Ƶ�������������Ϊ����ͬ��ʱ��CLK*/
  /* PLLSAI_VCO ����ʱ�� = HSE_VALUE/PLL_M = 1 Mhz */
  /* PLLSAI_VCO ���ʱ�� = PLLSAI_VCO���� * PLLSAI_N = 416 Mhz */
  /* PLLLCDCLK = PLLSAI_VCO ���/PLLSAI_R = 420/6  Mhz */
  /* LTDC ʱ��Ƶ�� = PLLLCDCLK / DIV = 420/6/8 = 8.75 Mhz */
	/* LTDCʱ��̫�߻ᵼ����������ˢ���ٶ�Ҫ�󲻸ߣ�����ʱ��Ƶ�ʿɼ��ٻ�������*/
	/* ���º������������ֱ�Ϊ��PLLSAIN,PLLSAIQ,PLLSAIR������PLLSAIQ��LTDC�޹�*/
  RCC_PLLSAIConfig(420,7, 6);
	/*���º����Ĳ���ΪDIVֵ*/
  RCC_LTDCCLKDivConfig(RCC_PLLSAIDivR_Div8);
  
  /* ʹ�� PLLSAI ʱ�� */
  RCC_PLLSAICmd(ENABLE);
  /* �ȴ� PLLSAI ��ʼ����� */
  while(RCC_GetFlagStatus(RCC_FLAG_PLLSAIRDY) == RESET)
  {
  }
  
  /* LTDC����*********************************************************/  
  /*�źż�������*/
  /* ��ͬ���źż��� */
  LTDC_InitStruct.LTDC_HSPolarity = LTDC_HSPolarity_AL;     
  /* ��ֱͬ���źż��� */  
  LTDC_InitStruct.LTDC_VSPolarity = LTDC_VSPolarity_AL;     
  /* ����ʹ���źż��� */
  LTDC_InitStruct.LTDC_DEPolarity = LTDC_DEPolarity_AL;     
  /* ����ͬ��ʱ�Ӽ��� */ 
  LTDC_InitStruct.LTDC_PCPolarity = LTDC_PCPolarity_IPC;
  
  /* ����LCD������ɫ */                   
  LTDC_InitStruct.LTDC_BackgroundRedValue = 0;            
  LTDC_InitStruct.LTDC_BackgroundGreenValue = 0;          
  LTDC_InitStruct.LTDC_BackgroundBlueValue = 0;    
 
  /* ʱ��������� */  
 /* ������ͬ���źſ��(HSW-1) */
 LTDC_InitStruct.LTDC_HorizontalSync =HSW-1;
 /* ���ô�ֱͬ���źſ��(VSW-1) */
 LTDC_InitStruct.LTDC_VerticalSync = VSW-1;
 /* ����(HSW+HBP-1) */
 LTDC_InitStruct.LTDC_AccumulatedHBP =HSW+HBP-1;
 /* ����(VSW+VBP-1) */
 LTDC_InitStruct.LTDC_AccumulatedVBP = VSW+VBP-1;
 /* ����(HSW+HBP+��Ч���ؿ��-1) */
 LTDC_InitStruct.LTDC_AccumulatedActiveW = HSW+HBP+LCD_PIXEL_WIDTH-1;
 /* ����(VSW+VBP+��Ч���ظ߶�-1) */
 LTDC_InitStruct.LTDC_AccumulatedActiveH = VSW+VBP+LCD_PIXEL_HEIGHT-1;
 /* �����ܿ��(HSW+HBP+��Ч���ؿ��+HFP-1) */
 LTDC_InitStruct.LTDC_TotalWidth =HSW+ HBP+LCD_PIXEL_WIDTH  + HFP-1; 
 /* �����ܸ߶�(VSW+VBP+��Ч���ظ߶�+VFP-1) */
 LTDC_InitStruct.LTDC_TotalHeigh =VSW+ VBP+LCD_PIXEL_HEIGHT  + VFP-1;

  LTDC_Init(&LTDC_InitStruct);
  
  LTDC_Cmd(ENABLE);
}  

/**
  * @brief ��ʼ��LTD�� �� ����
  *           - �����Դ�ռ�
  *           - ���÷ֱ���
  * @param  None
  * @retval None
  */
void LCD_LayerInit(void)
{
  LTDC_Layer_InitTypeDef LTDC_Layer_InitStruct; 
  
  /* �㴰������ */
  /* ���ñ���Ĵ��ڱ߽磬ע����Щ�����ǰ���HBP HSW VBP VSW�� */    
	//һ�еĵ�һ����ʼ���أ��ó�ԱֵӦ��Ϊ (LTDC_InitStruct.LTDC_AccumulatedHBP+1)��ֵ
	LTDC_Layer_InitStruct.LTDC_HorizontalStart = HBP + HSW;
	//һ�е����һ�����أ��ó�ԱֵӦ��Ϊ (LTDC_InitStruct.LTDC_AccumulatedActiveW)��ֵ
	LTDC_Layer_InitStruct.LTDC_HorizontalStop = HSW+HBP+LCD_PIXEL_WIDTH-1;
	//һ�еĵ�һ����ʼ���أ��ó�ԱֵӦ��Ϊ (LTDC_InitStruct.LTDC_AccumulatedVBP+1)��ֵ
	LTDC_Layer_InitStruct.LTDC_VerticalStart =  VBP + VSW;
	//һ�е����һ�����أ��ó�ԱֵӦ��Ϊ (LTDC_InitStruct.LTDC_AccumulatedActiveH)��ֵ
	LTDC_Layer_InitStruct.LTDC_VerticalStop = VSW+VBP+LCD_PIXEL_HEIGHT-1;
	
  /* ���ظ�ʽ����*/
  LTDC_Layer_InitStruct.LTDC_PixelFormat = LTDC_Pixelformat_RGB888;
  /* �㶨Alphaֵ���ã�0-255 */
  LTDC_Layer_InitStruct.LTDC_ConstantAlpha = 255; 
  /* Ĭ�ϱ�����ɫ������ɫ�ڶ���Ĳ㴰������ڲ��ֹʱʹ�á� */          
  LTDC_Layer_InitStruct.LTDC_DefaultColorBlue = 0xFF;        
  LTDC_Layer_InitStruct.LTDC_DefaultColorGreen = 0xFF;       
  LTDC_Layer_InitStruct.LTDC_DefaultColorRed = 0xFF;         
  LTDC_Layer_InitStruct.LTDC_DefaultColorAlpha = 0xFF;
  /* ���û������ CA��ʾʹ�ú㶨Alphaֵ��PAxCA��ʾʹ������Alpha x �㶨Alphaֵ */       
  LTDC_Layer_InitStruct.LTDC_BlendingFactor_1 = LTDC_BlendingFactor1_CA;    
  LTDC_Layer_InitStruct.LTDC_BlendingFactor_2 = LTDC_BlendingFactor2_PAxCA;
  
  /* �ó�ԱӦд��(һ����������ռ�õ��ֽ���+3)
  Line Lenth = ����Ч���ظ��� x ÿ�����ص��ֽ��� + 3 
  ����Ч���ظ��� = LCD_PIXEL_WIDTH 
  ÿ�����ص��ֽ��� = 2��RGB565/RGB1555��/ 3 (RGB888)/ 4��ARGB8888�� 
  */
  LTDC_Layer_InitStruct.LTDC_CFBLineLength = ((LCD_PIXEL_WIDTH * 3) + 3);
  /* ��ĳ�е���ʼλ�õ���һ����ʼλ�ô�����������
  Pitch = ����Ч���ظ��� x ÿ�����ص��ֽ��� */ 
  LTDC_Layer_InitStruct.LTDC_CFBPitch = (LCD_PIXEL_WIDTH * 3);
  
  /* ������Ч������ */  
  LTDC_Layer_InitStruct.LTDC_CFBLineNumber = LCD_PIXEL_HEIGHT;
  
  /* ���ñ�����Դ��׵�ַ */    
  LTDC_Layer_InitStruct.LTDC_CFBStartAdress = LCD_FRAME_BUFFER;
  
  /* ����������ó�ʼ���� 1 ��*/
  LTDC_LayerInit(LTDC_Layer1, &LTDC_Layer_InitStruct);

  /*���õ� 2 �㣬��û����дĳ����Ա��ֵ����ó�Աʹ�ø���1��һ�������� */
  /* ���ñ�����Դ��׵�ַ�����������������ڵ�1��ĺ���*/     
  LTDC_Layer_InitStruct.LTDC_CFBStartAdress = LCD_FRAME_BUFFER + BUFFER_OFFSET;

	/* ���û�����ӣ�ʹ������Alpha������ */       
  LTDC_Layer_InitStruct.LTDC_BlendingFactor_1 = LTDC_BlendingFactor1_PAxCA;    
  LTDC_Layer_InitStruct.LTDC_BlendingFactor_2 = LTDC_BlendingFactor2_PAxCA;

  /* ��ʼ����2�� */
  LTDC_LayerInit(LTDC_Layer2, &LTDC_Layer_InitStruct);
 
 /* ������������ */  
 LTDC_ReloadConfig(LTDC_IMReload);

 /*ʹ��ǰ���������� */
  LTDC_LayerCmd(LTDC_Layer1, ENABLE); 
  LTDC_LayerCmd(LTDC_Layer2, ENABLE);
 
  /* ������������ */  
  LTDC_ReloadConfig(LTDC_IMReload);

  /* �趨����(Ӣ��) */    
  LCD_SetFont(&LCD_DEFAULT_FONT); 
}



/**
  * @brief  ѡ��Ҫ���ƵĲ�.
  * @param  Layerx: ѡ��Ҫ����ǰ����(��2��)���Ǳ�����(��1��)
  * @retval None
  */
void LCD_SetLayer(uint32_t Layerx)
{
  if (Layerx == LCD_BACKGROUND_LAYER)
  {
    CurrentFrameBuffer = LCD_FRAME_BUFFER; 
    CurrentLayer = LCD_BACKGROUND_LAYER;
  }
  else
  {
    CurrentFrameBuffer = LCD_FRAME_BUFFER + BUFFER_OFFSET;
    CurrentLayer = LCD_FOREGROUND_LAYER;
  }
}  

/**
  * @brief  �����������ɫ������ı�����ɫ
  * @param  TextColor: ������ɫ
  * @param  BackColor: ����ı�����ɫ
  * @retval None
  */
void LCD_SetColors(uint32_t TextColor, uint32_t BackColor) 
{
  CurrentTextColor = TextColor; 
  CurrentBackColor = BackColor;
}

/**
  * @brief ��ȡ��ǰ���õ�������ɫ������ı�����ɫ
  * @param  TextColor: ָ��������ɫ��ָ��
  * @param  BackColor: ָ�����屳����ɫ��ָ��
  * @retval None
  */
void LCD_GetColors(uint32_t *TextColor, uint32_t *BackColor)
{
  *TextColor = CurrentTextColor;
  *BackColor = CurrentBackColor;
}

/**
  * @brief  ����������ɫ
  * @param  Color: ������ɫ
  * @retval None
  */
void LCD_SetTextColor(uint32_t Color)
{
  CurrentTextColor = Color;
}

/**
  * @brief  ��������ı�����ɫ
  * @param  Color: ����ı�����ɫ
  * @retval None
  */
void LCD_SetBackColor(uint32_t Color)
{
  CurrentBackColor = Color;
}

/**
  * @brief  ���������ʽ(Ӣ��)
  * @param  fonts: ѡ��Ҫ���õ������ʽ
  * @retval None
  */
void LCD_SetFont(sFONT *fonts)
{
  LCD_Currentfonts = fonts;
}

/**
  * @brief  Configure the transparency.
  * @param  transparency: specifies the transparency, 
  *         This parameter must range from 0x00 to 0xFF.
  * @retval None
  */
void LCD_SetTransparency(uint8_t transparency)
{
  if (CurrentLayer == LCD_BACKGROUND_LAYER)
  {
    LTDC_LayerAlpha(LTDC_Layer1, transparency);
  }
  else
  {     
    LTDC_LayerAlpha(LTDC_Layer2, transparency);
  }
  LTDC_ReloadConfig(LTDC_IMReload);
}

/**
  * @brief  Gets the Text Font.
  * @param  None.
  * @retval the used font.
  */
sFONT *LCD_GetFont(void)
{
  return LCD_Currentfonts;
}

/**
  * @brief  Clears the selected line.
  * @param  Line: the Line to be cleared.
  *   This parameter can be one of the following values:
  *     @arg LCD_LINE_x: where x can be: 0..13 if LCD_Currentfonts is Font16x24
  *                                      0..26 if LCD_Currentfonts is Font12x12 or Font8x12
  *                                      0..39 if LCD_Currentfonts is Font8x8
  * @retval None
  */
void LCD_ClearLine(uint16_t Line)
{
  uint16_t refcolumn = 0;
  /* Send the string character by character on lCD */
  while ((refcolumn < LCD_PIXEL_WIDTH) && (((refcolumn + LCD_Currentfonts->Width)& 0xFFFF) >= LCD_Currentfonts->Width))
  {
    /* Display one character on LCD */
    LCD_DisplayChar(Line, refcolumn, ' ');
    /* Decrement the column position by 16 */
    refcolumn += LCD_Currentfonts->Width;
  }
}

/**
  * @brief  Clears the hole LCD.
  * @param  Color: the color of the background.
  * @retval None
  */
void LCD_Clear(uint32_t Color)
{

  DMA2D_InitTypeDef      DMA2D_InitStruct;
  
  uint16_t Red_Value = 0, Green_Value = 0, Blue_Value = 0;
  
 
  Red_Value = (0xFF0000 & Color) >> 16;
  Green_Value = (0x00FF00 & Color) >> 8;
  Blue_Value = 0x0000FF & Color;
  
  /* Configure DMA2D */    
  DMA2D_DeInit();  
  DMA2D_InitStruct.DMA2D_Mode = DMA2D_R2M;       
  DMA2D_InitStruct.DMA2D_CMode = DMA2D_RGB888;      
  DMA2D_InitStruct.DMA2D_OutputRed =Red_Value; //Red_Value;     
  DMA2D_InitStruct.DMA2D_OutputGreen =Green_Value;// Green_Value;      
  DMA2D_InitStruct.DMA2D_OutputBlue =Blue_Value;// Blue_Value;     
             
  DMA2D_InitStruct.DMA2D_OutputAlpha = 0x0F;                  
  DMA2D_InitStruct.DMA2D_OutputMemoryAdd = CurrentFrameBuffer;                  
  
  {                                                      
    DMA2D_InitStruct.DMA2D_OutputOffset = 0;                
    DMA2D_InitStruct.DMA2D_NumberOfLine = LCD_PIXEL_HEIGHT;            
    DMA2D_InitStruct.DMA2D_PixelPerLine = LCD_PIXEL_WIDTH; 
  }
  
  DMA2D_Init(&DMA2D_InitStruct);  
  /* Start Transfer */ 
  DMA2D_StartTransfer();  
  /* Wait for CTC Flag activation */
  while(DMA2D_GetFlagStatus(DMA2D_FLAG_TC) == RESET)
  {
  }  

}

/**
  * @brief  Sets the cursor position.
  * @param  Xpos: specifies the X position.
  * @param  Ypos: specifies the Y position. 
  * @retval Display Address
  */
uint32_t LCD_SetCursor(uint16_t Xpos, uint16_t Ypos)
{  
  return CurrentFrameBuffer + 3*(Xpos + (LCD_PIXEL_WIDTH*Ypos));
}

/**
  * @brief  Config and Sets the color Keying.
  * @param  RGBValue: Specifies the Color reference. 
  * @retval None
  */
void LCD_SetColorKeying(uint32_t RGBValue)
{  
  LTDC_ColorKeying_InitTypeDef   LTDC_colorkeying_InitStruct;
  
  /* configure the color Keying */
  LTDC_colorkeying_InitStruct.LTDC_ColorKeyBlue = 0x0000FF & RGBValue;
  LTDC_colorkeying_InitStruct.LTDC_ColorKeyGreen = (0x00FF00 & RGBValue);
  LTDC_colorkeying_InitStruct.LTDC_ColorKeyRed = (0xFF0000 & RGBValue) ;  

  if (CurrentLayer == LCD_BACKGROUND_LAYER)
  {   
    /* Enable the color Keying for Layer1 */
    LTDC_ColorKeyingConfig(LTDC_Layer1, &LTDC_colorkeying_InitStruct, ENABLE);
    LTDC_ReloadConfig(LTDC_IMReload);
  }
  else
  {
    /* Enable the color Keying for Layer2 */
    LTDC_ColorKeyingConfig(LTDC_Layer2, &LTDC_colorkeying_InitStruct, ENABLE);
    LTDC_ReloadConfig(LTDC_IMReload);
  }
}

/**
  * @brief  Disable the color Keying.
  * @param  RGBValue: Specifies the Color reference. 
  * @retval None
  */
void LCD_ReSetColorKeying(void)
{
  LTDC_ColorKeying_InitTypeDef   LTDC_colorkeying_InitStruct;
  
  if (CurrentLayer == LCD_BACKGROUND_LAYER)
  {   
    /* Disable the color Keying for Layer1 */
    LTDC_ColorKeyingConfig(LTDC_Layer1, &LTDC_colorkeying_InitStruct, DISABLE);
    LTDC_ReloadConfig(LTDC_IMReload);
  }
  else
  {
    /* Disable the color Keying for Layer2 */
    LTDC_ColorKeyingConfig(LTDC_Layer2, &LTDC_colorkeying_InitStruct, DISABLE);
    LTDC_ReloadConfig(LTDC_IMReload);
  }
} 

/**
  * @brief  Draws a character on LCD.
  * @param  Xpos: the Line where to display the character shape.
  * @param  Ypos: start column address.
  * @param  c: pointer to the character data.
  * @retval None
  */
void LCD_DrawChar(uint16_t Xpos, uint16_t Ypos, const uint16_t *c)
{
  uint32_t index = 0, counter = 0, xpos =0;
  uint32_t  Xaddress = 0;
  
  xpos = Xpos*LCD_PIXEL_WIDTH*3;
  Xaddress += Ypos;
  
  for(index = 0; index < LCD_Currentfonts->Height; index++)
  {
    
    for(counter = 0; counter < LCD_Currentfonts->Width; counter++)
    {
          
      if((((c[index] & ((0x80 << ((LCD_Currentfonts->Width / 12 ) * 8 ) ) >> counter)) == 0x00) &&(LCD_Currentfonts->Width <= 12))||
        (((c[index] & (0x1 << counter)) == 0x00)&&(LCD_Currentfonts->Width > 12 )))
      {
        *(__IO uint16_t*)(CurrentFrameBuffer + (3*Xaddress) + xpos) = (0x00FFFF & CurrentBackColor);        //GB
        *(__IO uint8_t*)(CurrentFrameBuffer + (3*Xaddress) + xpos+2) = (0xFF0000 & CurrentBackColor) >> 16; //R
      }
      else
      {
        *(__IO uint16_t*)(CurrentFrameBuffer + (3*Xaddress) + xpos) = (0x00FFFF & CurrentTextColor);        //GB
        *(__IO uint8_t*)(CurrentFrameBuffer + (3*Xaddress) + xpos+2) = (0xFF0000 & CurrentTextColor) >> 16; //R
      }
      Xaddress++;
    }
      Xaddress += (LCD_PIXEL_WIDTH - LCD_Currentfonts->Width);
  }
}

/**
  * @brief  Displays one character (16dots width, 24dots height).
  * @param  Line: the Line where to display the character shape .
  *   This parameter can be one of the following values:
  *     @arg Linex: where x can be 0..29
  * @param  Column: start column address.
  * @param  Ascii: character ascii code, must be between 0x20 and 0x7E.
  * @retval None
  */
void LCD_DisplayChar(uint16_t Line, uint16_t Column, uint8_t Ascii)
{
  Ascii -= 32;

  LCD_DrawChar(Line, Column, &LCD_Currentfonts->table[Ascii * LCD_Currentfonts->Height]);
}

/**
  * @brief  Displays a maximum of 20 char on the LCD.
  * @param  Line: the Line where to display the character shape .
  *   This parameter can be one of the following values:
  *     @arg Linex: where x can be 0..9
  * @param  *ptr: pointer to string to display on LCD.
  * @retval None
  */
void LCD_DisplayStringLine(uint16_t Line, uint8_t *ptr)
{  
  uint16_t refcolumn = 0;
  /* Send the string character by character on lCD */
  while ((refcolumn < LCD_PIXEL_WIDTH) && ((*ptr != 0) & (((refcolumn + LCD_Currentfonts->Width) & 0xFFFF) >= LCD_Currentfonts->Width)))
  {
    /* Display one character on LCD */
    LCD_DisplayChar(Line, refcolumn, *ptr);
    /* Decrement the column position by width */
    refcolumn += LCD_Currentfonts->Width;
    /* Point on the next character */
    ptr++;
  }
}

/**
  * @brief  Sets a display window
  * @param  Xpos: specifies the X bottom left position from 0 to 240.
  * @param  Ypos: specifies the Y bottom left position from 0 to 320.
  * @param  Height: display window height, can be a value from 0 to 320.
  * @param  Width: display window width, can be a value from 0 to 240.
  * @retval None
  */
void LCD_SetDisplayWindow(uint16_t Xpos, uint16_t Ypos, uint16_t Height, uint16_t Width)
{

  if (CurrentLayer == LCD_BACKGROUND_LAYER)
  { 
    /* reconfigure the layer1 position */
    LTDC_LayerPosition(LTDC_Layer1, Xpos, Ypos);
    LTDC_ReloadConfig(LTDC_IMReload);
    
    /* reconfigure the layer1 size */
    LTDC_LayerSize(LTDC_Layer1, Width, Height);
    LTDC_ReloadConfig(LTDC_IMReload);
 }
 else
 {   
    /* reconfigure the layer2 position */
    LTDC_LayerPosition(LTDC_Layer2, Xpos, Ypos);
    LTDC_ReloadConfig(LTDC_IMReload); 
   
   /* reconfigure the layer2 size */
    LTDC_LayerSize(LTDC_Layer2, Width, Height);
    LTDC_ReloadConfig(LTDC_IMReload);
  }
}

/**
  * @brief  Disables LCD Window mode.
  * @param  None
  * @retval None
  */
void LCD_WindowModeDisable(void)
{
  LCD_SetDisplayWindow(0, 0, LCD_PIXEL_HEIGHT, LCD_PIXEL_WIDTH); 
}

/**
  * @brief ��ʾһ��ֱ��
  * @param Xpos: ֱ������x����
  * @param Ypos: ֱ������y����
  * @param Length: ֱ�ߵĳ���
  * @param Direction: ֱ�ߵķ��򣬿�����LCD_DIR_HORIZONTAL(ˮƽ����) LCD_DIR_VERTICAL(��ֱ����).
  * @retval None
  */
void LCD_DrawLine(uint16_t Xpos, uint16_t Ypos, uint16_t Length, uint8_t Direction)
{
  DMA2D_InitTypeDef      DMA2D_InitStruct;
  
  uint32_t  Xaddress = 0;
  uint16_t Red_Value = 0, Green_Value = 0, Blue_Value = 0;
  
	/*����Ŀ���ַ*/
  Xaddress = CurrentFrameBuffer + 3*(LCD_PIXEL_WIDTH*Ypos + Xpos);
 
	/*��ȡ��ɫ����*/
  Red_Value = (0xFF0000 & CurrentTextColor) >>16;
  Blue_Value = 0x0000FF & CurrentTextColor;
  Green_Value = (0x00FF00 & CurrentTextColor)>>8 ;

  /* ����DMA2D */    
  DMA2D_DeInit();  
  DMA2D_InitStruct.DMA2D_Mode = DMA2D_R2M;       
  DMA2D_InitStruct.DMA2D_CMode = DMA2D_RGB888;      
  DMA2D_InitStruct.DMA2D_OutputGreen = Green_Value;      
  DMA2D_InitStruct.DMA2D_OutputBlue = Blue_Value;     
  DMA2D_InitStruct.DMA2D_OutputRed = Red_Value;                
  DMA2D_InitStruct.DMA2D_OutputAlpha = 0x0F;                  
  DMA2D_InitStruct.DMA2D_OutputMemoryAdd = Xaddress;                  
  
	/*ˮƽ����*/
  if(Direction == LCD_DIR_HORIZONTAL)
  {                                                      
    DMA2D_InitStruct.DMA2D_OutputOffset = 0;                
    DMA2D_InitStruct.DMA2D_NumberOfLine = 1;            
    DMA2D_InitStruct.DMA2D_PixelPerLine = Length; 
  }
  else /*��ֱ����*/
  {                                                            
    DMA2D_InitStruct.DMA2D_OutputOffset = LCD_PIXEL_WIDTH - 1;                
    DMA2D_InitStruct.DMA2D_NumberOfLine = Length;            
    DMA2D_InitStruct.DMA2D_PixelPerLine = 1;  
  }
  
  DMA2D_Init(&DMA2D_InitStruct);  
  /*��ʼDMA2D���� */ 
  DMA2D_StartTransfer();  
  /*�ȴ�������� */
  while(DMA2D_GetFlagStatus(DMA2D_FLAG_TC) == RESET)
  {
  }  
}

/**
  * @brief  Displays a rectangle.
  * @param  Xpos: specifies the X position, can be a value from 0 to 240.
  * @param  Ypos: specifies the Y position, can be a value from 0 to 320.
  * @param  Height: display rectangle height, can be a value from 0 to 320.
  * @param  Width: display rectangle width, can be a value from 0 to 240.
  * @retval None
  */
void LCD_DrawRect(uint16_t Xpos, uint16_t Ypos, uint16_t Width, uint16_t Height)
{
  /* draw horizontal lines */
  LCD_DrawLine(Xpos, Ypos, Width, LCD_DIR_HORIZONTAL);
  LCD_DrawLine(Xpos, (Ypos+ Height), Width, LCD_DIR_HORIZONTAL);
  
  /* draw vertical lines */
  LCD_DrawLine(Xpos, Ypos, Height, LCD_DIR_VERTICAL);
  LCD_DrawLine((Xpos + Width), Ypos, Height, LCD_DIR_VERTICAL);
}

/**
  * @brief  Draw a circle.
  * @param  Xpos: specifies the X position, can be a value from 0 to 240.
  * @param  Ypos: specifies the Y position, can be a value from 0 to 320.
  * @param  Radius: radius of the circle.
  * @retval None
  */
void LCD_DrawCircle(uint16_t Xpos, uint16_t Ypos, uint16_t Radius)
{
  
    int x = -Radius, y = 0, err = 2-2*Radius, e2;
    do {
      

        *(__IO uint16_t*) (CurrentFrameBuffer + (3*((Xpos-x) + LCD_PIXEL_WIDTH*(Ypos+y))))= (0x00FFFF & CurrentTextColor);       //GB
        *(__IO uint8_t*)(CurrentFrameBuffer + (3*((Xpos-x) + LCD_PIXEL_WIDTH*(Ypos+y))+2))= (0xFF0000 & CurrentTextColor) >> 16; //R
      
        *(__IO uint16_t*) (CurrentFrameBuffer + (3*((Xpos+x) + LCD_PIXEL_WIDTH*(Ypos+y)))) = (0x00FFFF & CurrentTextColor);      //GB
        *(__IO uint8_t*)(CurrentFrameBuffer + (3*((Xpos+x) + LCD_PIXEL_WIDTH*(Ypos+y))+2))= (0xFF0000 & CurrentTextColor) >> 16; //R
      
        *(__IO uint16_t*) (CurrentFrameBuffer + (3*((Xpos+x) + LCD_PIXEL_WIDTH*(Ypos-y)))) = (0x00FFFF & CurrentTextColor);      //GB
        *(__IO uint8_t*)(CurrentFrameBuffer + (3*((Xpos+x) + LCD_PIXEL_WIDTH*(Ypos-y))+2))= (0xFF0000 & CurrentTextColor) >> 16; //R
      
        *(__IO uint16_t*) (CurrentFrameBuffer + (3*((Xpos-x) + LCD_PIXEL_WIDTH*(Ypos-y)))) = (0x00FFFF & CurrentTextColor);      //GB
        *(__IO uint8_t*)(CurrentFrameBuffer + (3*((Xpos-x) + LCD_PIXEL_WIDTH*(Ypos-y))+2))= (0xFF0000 & CurrentTextColor) >> 16; //R


        e2 = err;
        if (e2 <= y) {
            err += ++y*2+1;
            if (-x == y && e2 <= x) e2 = 0;
        }
        if (e2 > x) err += ++x*2+1;
    }
    while (x <= 0);
    
}

/**
  * @brief  Draw a full ellipse.
  * @param  Xpos: specifies the X position, can be a value from 0 to 240.
  * @param  Ypos: specifies the Y position, can be a value from 0 to 320.
  * @param  Radius: minor radius of ellipse.
  * @param  Radius2: major radius of ellipse.  
  * @retval None
  */
void LCD_DrawFullEllipse(int Xpos, int Ypos, int Radius, int Radius2)
{
  int x = -Radius, y = 0, err = 2-2*Radius, e2;
  float K = 0, rad1 = 0, rad2 = 0;
  
  rad1 = Radius;
  rad2 = Radius2;
  
  if (Radius > Radius2)
  { 
    do 
    {
      K = (float)(rad1/rad2);
      LCD_DrawLine((Xpos+x), (Ypos-(uint16_t)(y/K)), (2*(uint16_t)(y/K) + 1), LCD_DIR_VERTICAL);
      LCD_DrawLine((Xpos-x), (Ypos-(uint16_t)(y/K)), (2*(uint16_t)(y/K) + 1), LCD_DIR_VERTICAL);
      
      e2 = err;
      if (e2 <= y) 
      {
        err += ++y*2+1;
        if (-x == y && e2 <= x) e2 = 0;
      }
      if (e2 > x) err += ++x*2+1;
      
    }
    while (x <= 0);
  }
  else
  {
    y = -Radius2; 
    x = 0;
    do 
    { 
      K = (float)(rad2/rad1);       
      LCD_DrawLine((Xpos-(uint16_t)(x/K)), (Ypos+y), (2*(uint16_t)(x/K) + 1), LCD_DIR_HORIZONTAL);
      LCD_DrawLine((Xpos-(uint16_t)(x/K)), (Ypos-y), (2*(uint16_t)(x/K) + 1), LCD_DIR_HORIZONTAL);
      
      e2 = err;
      if (e2 <= x) 
      {
        err += ++x*2+1;
        if (-y == x && e2 <= y) e2 = 0;
      }
      if (e2 > y) err += ++y*2+1;
    }
    while (y <= 0);
  }
}

/**
  * @brief  Displays an Ellipse.
  * @param  Xpos: specifies the X position, can be a value from 0 to 240.
  * @param  Ypos: specifies the Y position, can be a value from 0 to 320.
  * @param  Radius: specifies Radius.
  * @param  Radius2: specifies Radius2.
  * @retval None
  */
void LCD_DrawEllipse(int Xpos, int Ypos, int Radius, int Radius2)
{
  int x = -Radius, y = 0, err = 2-2*Radius, e2;
  float K = 0, rad1 = 0, rad2 = 0;
   
  rad1 = Radius;
  rad2 = Radius2;
  
  if (Radius > Radius2)
  { 
    do {
      K = (float)(rad1/rad2);
   
      *(__IO uint16_t*)(CurrentFrameBuffer + (3*((Xpos-x) + LCD_PIXEL_WIDTH*(Ypos+(uint16_t)(y/K))))) = (0x00FFFF & CurrentTextColor);        //GB
      *(__IO uint8_t*)(CurrentFrameBuffer + (3*((Xpos-x) + LCD_PIXEL_WIDTH*(Ypos+(uint16_t)(y/K)))+2)) = (0xFF0000 & CurrentTextColor) >> 16; //R
     
      *(__IO uint16_t*)(CurrentFrameBuffer + (3*((Xpos+x) + LCD_PIXEL_WIDTH*(Ypos+(uint16_t)(y/K))))) = (0x00FFFF & CurrentTextColor);        //GB
      *(__IO uint8_t*)(CurrentFrameBuffer + (3*((Xpos+x) + LCD_PIXEL_WIDTH*(Ypos+(uint16_t)(y/K)))+2)) = (0xFF0000 & CurrentTextColor) >> 16; //R

      *(__IO uint16_t*)(CurrentFrameBuffer + (3*((Xpos+x) + LCD_PIXEL_WIDTH*(Ypos-(uint16_t)(y/K))))) = (0x00FFFF & CurrentTextColor);        //GB
      *(__IO uint8_t*)(CurrentFrameBuffer + (3*((Xpos+x) + LCD_PIXEL_WIDTH*(Ypos-(uint16_t)(y/K)))+2)) = (0xFF0000 & CurrentTextColor) >> 16; //R

      *(__IO uint16_t*)(CurrentFrameBuffer + (3*((Xpos-x) + LCD_PIXEL_WIDTH*(Ypos-(uint16_t)(y/K))))) = (0x00FFFF & CurrentTextColor);        //GB
      *(__IO uint8_t*)(CurrentFrameBuffer + (3*((Xpos-x) + LCD_PIXEL_WIDTH*(Ypos-(uint16_t)(y/K)))+2)) = (0xFF0000 & CurrentTextColor) >> 16; //R

      
      e2 = err;
      if (e2 <= y) {
        err += ++y*2+1;
        if (-x == y && e2 <= x) e2 = 0;
      }
      if (e2 > x) err += ++x*2+1;
    }
    while (x <= 0);
  }
  else
  {
    y = -Radius2; 
    x = 0;
    do { 
      K = (float)(rad2/rad1);

      *(__IO uint16_t*)(CurrentFrameBuffer + (3*((Xpos-(uint16_t)(x/K)) + LCD_PIXEL_WIDTH*(Ypos+y))))= (0x00FFFF & CurrentTextColor);        //GB
      *(__IO uint8_t*)(CurrentFrameBuffer + (3*((Xpos-(uint16_t)(x/K)) + LCD_PIXEL_WIDTH*(Ypos+y))+2)) = (0xFF0000 & CurrentTextColor) >> 16; //R

      *(__IO uint16_t*)(CurrentFrameBuffer + (3*((Xpos+(uint16_t)(x/K)) + LCD_PIXEL_WIDTH*(Ypos+y))))= (0x00FFFF & CurrentTextColor);        //GB
      *(__IO uint8_t*)(CurrentFrameBuffer + (3*((Xpos+(uint16_t)(x/K)) + LCD_PIXEL_WIDTH*(Ypos+y))+2)) = (0xFF0000 & CurrentTextColor) >> 16; //R

      *(__IO uint16_t*)(CurrentFrameBuffer + (3*((Xpos+(uint16_t)(x/K)) + LCD_PIXEL_WIDTH*(Ypos-y))))= (0x00FFFF & CurrentTextColor);        //GB
      *(__IO uint8_t*)(CurrentFrameBuffer + (3*((Xpos+(uint16_t)(x/K)) + LCD_PIXEL_WIDTH*(Ypos-y))+2)) = (0xFF0000 & CurrentTextColor) >> 16; //R

      *(__IO uint16_t*)(CurrentFrameBuffer + (3*((Xpos-(uint16_t)(x/K)) + LCD_PIXEL_WIDTH*(Ypos-y))))= (0x00FFFF & CurrentTextColor);        //GB
      *(__IO uint8_t*)(CurrentFrameBuffer + (3*((Xpos-(uint16_t)(x/K)) + LCD_PIXEL_WIDTH*(Ypos-y))+2)) = (0xFF0000 & CurrentTextColor) >> 16; //R

      
      e2 = err;
      if (e2 <= x) {
        err += ++x*3+1;
        if (-y == x && e2 <= y) e2 = 0;
      }
      if (e2 > y) err += ++y*3+1;     
    }
    while (y <= 0);
  }
}

/**
  * @brief  Displays a mono-color picture.
  * @param  Pict: pointer to the picture array.
  * @retval None
  */
void LCD_DrawMonoPict(const uint32_t *Pict)
{
  uint32_t index = 0, counter = 0;
  
   
  for(index = 0; index < 2400; index++)
  {
    for(counter = 0; counter < 32; counter++)
    {
      if((Pict[index] & (1 << counter)) == 0x00)
      {
       
        *(__IO uint16_t*)(CurrentFrameBuffer) = (0x00FFFF & CurrentBackColor);        //GB
        *(__IO uint8_t*)(CurrentFrameBuffer+2)= (0xFF0000 & CurrentBackColor) >> 16; //R

      }
      else
      {
        
        *(__IO uint16_t*)(CurrentFrameBuffer) = (0x00FFFF & CurrentTextColor);        //GB
        *(__IO uint8_t*)(CurrentFrameBuffer+2)= (0xFF0000 & CurrentTextColor) >> 16; //R
      }
    }
  }
}

/**
  * @brief  Displays a bitmap picture loaded in the internal Flash.
  * @param  BmpAddress: Bmp picture address in the internal Flash.
  * @retval None
  */
void LCD_WriteBMP(uint32_t BmpAddress)
{
  uint32_t index = 0, size = 0, width = 0, height = 0, bit_pixel = 0;
  uint32_t Address;
  uint32_t currentline = 0, linenumber = 0;
 
  Address = CurrentFrameBuffer;

  /* Read bitmap size */
  size = *(__IO uint16_t *) (BmpAddress + 2);
  size |= (*(__IO uint16_t *) (BmpAddress + 4)) << 16;
  
  /* Get bitmap data address offset */
  index = *(__IO uint16_t *) (BmpAddress + 10);
  index |= (*(__IO uint16_t *) (BmpAddress + 12)) << 16;

  /* Read bitmap width */
  width = *(uint16_t *) (BmpAddress + 18);
  width |= (*(uint16_t *) (BmpAddress + 20)) << 16;

  /* Read bitmap height */
  height = *(uint16_t *) (BmpAddress + 22);
  height |= (*(uint16_t *) (BmpAddress + 24)) << 16;

  /* Read bit/pixel */
  bit_pixel = *(uint16_t *) (BmpAddress + 28);  
 
  if (CurrentLayer == LCD_BACKGROUND_LAYER)
  {
    /* reconfigure layer size in accordance with the picture */
    LTDC_LayerSize(LTDC_Layer1, width, height);
    LTDC_ReloadConfig(LTDC_VBReload);

    /* Reconfigure the Layer pixel format in accordance with the picture */    
    if ((bit_pixel/8) == 4)
    {
      LTDC_LayerPixelFormat(LTDC_Layer1, LTDC_Pixelformat_ARGB8888);
      LTDC_ReloadConfig(LTDC_VBReload);
    }
    else if ((bit_pixel/8) == 2)
    {
      LTDC_LayerPixelFormat(LTDC_Layer1, LTDC_Pixelformat_RGB565);
      LTDC_ReloadConfig(LTDC_VBReload);
    }
    else 
    {
      LTDC_LayerPixelFormat(LTDC_Layer1, LTDC_Pixelformat_RGB888);
      LTDC_ReloadConfig(LTDC_VBReload);
    } 
  }
  else
  {
    /* reconfigure layer size in accordance with the picture */
    LTDC_LayerSize(LTDC_Layer2, width, height);
    LTDC_ReloadConfig(LTDC_VBReload); 
    
    /* Reconfigure the Layer pixel format in accordance with the picture */
    if ((bit_pixel/8) == 4)
    {
      LTDC_LayerPixelFormat(LTDC_Layer2, LTDC_Pixelformat_ARGB8888);
      LTDC_ReloadConfig(LTDC_VBReload);
    }
    else if ((bit_pixel/8) == 2)
    {
      LTDC_LayerPixelFormat(LTDC_Layer2, LTDC_Pixelformat_RGB565);
      LTDC_ReloadConfig(LTDC_VBReload);
    }
    else
    {
      LTDC_LayerPixelFormat(LTDC_Layer2, LTDC_Pixelformat_RGB888);
      LTDC_ReloadConfig(LTDC_VBReload);  
    }
  }
 
  /* compute the real size of the picture (without the header)) */  
  size = (size - index); 

  /* bypass the bitmap header */
  BmpAddress += index;

  /* start copie image from the bottom */
  Address += width*(height-1)*(bit_pixel/8);
  
  for(index = 0; index < size; index++)
  {
    *(__IO uint8_t*) (Address) = *(__IO uint8_t *)BmpAddress;
    
    /*jump on next byte */   
    BmpAddress++;
    Address++;
    currentline++;
    
    if((currentline/(bit_pixel/8)) == width)
    {
      if(linenumber < height)
      {
        linenumber++;
        Address -=(2*width*(bit_pixel/8));
        currentline = 0;
      }
    }
  }
}

/**
  * @brief  ����ʵ�ľ���
  * @param  Xpos: ��ʼX����
  * @param  Ypos: ��ʼY����
  * @param  Height: ���θ�
  * @param  Width: ���ο�
  * @retval None
  */
void LCD_DrawFullRect(uint16_t Xpos, uint16_t Ypos, uint16_t Width, uint16_t Height)
{
  DMA2D_InitTypeDef      DMA2D_InitStruct;
  
  uint32_t  Xaddress = 0; 
  uint16_t Red_Value = 0, Green_Value = 0, Blue_Value = 0;
 
  Red_Value = (0xFF0000 & CurrentTextColor)>>16 ;
  Blue_Value = 0x0000FF & CurrentTextColor;
  Green_Value = (0x00FF00 & CurrentTextColor)>>8;
  
  Xaddress = CurrentFrameBuffer + 3*(LCD_PIXEL_WIDTH*Ypos + Xpos);
  
  /* ����DMA2D DMA2D */
  DMA2D_DeInit();
  DMA2D_InitStruct.DMA2D_Mode = DMA2D_R2M;       
  DMA2D_InitStruct.DMA2D_CMode = DMA2D_RGB888;      
  DMA2D_InitStruct.DMA2D_OutputGreen = Green_Value;      
  DMA2D_InitStruct.DMA2D_OutputBlue = Blue_Value;     
  DMA2D_InitStruct.DMA2D_OutputRed = Red_Value;                
  DMA2D_InitStruct.DMA2D_OutputAlpha = 0x0F;                  
  DMA2D_InitStruct.DMA2D_OutputMemoryAdd = Xaddress;                
  DMA2D_InitStruct.DMA2D_OutputOffset = (LCD_PIXEL_WIDTH - Width);                
  DMA2D_InitStruct.DMA2D_NumberOfLine = Height;            
  DMA2D_InitStruct.DMA2D_PixelPerLine = Width;
  DMA2D_Init(&DMA2D_InitStruct); 
  
  /* ��ʼDMA2D���� */ 
  DMA2D_StartTransfer();
  
  /* �ȴ�������� */
  while(DMA2D_GetFlagStatus(DMA2D_FLAG_TC) == RESET)
  {
  } 

  LCD_SetTextColor(CurrentTextColor);
}

/**
  * @brief  Displays a full circle.
  * @param  Xpos: specifies the X position, can be a value from 0 to 240.
  * @param  Ypos: specifies the Y position, can be a value from 0 to 320.
  * @param  Radius
  * @retval None
  */
void LCD_DrawFullCircle(uint16_t Xpos, uint16_t Ypos, uint16_t Radius)
{
  int32_t  D;    /* Decision Variable */ 
  uint32_t  CurX;/* Current X Value */
  uint32_t  CurY;/* Current Y Value */ 
  
  D = 3 - (Radius << 1);
  
  CurX = 0;
  CurY = Radius;
  
  while (CurX <= CurY)
  {
    if(CurY > 0) 
    {
      LCD_DrawLine(Xpos - CurX, Ypos - CurY, 2*CurY, LCD_DIR_VERTICAL);
      LCD_DrawLine(Xpos + CurX, Ypos - CurY, 2*CurY, LCD_DIR_VERTICAL);
    }
    
    if(CurX > 0) 
    {
      LCD_DrawLine(Xpos - CurY, Ypos - CurX, 2*CurX, LCD_DIR_VERTICAL);
      LCD_DrawLine(Xpos + CurY, Ypos - CurX, 2*CurX, LCD_DIR_VERTICAL);
    }
    if (D < 0)
    { 
      D += (CurX << 2) + 6;
    }
    else
    {
      D += ((CurX - CurY) << 2) + 10;
      CurY--;
    }
    CurX++;
  }
  
  LCD_DrawCircle(Xpos, Ypos, Radius);  
}

/**
  * @brief  Displays an uni-line (between two points).
  * @param  x1: specifies the point 1 x position.
  * @param  y1: specifies the point 1 y position.
  * @param  x2: specifies the point 2 x position.
  * @param  y2: specifies the point 2 y position.
  * @retval None
  */
void LCD_DrawUniLine(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2)
{
  int16_t deltax = 0, deltay = 0, x = 0, y = 0, xinc1 = 0, xinc2 = 0, 
  yinc1 = 0, yinc2 = 0, den = 0, num = 0, numadd = 0, numpixels = 0, 
  curpixel = 0;
  
  deltax = ABS(x2 - x1);        /* The difference between the x's */
  deltay = ABS(y2 - y1);        /* The difference between the y's */
  x = x1;                       /* Start x off at the first pixel */
  y = y1;                       /* Start y off at the first pixel */
  
  if (x2 >= x1)                 /* The x-values are increasing */
  {
    xinc1 = 1;
    xinc2 = 1;
  }
  else                          /* The x-values are decreasing */
  {
    xinc1 = -1;
    xinc2 = -1;
  }
  
  if (y2 >= y1)                 /* The y-values are increasing */
  {
    yinc1 = 1;
    yinc2 = 1;
  }
  else                          /* The y-values are decreasing */
  {
    yinc1 = -1;
    yinc2 = -1;
  }
  
  if (deltax >= deltay)         /* There is at least one x-value for every y-value */
  {
    xinc1 = 0;                  /* Don't change the x when numerator >= denominator */
    yinc2 = 0;                  /* Don't change the y for every iteration */
    den = deltax;
    num = deltax / 2;
    numadd = deltay;
    numpixels = deltax;         /* There are more x-values than y-values */
  }
  else                          /* There is at least one y-value for every x-value */
  {
    xinc2 = 0;                  /* Don't change the x for every iteration */
    yinc1 = 0;                  /* Don't change the y when numerator >= denominator */
    den = deltay;
    num = deltay / 2;
    numadd = deltax;
    numpixels = deltay;         /* There are more y-values than x-values */
  }
  
  for (curpixel = 0; curpixel <= numpixels; curpixel++)
  {
    PutPixel(x, y);             /* Draw the current pixel */
    num += numadd;              /* Increase the numerator by the top of the fraction */
    if (num >= den)             /* Check if numerator >= denominator */
    {
      num -= den;               /* Calculate the new numerator value */
      x += xinc1;               /* Change the x as appropriate */
      y += yinc1;               /* Change the y as appropriate */
    }
    x += xinc2;                 /* Change the x as appropriate */
    y += yinc2;                 /* Change the y as appropriate */
  }
}

/**
  * @brief  Displays an triangle.
  * @param  Points: pointer to the points array.
  * @retval None
  */
void LCD_Triangle(pPoint Points, uint16_t PointCount)
{
  int16_t X = 0, Y = 0;
  pPoint First = Points;

  if(PointCount != 3)
  {
    return;
  }

  while(--PointCount)
  {
    X = Points->X;
    Y = Points->Y;
    Points++;
    LCD_DrawUniLine(X, Y, Points->X, Points->Y);
  }
  LCD_DrawUniLine(First->X, First->Y, Points->X, Points->Y);
}

/**
  * @brief  Fill an triangle (between 3 points).
  * @param  x1..3: x position of triangle point 1..3.
  * @param  y1..3: y position of triangle point 1..3.
  * @retval None
  */
void LCD_FillTriangle(uint16_t x1, uint16_t x2, uint16_t x3, uint16_t y1, uint16_t y2, uint16_t y3)
{ 
  
  int16_t deltax = 0, deltay = 0, x = 0, y = 0, xinc1 = 0, xinc2 = 0, 
  yinc1 = 0, yinc2 = 0, den = 0, num = 0, numadd = 0, numpixels = 0, 
  curpixel = 0;
  
  deltax = ABS(x2 - x1);        /* The difference between the x's */
  deltay = ABS(y2 - y1);        /* The difference between the y's */
  x = x1;                       /* Start x off at the first pixel */
  y = y1;                       /* Start y off at the first pixel */
  
  if (x2 >= x1)                 /* The x-values are increasing */
  {
    xinc1 = 1;
    xinc2 = 1;
  }
  else                          /* The x-values are decreasing */
  {
    xinc1 = -1;
    xinc2 = -1;
  }
  
  if (y2 >= y1)                 /* The y-values are increasing */
  {
    yinc1 = 1;
    yinc2 = 1;
  }
  else                          /* The y-values are decreasing */
  {
    yinc1 = -1;
    yinc2 = -1;
  }
  
  if (deltax >= deltay)         /* There is at least one x-value for every y-value */
  {
    xinc1 = 0;                  /* Don't change the x when numerator >= denominator */
    yinc2 = 0;                  /* Don't change the y for every iteration */
    den = deltax;
    num = deltax / 2;
    numadd = deltay;
    numpixels = deltax;         /* There are more x-values than y-values */
  }
  else                          /* There is at least one y-value for every x-value */
  {
    xinc2 = 0;                  /* Don't change the x for every iteration */
    yinc1 = 0;                  /* Don't change the y when numerator >= denominator */
    den = deltay;
    num = deltay / 2;
    numadd = deltax;
    numpixels = deltay;         /* There are more y-values than x-values */
  }
  
  for (curpixel = 0; curpixel <= numpixels; curpixel++)
  {
    LCD_DrawUniLine(x, y, x3, y3);
    
    num += numadd;              /* Increase the numerator by the top of the fraction */
    if (num >= den)             /* Check if numerator >= denominator */
    {
      num -= den;               /* Calculate the new numerator value */
      x += xinc1;               /* Change the x as appropriate */
      y += yinc1;               /* Change the y as appropriate */
    }
    x += xinc2;                 /* Change the x as appropriate */
    y += yinc2;                 /* Change the y as appropriate */
  }  
  
  
}
/**
  * @brief  Displays an poly-line (between many points).
  * @param  Points: pointer to the points array.
  * @param  PointCount: Number of points.
  * @retval None
  */
void LCD_PolyLine(pPoint Points, uint16_t PointCount)
{
  int16_t X = 0, Y = 0;

  if(PointCount < 2)
  {
    return;
  }

  while(--PointCount)
  {
    X = Points->X;
    Y = Points->Y;
    Points++;
    LCD_DrawUniLine(X, Y, Points->X, Points->Y);
  }
}

/**
  * @brief  Displays an relative poly-line (between many points).
  * @param  Points: pointer to the points array.
  * @param  PointCount: Number of points.
  * @param  Closed: specifies if the draw is closed or not.
  *           1: closed, 0 : not closed.
  * @retval None
  */
static void LCD_PolyLineRelativeClosed(pPoint Points, uint16_t PointCount, uint16_t Closed)
{
  int16_t X = 0, Y = 0;
  pPoint First = Points;

  if(PointCount < 2)
  {
    return;
  }  
  X = Points->X;
  Y = Points->Y;
  while(--PointCount)
  {
    Points++;
    LCD_DrawUniLine(X, Y, X + Points->X, Y + Points->Y);
    X = X + Points->X;
    Y = Y + Points->Y;
  }
  if(Closed)
  {
    LCD_DrawUniLine(First->X, First->Y, X, Y);
  }  
}

/**
  * @brief  Displays a closed poly-line (between many points).
  * @param  Points: pointer to the points array.
  * @param  PointCount: Number of points.
  * @retval None
  */
void LCD_ClosedPolyLine(pPoint Points, uint16_t PointCount)
{
  LCD_PolyLine(Points, PointCount);
  LCD_DrawUniLine(Points->X, Points->Y, (Points+PointCount-1)->X, (Points+PointCount-1)->Y);
}

/**
  * @brief  Displays a relative poly-line (between many points).
  * @param  Points: pointer to the points array.
  * @param  PointCount: Number of points.
  * @retval None
  */
void LCD_PolyLineRelative(pPoint Points, uint16_t PointCount)
{
  LCD_PolyLineRelativeClosed(Points, PointCount, 0);
}

/**
  * @brief  Displays a closed relative poly-line (between many points).
  * @param  Points: pointer to the points array.
  * @param  PointCount: Number of points.
  * @retval None
  */
void LCD_ClosedPolyLineRelative(pPoint Points, uint16_t PointCount)
{
  LCD_PolyLineRelativeClosed(Points, PointCount, 1);
}

/**
  * @brief  Displays a  full poly-line (between many points).
  * @param  Points: pointer to the points array.
  * @param  PointCount: Number of points.
  * @retval None
  */
void LCD_FillPolyLine(pPoint Points, uint16_t PointCount)
{
 
  int16_t X = 0, Y = 0, X2 = 0, Y2 = 0, X_center = 0, Y_center = 0, X_first = 0, Y_first = 0, pixelX = 0, pixelY = 0, counter = 0;
  uint16_t  IMAGE_LEFT = 0, IMAGE_RIGHT = 0, IMAGE_TOP = 0, IMAGE_BOTTOM = 0;  

  IMAGE_LEFT = IMAGE_RIGHT = Points->X;
  IMAGE_TOP= IMAGE_BOTTOM = Points->Y;

  for(counter = 1; counter < PointCount; counter++)
  {
    pixelX = POLY_X(counter);
    if(pixelX < IMAGE_LEFT)
    {
      IMAGE_LEFT = pixelX;
    }
    if(pixelX > IMAGE_RIGHT)
    {
      IMAGE_RIGHT = pixelX;
    }
    
    pixelY = POLY_Y(counter);
    if(pixelY < IMAGE_TOP)
    { 
      IMAGE_TOP = pixelY;
    }
    if(pixelY > IMAGE_BOTTOM)
    {
      IMAGE_BOTTOM = pixelY;
    }
  }  
  
  if(PointCount < 2)
  {
    return;
  }
  
  X_center = (IMAGE_LEFT + IMAGE_RIGHT)/2;
  Y_center = (IMAGE_BOTTOM + IMAGE_TOP)/2;
 
  X_first = Points->X;
  Y_first = Points->Y;
  
  while(--PointCount)
  {
    X = Points->X;
    Y = Points->Y;
    Points++;
    X2 = Points->X;
    Y2 = Points->Y;    
  
    LCD_FillTriangle(X, X2, X_center, Y, Y2, Y_center);
    LCD_FillTriangle(X, X_center, X2, Y, Y_center, Y2);
    LCD_FillTriangle(X_center, X2, X, Y_center, Y2, Y);   
  }
  
  LCD_FillTriangle(X_first, X2, X_center, Y_first, Y2, Y_center);
  LCD_FillTriangle(X_first, X_center, X2, Y_first, Y_center, Y2);
  LCD_FillTriangle(X_center, X2, X_first, Y_center, Y2, Y_first); 
}




/**
  * @brief  Sets or reset LCD control lines.
  * @param  GPIOx: where x can be B or D to select the GPIO peripheral.
  * @param  CtrlPins: the Control line.
  *   This parameter can be:
  *     @arg LCD_NCS_PIN: Chip Select pin
  *     @arg LCD_NWR_PIN: Read/Write Selection pin
  *     @arg LCD_RS_PIN: Register/RAM Selection pin
  * @param  BitVal: specifies the value to be written to the selected bit.
  *   This parameter can be:
  *     @arg Bit_RESET: to clear the port pin
  *     @arg Bit_SET: to set the port pin
  * @retval None
  */
void LCD_CtrlLinesWrite(GPIO_TypeDef* GPIOx, uint16_t CtrlPins, BitAction BitVal)
{
  /* Set or Reset the control line */
  GPIO_WriteBit(GPIOx, (uint16_t)CtrlPins, (BitAction)BitVal);
}


 /**
  * @brief  ��ʼ������LCD��IO
  * @param  ��
  * @retval ��
  */
static void LCD_GPIO_Config(void)
{ 
	GPIO_InitTypeDef GPIO_InitStruct;
  
  /* ʹ��LCDʹ�õ�������ʱ�� */
                          //��ɫ������
  RCC_AHB1PeriphClockCmd(LTDC_R0_GPIO_CLK | LTDC_R1_GPIO_CLK | LTDC_R2_GPIO_CLK| 
                         LTDC_R3_GPIO_CLK | LTDC_R4_GPIO_CLK | LTDC_R5_GPIO_CLK|
                         LTDC_R6_GPIO_CLK | LTDC_R7_GPIO_CLK |
                          //��ɫ������
                          LTDC_G0_GPIO_CLK|LTDC_G1_GPIO_CLK|LTDC_G2_GPIO_CLK|
                          LTDC_G3_GPIO_CLK|LTDC_G4_GPIO_CLK|LTDC_G5_GPIO_CLK|
                          LTDC_G6_GPIO_CLK|LTDC_G7_GPIO_CLK|
                          //��ɫ������
                          LTDC_B0_GPIO_CLK|LTDC_B1_GPIO_CLK|LTDC_B2_GPIO_CLK|
                          LTDC_B3_GPIO_CLK|LTDC_B4_GPIO_CLK|LTDC_B5_GPIO_CLK|
                          LTDC_B6_GPIO_CLK|LTDC_B7_GPIO_CLK|
                          //�����ź���
                          LTDC_CLK_GPIO_CLK | LTDC_HSYNC_GPIO_CLK |LTDC_VSYNC_GPIO_CLK|
                          LTDC_DE_GPIO_CLK  | LTDC_BL_GPIO_CLK    |LTDC_DISP_GPIO_CLK ,ENABLE);

/* GPIO���� */

 /* ��ɫ������ */
  GPIO_InitStruct.GPIO_Pin = LTDC_R0_GPIO_PIN;                             
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
  
  GPIO_Init(LTDC_R0_GPIO_PORT, &GPIO_InitStruct);
  GPIO_PinAFConfig(LTDC_R0_GPIO_PORT, LTDC_R0_PINSOURCE, LTDC_R0_AF);

  GPIO_InitStruct.GPIO_Pin = LTDC_R1_GPIO_PIN;
  GPIO_Init(LTDC_R1_GPIO_PORT, &GPIO_InitStruct);
  GPIO_PinAFConfig(LTDC_R1_GPIO_PORT, LTDC_R1_PINSOURCE, LTDC_R1_AF);
  
  GPIO_InitStruct.GPIO_Pin = LTDC_R2_GPIO_PIN;
  GPIO_Init(LTDC_R2_GPIO_PORT, &GPIO_InitStruct);
  GPIO_PinAFConfig(LTDC_R2_GPIO_PORT, LTDC_R2_PINSOURCE, LTDC_R2_AF);
  
  GPIO_InitStruct.GPIO_Pin = LTDC_R3_GPIO_PIN;
  GPIO_Init(LTDC_R3_GPIO_PORT, &GPIO_InitStruct);
  GPIO_PinAFConfig(LTDC_R3_GPIO_PORT, LTDC_R3_PINSOURCE, LTDC_R3_AF);
  
  GPIO_InitStruct.GPIO_Pin = LTDC_R4_GPIO_PIN;
  GPIO_Init(LTDC_R4_GPIO_PORT, &GPIO_InitStruct);
  GPIO_PinAFConfig(LTDC_R4_GPIO_PORT, LTDC_R4_PINSOURCE, LTDC_R4_AF);
  
  GPIO_InitStruct.GPIO_Pin = LTDC_R5_GPIO_PIN;
  GPIO_Init(LTDC_R5_GPIO_PORT, &GPIO_InitStruct);
  GPIO_PinAFConfig(LTDC_R5_GPIO_PORT, LTDC_R5_PINSOURCE, LTDC_R5_AF);
  
  GPIO_InitStruct.GPIO_Pin = LTDC_R6_GPIO_PIN;
  GPIO_Init(LTDC_R6_GPIO_PORT, &GPIO_InitStruct);
  GPIO_PinAFConfig(LTDC_R6_GPIO_PORT, LTDC_R6_PINSOURCE, LTDC_R6_AF);
  
  GPIO_InitStruct.GPIO_Pin = LTDC_R7_GPIO_PIN;
  GPIO_Init(LTDC_R7_GPIO_PORT, &GPIO_InitStruct);
  GPIO_PinAFConfig(LTDC_R7_GPIO_PORT, LTDC_R7_PINSOURCE, LTDC_R7_AF);
  
  //��ɫ������
  GPIO_InitStruct.GPIO_Pin = LTDC_G0_GPIO_PIN;
  GPIO_Init(LTDC_G0_GPIO_PORT, &GPIO_InitStruct);
  GPIO_PinAFConfig(LTDC_G0_GPIO_PORT, LTDC_G0_PINSOURCE, LTDC_G0_AF);
  
  GPIO_InitStruct.GPIO_Pin = LTDC_G1_GPIO_PIN;
  GPIO_Init(LTDC_G1_GPIO_PORT, &GPIO_InitStruct);
  GPIO_PinAFConfig(LTDC_G1_GPIO_PORT, LTDC_G1_PINSOURCE, LTDC_G1_AF);
  
  GPIO_InitStruct.GPIO_Pin = LTDC_G2_GPIO_PIN;
  GPIO_Init(LTDC_G2_GPIO_PORT, &GPIO_InitStruct);
  GPIO_PinAFConfig(LTDC_G2_GPIO_PORT, LTDC_G2_PINSOURCE, LTDC_G2_AF);
  
  GPIO_InitStruct.GPIO_Pin = LTDC_G3_GPIO_PIN;
  GPIO_Init(LTDC_G3_GPIO_PORT, &GPIO_InitStruct);
  GPIO_PinAFConfig(LTDC_G3_GPIO_PORT, LTDC_G3_PINSOURCE, LTDC_G3_AF);
  
  GPIO_InitStruct.GPIO_Pin = LTDC_G4_GPIO_PIN;
  GPIO_Init(LTDC_G4_GPIO_PORT, &GPIO_InitStruct);
  GPIO_PinAFConfig(LTDC_G4_GPIO_PORT, LTDC_G4_PINSOURCE, LTDC_G4_AF);
  
  GPIO_InitStruct.GPIO_Pin = LTDC_G5_GPIO_PIN;
  GPIO_Init(LTDC_G5_GPIO_PORT, &GPIO_InitStruct);
  GPIO_PinAFConfig(LTDC_G5_GPIO_PORT, LTDC_G5_PINSOURCE, LTDC_G5_AF);
  
  GPIO_InitStruct.GPIO_Pin = LTDC_G6_GPIO_PIN;
  GPIO_Init(LTDC_G6_GPIO_PORT, &GPIO_InitStruct);
  GPIO_PinAFConfig(LTDC_G6_GPIO_PORT, LTDC_G6_PINSOURCE, LTDC_G6_AF);
  
  GPIO_InitStruct.GPIO_Pin = LTDC_G7_GPIO_PIN;
  GPIO_Init(LTDC_G7_GPIO_PORT, &GPIO_InitStruct);
  GPIO_PinAFConfig(LTDC_G7_GPIO_PORT, LTDC_G7_PINSOURCE, LTDC_G7_AF);
  
  //��ɫ������
  GPIO_InitStruct.GPIO_Pin = LTDC_B0_GPIO_PIN;
  GPIO_Init(LTDC_B0_GPIO_PORT, &GPIO_InitStruct);
  GPIO_PinAFConfig(LTDC_B0_GPIO_PORT, LTDC_B0_PINSOURCE, LTDC_B0_AF);
  
  GPIO_InitStruct.GPIO_Pin = LTDC_B1_GPIO_PIN;
  GPIO_Init(LTDC_B1_GPIO_PORT, &GPIO_InitStruct);
  GPIO_PinAFConfig(LTDC_B1_GPIO_PORT, LTDC_B1_PINSOURCE, LTDC_B1_AF);
  
  GPIO_InitStruct.GPIO_Pin = LTDC_B2_GPIO_PIN;
  GPIO_Init(LTDC_B2_GPIO_PORT, &GPIO_InitStruct);
  GPIO_PinAFConfig(LTDC_B2_GPIO_PORT, LTDC_B2_PINSOURCE, LTDC_B2_AF);
  
  GPIO_InitStruct.GPIO_Pin = LTDC_B3_GPIO_PIN;
  GPIO_Init(LTDC_B3_GPIO_PORT, &GPIO_InitStruct);
  GPIO_PinAFConfig(LTDC_B3_GPIO_PORT, LTDC_B3_PINSOURCE, LTDC_B3_AF);
  
  GPIO_InitStruct.GPIO_Pin = LTDC_B4_GPIO_PIN;
  GPIO_Init(LTDC_B4_GPIO_PORT, &GPIO_InitStruct);
  GPIO_PinAFConfig(LTDC_B4_GPIO_PORT, LTDC_B4_PINSOURCE, LTDC_B4_AF);
  
  GPIO_InitStruct.GPIO_Pin = LTDC_B5_GPIO_PIN;
  GPIO_Init(LTDC_B5_GPIO_PORT, &GPIO_InitStruct);
  GPIO_PinAFConfig(LTDC_B5_GPIO_PORT, LTDC_B5_PINSOURCE, LTDC_B5_AF);
  
  GPIO_InitStruct.GPIO_Pin = LTDC_B6_GPIO_PIN;
  GPIO_Init(LTDC_B6_GPIO_PORT, &GPIO_InitStruct);
  GPIO_PinAFConfig(LTDC_B6_GPIO_PORT, LTDC_B6_PINSOURCE, LTDC_B6_AF);
  
  GPIO_InitStruct.GPIO_Pin = LTDC_B7_GPIO_PIN;
  GPIO_Init(LTDC_B7_GPIO_PORT, &GPIO_InitStruct);
  GPIO_PinAFConfig(LTDC_B7_GPIO_PORT, LTDC_B7_PINSOURCE, LTDC_B7_AF);
  
  //�����ź���
  GPIO_InitStruct.GPIO_Pin = LTDC_CLK_GPIO_PIN;
  GPIO_Init(LTDC_CLK_GPIO_PORT, &GPIO_InitStruct);
  GPIO_PinAFConfig(LTDC_CLK_GPIO_PORT, LTDC_CLK_PINSOURCE, LTDC_CLK_AF);
  
  GPIO_InitStruct.GPIO_Pin = LTDC_HSYNC_GPIO_PIN;
  GPIO_Init(LTDC_HSYNC_GPIO_PORT, &GPIO_InitStruct);
  GPIO_PinAFConfig(LTDC_HSYNC_GPIO_PORT, LTDC_HSYNC_PINSOURCE, LTDC_HSYNC_AF);
  
  GPIO_InitStruct.GPIO_Pin = LTDC_VSYNC_GPIO_PIN;
  GPIO_Init(LTDC_VSYNC_GPIO_PORT, &GPIO_InitStruct);
  GPIO_PinAFConfig(LTDC_VSYNC_GPIO_PORT, LTDC_VSYNC_PINSOURCE, LTDC_VSYNC_AF);
  
  GPIO_InitStruct.GPIO_Pin = LTDC_DE_GPIO_PIN;
  GPIO_Init(LTDC_DE_GPIO_PORT, &GPIO_InitStruct);
  GPIO_PinAFConfig(LTDC_DE_GPIO_PORT, LTDC_DE_PINSOURCE, LTDC_DE_AF);
  
  //����BL ��Һ��ʹ���ź�DISP
  GPIO_InitStruct.GPIO_Pin = LTDC_DISP_GPIO_PIN;                             
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
  
  GPIO_Init(LTDC_DISP_GPIO_PORT, &GPIO_InitStruct);
  
  
  GPIO_InitStruct.GPIO_Pin = LTDC_BL_GPIO_PIN; 
  GPIO_Init(LTDC_BL_GPIO_PORT, &GPIO_InitStruct);
  
  //����ʹ��lcd
  GPIO_SetBits(LTDC_DISP_GPIO_PORT,LTDC_DISP_GPIO_PIN);
  GPIO_SetBits(LTDC_BL_GPIO_PORT,LTDC_BL_GPIO_PIN);


}

/**
  * @brief  ��ʾһ�����ص�
  * @param  x: ���ص��x����
  * @param  y: ���ص��y����
  * @retval None
  */
void PutPixel(int16_t x, int16_t y)
{ 
  if(x < 0 || x > LCD_PIXEL_WIDTH || y < 0 || y > LCD_PIXEL_HEIGHT)
  {
    return;  
  }
#if 0
 LCD_DrawLine(x, y, 1, LCD_DIR_HORIZONTAL);
#else /*����ֱ�����Ч�ʹ��ƻ�ߵ�*/
 {
	  /*RGB888*/
	  uint32_t  Xaddress =0;
    Xaddress =  CurrentFrameBuffer + 3*(LCD_PIXEL_WIDTH*y + x);
	  *(__IO uint16_t*) Xaddress= (0x00FFFF & CurrentTextColor);        //GB
  	*(__IO uint8_t*)( Xaddress+2)= (0xFF0000 & CurrentTextColor) >> 16; //R
  }
#endif
}




//#endif
/**
  * @}
  */ 

/**
  * @}
  */ 

/**
  * @}
  */ 

/**
  * @}
  */ 

/**
  * @}
  */ 
	
#else	
/****************RG565����***************************RGB565����**********************************RGB565����*****************************RGB565����*****************************RGB565����*****************************/


/**
 ******************************************************************************
 * @file    stm32f429i_discovery_lcd.c
 * @author  MCD Application Team
 * @version V1.0.1
 * @date    28-October-2013
 * @brief   This file includes the LCD driver for ILI9341 Liquid Crystal
 *          Display Modules of STM32F429I-DISCO kit (MB1075).
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; COPYRIGHT 2013 STMicroelectronics</center></h2>
 *
 * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
 * You may not use this file except in compliance with the License.
 * You may obtain a copy of the License at:
 *
 *        http://www.st.com/software_license_agreement_liberty_v2
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 ******************************************************************************
 */
/**
 * @}
 */

/** @defgroup STM32F429I_DISCOVERY_LCD_Private_Variables
 * @{
 */
static sFONT *LCD_Currentfonts;
/* Global variables to set the written text color */
static uint16_t CurrentTextColor   = 0x0000;
static uint16_t CurrentBackColor   = 0xFFFF;
/* Default LCD configuration with LCD Layer 1 */
static uint32_t CurrentFrameBuffer = LCD_FRAME_BUFFER;
static uint32_t CurrentLayer = LCD_BACKGROUND_LAYER;
/**
 * @}
 */
static void LCD_PolyLineRelativeClosed(pPoint Points, uint16_t PointCount, uint16_t Closed);
static void LCD_GPIO_Config(void);

/**
 * @}
 */



/**
 * @brief  Initializes the LCD.
 * @param  None
 * @retval None
 */


/*����Һ�������ֲ�Ĳ�������*/
#define HBP  46		//HSYNC�����Ч����
#define VBP  23		//VSYNC�����Ч����

#define HSW   1		//HSYNC���
#define VSW   1		//VSYNC���

#define HFP  20		//HSYNCǰ����Ч����
#define VFP   22		//VSYNCǰ����Ч����



void LCD_Init(void)
{
 LTDC_InitTypeDef       LTDC_InitStruct;

 /* Enable the LTDC Clock */
 RCC_APB2PeriphClockCmd(RCC_APB2Periph_LTDC, ENABLE);

 /* Enable the DMA2D Clock */
 RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2D, ENABLE);

 /* Configure the LCD Control pins */
 LCD_GPIO_Config();

 /* Configure the FMC Parallel interface : SDRAM is used as Frame Buffer for LCD */
 SDRAM_Init();

 /* LTDC Configuration *********************************************************/
 /* Polarity configuration */
 /* Initialize the horizontal synchronization polarity as active low */
 LTDC_InitStruct.LTDC_HSPolarity = LTDC_HSPolarity_AL;
 /* Initialize the vertical synchronization polarity as active low */
 LTDC_InitStruct.LTDC_VSPolarity = LTDC_VSPolarity_AL;
 /* Initialize the data enable polarity as active low */
 LTDC_InitStruct.LTDC_DEPolarity = LTDC_DEPolarity_AL;
 /* Initialize the pixel clock polarity as input pixel clock */
 LTDC_InitStruct.LTDC_PCPolarity = LTDC_PCPolarity_IPC;

 /* Configure R,G,B component values for LCD background color */
 LTDC_InitStruct.LTDC_BackgroundRedValue = 0;
 LTDC_InitStruct.LTDC_BackgroundGreenValue = 0;
 LTDC_InitStruct.LTDC_BackgroundBlueValue = 0;

	/* ���� PLLSAI ��Ƶ�������������Ϊ����ͬ��ʱ��CLK*/
  /* PLLSAI_VCO ����ʱ�� = HSE_VALUE/PLL_M = 1 Mhz */
  /* PLLSAI_VCO ���ʱ�� = PLLSAI_VCO���� * PLLSAI_N = 384 Mhz */
  /* PLLLCDCLK = PLLSAI_VCO ���/PLLSAI_R = 384/6  Mhz */
  /* LTDC ʱ��Ƶ�� = PLLLCDCLK / DIV = 384/4/4 = 24 Mhz */
	/* LTDCʱ��̫�߻ᵼ����������ˢ���ٶ�Ҫ�󲻸ߣ�����ʱ��Ƶ�ʿɼ��ٻ�������*/
	/* ���º������������ֱ�Ϊ��PLLSAIN,PLLSAIQ,PLLSAIR������PLLSAIQ��LTDC�޹�*/
 RCC_PLLSAIConfig(384, 7, 4);
 	/*���º����Ĳ���ΪDIVֵ*/
 RCC_LTDCCLKDivConfig(RCC_PLLSAIDivR_Div4);

 /* Enable PLLSAI Clock */
 RCC_PLLSAICmd(ENABLE);
 /* Wait for PLLSAI activation */
 while(RCC_GetFlagStatus(RCC_FLAG_PLLSAIRDY) == RESET)
 {
 }

  /* ʱ��������� */  
 /* ������ͬ���źſ��(HSW-1) */
 LTDC_InitStruct.LTDC_HorizontalSync =HSW-1;
 /* ���ô�ֱͬ���źſ��(VSW-1) */
 LTDC_InitStruct.LTDC_VerticalSync = VSW-1;
 /* ����(HSW+HBP-1) */
 LTDC_InitStruct.LTDC_AccumulatedHBP =HSW+HBP-1;
 /* ����(VSW+VBP-1) */
 LTDC_InitStruct.LTDC_AccumulatedVBP = VSW+VBP-1;
 /* ����(HSW+HBP+��Ч���ؿ��-1) */
 LTDC_InitStruct.LTDC_AccumulatedActiveW = HSW+HBP+LCD_PIXEL_WIDTH-1;
 /* ����(VSW+VBP+��Ч���ظ߶�-1) */
 LTDC_InitStruct.LTDC_AccumulatedActiveH = VSW+VBP+LCD_PIXEL_HEIGHT-1;
 /* �����ܿ��(HSW+HBP+��Ч���ؿ��+HFP-1) */
 LTDC_InitStruct.LTDC_TotalWidth =HSW+ HBP+LCD_PIXEL_WIDTH  + HFP-1; 
 /* �����ܸ߶�(VSW+VBP+��Ч���ظ߶�+VFP-1) */
 LTDC_InitStruct.LTDC_TotalHeigh =VSW+ VBP+LCD_PIXEL_HEIGHT  + VFP-1;

 LTDC_Init(&LTDC_InitStruct);
}

/**
 * @brief  Initializes the LCD Layers.
 * @param  None
 * @retval None
 */
void LCD_LayerInit(void)
{
 LTDC_Layer_InitTypeDef LTDC_Layer_InitStruct;

  /* �㴰������ */
  /* ���ñ���Ĵ��ڱ߽磬ע����Щ�����ǰ���HBP HSW VBP VSW�� */    
	//һ�еĵ�һ����ʼ���أ��ó�ԱֵӦ��Ϊ (LTDC_InitStruct.LTDC_AccumulatedHBP+1)��ֵ
	LTDC_Layer_InitStruct.LTDC_HorizontalStart = HBP + HSW;
	//һ�е����һ�����أ��ó�ԱֵӦ��Ϊ (LTDC_InitStruct.LTDC_AccumulatedActiveW)��ֵ
	LTDC_Layer_InitStruct.LTDC_HorizontalStop = HSW+HBP+LCD_PIXEL_WIDTH-1;
	//һ�еĵ�һ����ʼ���أ��ó�ԱֵӦ��Ϊ (LTDC_InitStruct.LTDC_AccumulatedVBP+1)��ֵ
	LTDC_Layer_InitStruct.LTDC_VerticalStart =  VBP + VSW;
	//һ�е����һ�����أ��ó�ԱֵӦ��Ϊ (LTDC_InitStruct.LTDC_AccumulatedActiveH)��ֵ
	LTDC_Layer_InitStruct.LTDC_VerticalStop = VSW+VBP+LCD_PIXEL_HEIGHT-1;

 /* Pixel Format configuration*/
 LTDC_Layer_InitStruct.LTDC_PixelFormat = LTDC_Pixelformat_ARGB1555;
 /* Alpha constant (255 totally opaque) */
 LTDC_Layer_InitStruct.LTDC_ConstantAlpha = 255;
 /* Default Color configuration (configure A,R,G,B component values) */
 LTDC_Layer_InitStruct.LTDC_DefaultColorBlue = 0;
 LTDC_Layer_InitStruct.LTDC_DefaultColorGreen = 0;
 LTDC_Layer_InitStruct.LTDC_DefaultColorRed = 0;
 LTDC_Layer_InitStruct.LTDC_DefaultColorAlpha = 0;
 /* Configure blending factors */
 LTDC_Layer_InitStruct.LTDC_BlendingFactor_1 = LTDC_BlendingFactor1_CA;
 LTDC_Layer_InitStruct.LTDC_BlendingFactor_2 = LTDC_BlendingFactor2_CA;

 /* the length of one line of pixels in bytes + 3 then :
 Line Lenth = Active high width x number of bytes per pixel + 3
 Active high width         = LCD_PIXEL_WIDTH
 number of bytes per pixel = 2    (pixel_format : RGB565)
 */
 LTDC_Layer_InitStruct.LTDC_CFBLineLength = ((LCD_PIXEL_WIDTH * 2) + 3);
 /* the pitch is the increment from the start of one line of pixels to the
 start of the next line in bytes, then :
 Pitch = Active high width x number of bytes per pixel */
 LTDC_Layer_InitStruct.LTDC_CFBPitch = (LCD_PIXEL_WIDTH * 2);

 /* Configure the number of lines */
 LTDC_Layer_InitStruct.LTDC_CFBLineNumber = LCD_PIXEL_HEIGHT;

 /* Start Address configuration : the LCD Frame buffer is defined on SDRAM */
 LTDC_Layer_InitStruct.LTDC_CFBStartAdress = LCD_FRAME_BUFFER;

 /* Initialize LTDC layer 1 */
 LTDC_LayerInit(LTDC_Layer1, &LTDC_Layer_InitStruct);

  /* Configure Layer2 */
 /* Pixel Format configuration*/
 LTDC_Layer_InitStruct.LTDC_PixelFormat = LTDC_Pixelformat_ARGB1555;
 
  /* Start Address configuration : the LCD Frame buffer is defined on SDRAM w/ Offset */
 LTDC_Layer_InitStruct.LTDC_CFBStartAdress = LCD_FRAME_BUFFER + BUFFER_OFFSET;

  /* Configure blending factors */
 LTDC_Layer_InitStruct.LTDC_BlendingFactor_1 = LTDC_BlendingFactor1_PAxCA;
 LTDC_Layer_InitStruct.LTDC_BlendingFactor_2 = LTDC_BlendingFactor2_PAxCA;

  /* Initialize LTDC layer 2 */
 LTDC_LayerInit(LTDC_Layer2, &LTDC_Layer_InitStruct);

  /* LTDC configuration reload */
 LTDC_ReloadConfig(LTDC_IMReload);

  /* Enable foreground & background Layers */
 LTDC_LayerCmd(LTDC_Layer1, ENABLE);
 LTDC_LayerCmd(LTDC_Layer2, ENABLE);

  /* LTDC configuration reload */
 LTDC_ReloadConfig(LTDC_IMReload);

  /* Set default font */
 LCD_SetFont(&LCD_DEFAULT_FONT);

  /* dithering activation */
 LTDC_DitherCmd(ENABLE);
}



/**
 * @brief  Sets the LCD Layer.
 * @param  Layerx: specifies the Layer foreground or background.
 * @retval None
 */
void LCD_SetLayer(uint32_t Layerx)
{
 if (Layerx == LCD_BACKGROUND_LAYER)
 {
   CurrentFrameBuffer = LCD_FRAME_BUFFER;
   CurrentLayer = LCD_BACKGROUND_LAYER;
 }
 else
 {
   CurrentFrameBuffer = LCD_FRAME_BUFFER + BUFFER_OFFSET;
   CurrentLayer = LCD_FOREGROUND_LAYER;
 }
}

/**
 * @brief  Sets the LCD Text and Background colors.
 * @param  TextColor: specifies the Text Color.
 * @param  BackColor: specifies the Background Color.
 * @retval None
 */
void LCD_SetColors(uint16_t TextColor, uint16_t BackColor)
{
 CurrentTextColor = TextColor;
 CurrentBackColor = BackColor;
}

/**
 * @brief  Gets the LCD Text and Background colors.
 * @param  TextColor: pointer to the variable that will contain the Text
           Color.
 * @param  BackColor: pointer to the variable that will contain the Background
           Color.
 * @retval None
 */
void LCD_GetColors(uint16_t *TextColor, uint16_t *BackColor)
{
 *TextColor = CurrentTextColor;
 *BackColor = CurrentBackColor;
}

/**
 * @brief  Sets the Text color.
 * @param  Color: specifies the Text color code RGB(5-6-5).
 * @retval None
 */
void LCD_SetTextColor(uint16_t Color)
{
 CurrentTextColor = Color;
}

/**
 * @brief  Sets the Background color.
 * @param  Color: specifies the Background color code RGB(5-6-5).
 * @retval None
 */
void LCD_SetBackColor(uint16_t Color)
{
 CurrentBackColor = Color;
}

/**
 * @brief  Sets the Text Font.
 * @param  fonts: specifies the font to be used.
 * @retval None
 */
void LCD_SetFont(sFONT *fonts)
{
 LCD_Currentfonts = fonts;
}

/**
 * @brief  Configure the transparency.
 * @param  transparency: specifies the transparency,
 *         This parameter must range from 0x00 to 0xFF.
 *         0x00 -> Transparent   0xFF -> Opaque
 * @retval None
 */
void LCD_SetTransparency(uint8_t transparency)
{
 if (CurrentLayer == LCD_BACKGROUND_LAYER)
 {
   LTDC_LayerAlpha(LTDC_Layer1, transparency);
 }
 else
 {
   LTDC_LayerAlpha(LTDC_Layer2, transparency);
 }
 LTDC_ReloadConfig(LTDC_IMReload);
}

/**
 * @brief  Gets the Text Font.
 * @param  None.
 * @retval the used font.
 */
sFONT *LCD_GetFont(void)
{
 return LCD_Currentfonts;
}

/**
 * @brief  Clears the selected line.
 * @param  Line: the Line to be cleared.
 *   This parameter can be one of the following values:
 *     @arg LCD_LINE_x: where x can be: 0..13 if LCD_Currentfonts is Font16x24
 *                                      0..26 if LCD_Currentfonts is Font12x12 or Font8x12
 *                                      0..39 if LCD_Currentfonts is Font8x8
 * @retval None
 */
void LCD_ClearLine(uint16_t Line)
{
 uint16_t refcolumn = 0;
 /* Send the string character by character on lCD */
 while ((refcolumn < LCD_PIXEL_WIDTH) && (((refcolumn + LCD_Currentfonts->Width)& 0xFFFF) >= LCD_Currentfonts->Width))
 {
   /* Display one character on LCD */
   LCD_DisplayChar(Line, refcolumn, ' ');
   /* Decrement the column position by 16 */
   refcolumn += LCD_Currentfonts->Width;
 }
}

/**
 * @brief  Clears the hole LCD.
 * @param  Color: the color of the background.
 * @retval None
 */
void LCD_Clear(uint16_t Color)
{
   
 DMA2D_InitTypeDef      DMA2D_InitStruct;

 uint16_t Red_Value = 0, Green_Value = 0, Blue_Value = 0;

 Red_Value = (0xF800 & Color) >> 11;
 Blue_Value = 0x001F & Color;
 Green_Value = (0x07E0 & Color) >> 5;


 /* configure DMA2D */
 DMA2D_DeInit();
 DMA2D_InitStruct.DMA2D_Mode = DMA2D_R2M;
 DMA2D_InitStruct.DMA2D_CMode = DMA2D_RGB565;
 DMA2D_InitStruct.DMA2D_OutputGreen = Green_Value;
 DMA2D_InitStruct.DMA2D_OutputBlue = Blue_Value;
 DMA2D_InitStruct.DMA2D_OutputRed = Red_Value;
 DMA2D_InitStruct.DMA2D_OutputAlpha = (Color&0x8000) ? 0xFF:0x00;		//����͸����
 DMA2D_InitStruct.DMA2D_OutputMemoryAdd = CurrentFrameBuffer;
 DMA2D_InitStruct.DMA2D_OutputOffset = 0;
 DMA2D_InitStruct.DMA2D_NumberOfLine = LCD_PIXEL_HEIGHT;
 DMA2D_InitStruct.DMA2D_PixelPerLine = LCD_PIXEL_WIDTH;
 DMA2D_Init(&DMA2D_InitStruct);

 /* Start Transfer */
 DMA2D_StartTransfer();

 /* Wait for CTC Flag activation */
 while(DMA2D_GetFlagStatus(DMA2D_FLAG_TC) == RESET)
 {
 }

 LCD_SetTextColor(CurrentTextColor);
}

/**
 * @brief  Sets the cursor position.
 * @param  Xpos: specifies the X position.
 * @param  Ypos: specifies the Y position.
 * @retval Display Address
 */
uint32_t LCD_SetCursor(uint16_t Xpos, uint16_t Ypos)
{
 return CurrentFrameBuffer + 2*(Xpos + (LCD_PIXEL_WIDTH*Ypos));
}

/**
 * @brief  Config and Sets the color Keying.
 * @param  RGBValue: Specifies the Color reference.
 * @retval None
 */
void LCD_SetColorKeying(uint32_t RGBValue)
{
 LTDC_ColorKeying_InitTypeDef   LTDC_colorkeying_InitStruct;

 /* configure the color Keying */
 LTDC_colorkeying_InitStruct.LTDC_ColorKeyBlue = 0x0000FF & RGBValue;
 LTDC_colorkeying_InitStruct.LTDC_ColorKeyGreen = (0x00FF00 & RGBValue) >> 8;
 LTDC_colorkeying_InitStruct.LTDC_ColorKeyRed = (0xFF0000 & RGBValue) >> 16;

 if (CurrentLayer == LCD_BACKGROUND_LAYER)
 {
   /* Enable the color Keying for Layer1 */
   LTDC_ColorKeyingConfig(LTDC_Layer1, &LTDC_colorkeying_InitStruct, ENABLE);
   LTDC_ReloadConfig(LTDC_IMReload);
 }
 else
 {
   /* Enable the color Keying for Layer2 */
   LTDC_ColorKeyingConfig(LTDC_Layer2, &LTDC_colorkeying_InitStruct, ENABLE);
   LTDC_ReloadConfig(LTDC_IMReload);
 }
}

/**
 * @brief  Disable the color Keying.
 * @param  RGBValue: Specifies the Color reference.
 * @retval None
 */
void LCD_ReSetColorKeying(void)
{
 LTDC_ColorKeying_InitTypeDef   LTDC_colorkeying_InitStruct;

 if (CurrentLayer == LCD_BACKGROUND_LAYER)
 {
   /* Disable the color Keying for Layer1 */
   LTDC_ColorKeyingConfig(LTDC_Layer1, &LTDC_colorkeying_InitStruct, DISABLE);
   LTDC_ReloadConfig(LTDC_IMReload);
 }
 else
 {
   /* Disable the color Keying for Layer2 */
   LTDC_ColorKeyingConfig(LTDC_Layer2, &LTDC_colorkeying_InitStruct, DISABLE);
   LTDC_ReloadConfig(LTDC_IMReload);
 }
}

/**
 * @brief  Draws a character on LCD.
 * @param  Xpos: the Line where to display the character shape.
 * @param  Ypos: start column address.
 * @param  c: pointer to the character data.
 * @retval None
 */
void LCD_DrawChar(uint16_t Xpos, uint16_t Ypos, const uint16_t *c)
{
 uint32_t index = 0, counter = 0, xpos =0;
 uint32_t  Xaddress = 0;

 xpos = Xpos*LCD_PIXEL_WIDTH*2;
 Xaddress += Ypos;

 for(index = 0; index < LCD_Currentfonts->Height; index++)
 {

   for(counter = 0; counter < LCD_Currentfonts->Width; counter++)
   {

     if((((c[index] & ((0x80 << ((LCD_Currentfonts->Width / 12 ) * 8 ) ) >> counter)) == 0x00) &&(LCD_Currentfonts->Width <= 12))||
       (((c[index] & (0x1 << counter)) == 0x00)&&(LCD_Currentfonts->Width > 12 )))
     {
         /* Write data value to all SDRAM memory */
        *(__IO uint16_t*) (CurrentFrameBuffer + (2*Xaddress) + xpos) = CurrentBackColor;
     }
     else
     {
         /* Write data value to all SDRAM memory */
        *(__IO uint16_t*) (CurrentFrameBuffer + (2*Xaddress) + xpos) = CurrentTextColor;
     }
     Xaddress++;
   }
     Xaddress += (LCD_PIXEL_WIDTH - LCD_Currentfonts->Width);
 }
}

/**
 * @brief  Displays one character (16dots width, 24dots height).
 * @param  Line: the Line where to display the character shape .
 *   This parameter can be one of the following values:
 *     @arg Linex: where x can be 0..29
 * @param  Column: start column address.
 * @param  Ascii: character ascii code, must be between 0x20 and 0x7E.
 * @retval None
 */
void LCD_DisplayChar(uint16_t Line, uint16_t Column, uint8_t Ascii)
{
 Ascii -= 32;

 LCD_DrawChar(Line, Column, &LCD_Currentfonts->table[Ascii * LCD_Currentfonts->Height]);
}

/**
 * @brief  Displays a maximum of 20 char on the LCD.
 * @param  Line: the Line where to display the character shape .
 *   This parameter can be one of the following values:
 *     @arg Linex: where x can be 0..9
 * @param  *ptr: pointer to string to display on LCD.
 * @retval None
 */
void LCD_DisplayStringLine(uint16_t Line, uint8_t *ptr)
{
 uint16_t refcolumn = 0;
 /* Send the string character by character on lCD */
 while ((refcolumn < LCD_PIXEL_WIDTH) && ((*ptr != 0) & (((refcolumn + LCD_Currentfonts->Width) & 0xFFFF) >= LCD_Currentfonts->Width)))
 {
   /* Display one character on LCD */
   LCD_DisplayChar(Line, refcolumn, *ptr);
   /* Decrement the column position by width */
   refcolumn += LCD_Currentfonts->Width;
   /* Point on the next character */
   ptr++;
 }
}

/**
 * @brief  Sets a display window
 * @param  Xpos: specifies the X bottom left position from 0 to 240.
 * @param  Ypos: specifies the Y bottom left position from 0 to 320.
 * @param  Height: display window height, can be a value from 0 to 320.
 * @param  Width: display window width, can be a value from 0 to 240.
 * @retval None
 */
void LCD_SetDisplayWindow(uint16_t Xpos, uint16_t Ypos, uint16_t Height, uint16_t Width)
{

 if (CurrentLayer == LCD_BACKGROUND_LAYER)
 {
   /* reconfigure the layer1 position */
   LTDC_LayerPosition(LTDC_Layer1, Xpos, Ypos);
   LTDC_ReloadConfig(LTDC_IMReload);

   /* reconfigure the layer1 size */
   LTDC_LayerSize(LTDC_Layer1, Width, Height);
   LTDC_ReloadConfig(LTDC_IMReload);
}
else
{
   /* reconfigure the layer2 position */
   LTDC_LayerPosition(LTDC_Layer2, Xpos, Ypos);
   LTDC_ReloadConfig(LTDC_IMReload);

  /* reconfigure the layer2 size */
   LTDC_LayerSize(LTDC_Layer2, Width, Height);
   LTDC_ReloadConfig(LTDC_IMReload);
 }
}

/**
 * @brief  Disables LCD Window mode.
 * @param  None
 * @retval None
 */
void LCD_WindowModeDisable(void)
{
 LCD_SetDisplayWindow(0, 0, LCD_PIXEL_HEIGHT, LCD_PIXEL_WIDTH);
}

/**
 * @brief  Displays a line.
 * @param Xpos: specifies the X position, can be a value from 0 to 240.
 * @param Ypos: specifies the Y position, can be a value from 0 to 320.
 * @param Length: line length.
 * @param Direction: line direction.
 *   This parameter can be one of the following values: LCD_DIR_HORIZONTAL or LCD_DIR_VERTICAL.
 * @retval None
 */
void LCD_DrawLine(uint16_t Xpos, uint16_t Ypos, uint16_t Length, uint8_t Direction)
{
 DMA2D_InitTypeDef      DMA2D_InitStruct;

 uint32_t  Xaddress = 0;
 uint16_t Red_Value = 0, Green_Value = 0, Blue_Value = 0;

 Xaddress = CurrentFrameBuffer + 2*(LCD_PIXEL_WIDTH*Ypos + Xpos);

 Red_Value = (0xF800 & CurrentTextColor) >> 11;
 Blue_Value = 0x001F & CurrentTextColor;
 Green_Value = (0x07E0 & CurrentTextColor) >> 5;

 /* Configure DMA2D */
 DMA2D_DeInit();
 DMA2D_InitStruct.DMA2D_Mode = DMA2D_R2M;
 DMA2D_InitStruct.DMA2D_CMode = DMA2D_RGB565;
 DMA2D_InitStruct.DMA2D_OutputGreen = Green_Value;
 DMA2D_InitStruct.DMA2D_OutputBlue = Blue_Value;
 DMA2D_InitStruct.DMA2D_OutputRed = Red_Value;
 DMA2D_InitStruct.DMA2D_OutputAlpha = 0x0F;
 DMA2D_InitStruct.DMA2D_OutputMemoryAdd = Xaddress;

 if(Direction == LCD_DIR_HORIZONTAL)
 {
   DMA2D_InitStruct.DMA2D_OutputOffset = 0;
   DMA2D_InitStruct.DMA2D_NumberOfLine = 1;
   DMA2D_InitStruct.DMA2D_PixelPerLine = Length;
 }
 else
 {
   DMA2D_InitStruct.DMA2D_OutputOffset = LCD_PIXEL_WIDTH - 1;
   DMA2D_InitStruct.DMA2D_NumberOfLine = Length;
   DMA2D_InitStruct.DMA2D_PixelPerLine = 1;
 }

 DMA2D_Init(&DMA2D_InitStruct);
 /* Start Transfer */
 DMA2D_StartTransfer();
 /* Wait for CTC Flag activation */
 while(DMA2D_GetFlagStatus(DMA2D_FLAG_TC) == RESET)
 {
 }

}

/**
 * @brief  Displays a rectangle.
 * @param  Xpos: specifies the X position, can be a value from 0 to 240.
 * @param  Ypos: specifies the Y position, can be a value from 0 to 320.
 * @param  Height: display rectangle height, can be a value from 0 to 320.
 * @param  Width: display rectangle width, can be a value from 0 to 240.
 * @retval None
 */
void LCD_DrawRect(uint16_t Xpos, uint16_t Ypos, uint16_t Width, uint16_t Height)
{
 /* draw horizontal lines */
 LCD_DrawLine(Xpos, Ypos, Width, LCD_DIR_HORIZONTAL);
 LCD_DrawLine(Xpos, (Ypos+ Height), Width, LCD_DIR_HORIZONTAL);

 /* draw vertical lines */
 LCD_DrawLine(Xpos, Ypos, Height, LCD_DIR_VERTICAL);
 LCD_DrawLine((Xpos + Width), Ypos, Height, LCD_DIR_VERTICAL);
}

/**
 * @brief  Draw a circle.
 * @param  Xpos: specifies the X position, can be a value from 0 to 240.
 * @param  Ypos: specifies the Y position, can be a value from 0 to 320.
 * @param  Radius: radius of the circle.
 * @retval None
 */
void LCD_DrawCircle(uint16_t Xpos, uint16_t Ypos, uint16_t Radius)
{
   int x = -Radius, y = 0, err = 2-2*Radius, e2;
   do {
       *(__IO uint16_t*) (CurrentFrameBuffer + (2*((Xpos-x) + LCD_PIXEL_WIDTH*(Ypos+y)))) = CurrentTextColor;
       *(__IO uint16_t*) (CurrentFrameBuffer + (2*((Xpos+x) + LCD_PIXEL_WIDTH*(Ypos+y)))) = CurrentTextColor;
       *(__IO uint16_t*) (CurrentFrameBuffer + (2*((Xpos+x) + LCD_PIXEL_WIDTH*(Ypos-y)))) = CurrentTextColor;
       *(__IO uint16_t*) (CurrentFrameBuffer + (2*((Xpos-x) + LCD_PIXEL_WIDTH*(Ypos-y)))) = CurrentTextColor;

       e2 = err;
       if (e2 <= y) {
           err += ++y*2+1;
           if (-x == y && e2 <= x) e2 = 0;
       }
       if (e2 > x) err += ++x*2+1;
   }
   while (x <= 0);
}

/**
 * @brief  Draw a full ellipse.
 * @param  Xpos: specifies the X position, can be a value from 0 to 240.
 * @param  Ypos: specifies the Y position, can be a value from 0 to 320.
 * @param  Radius: minor radius of ellipse.
 * @param  Radius2: major radius of ellipse.
 * @retval None
 */
void LCD_DrawFullEllipse(int Xpos, int Ypos, int Radius, int Radius2)
{
 int x = -Radius, y = 0, err = 2-2*Radius, e2;
 float K = 0, rad1 = 0, rad2 = 0;

 rad1 = Radius;
 rad2 = Radius2;

 if (Radius > Radius2)
 {
   do
   {
     K = (float)(rad1/rad2);
     LCD_DrawLine((Xpos+x), (Ypos-(uint16_t)(y/K)), (2*(uint16_t)(y/K) + 1), LCD_DIR_VERTICAL);
     LCD_DrawLine((Xpos-x), (Ypos-(uint16_t)(y/K)), (2*(uint16_t)(y/K) + 1), LCD_DIR_VERTICAL);

     e2 = err;
     if (e2 <= y)
     {
       err += ++y*2+1;
       if (-x == y && e2 <= x) e2 = 0;
     }
     if (e2 > x) err += ++x*2+1;

   }
   while (x <= 0);
 }
 else
 {
   y = -Radius2;
   x = 0;
   do
   {
     K = (float)(rad2/rad1);
     LCD_DrawLine((Xpos-(uint16_t)(x/K)), (Ypos+y), (2*(uint16_t)(x/K) + 1), LCD_DIR_HORIZONTAL);
     LCD_DrawLine((Xpos-(uint16_t)(x/K)), (Ypos-y), (2*(uint16_t)(x/K) + 1), LCD_DIR_HORIZONTAL);

     e2 = err;
     if (e2 <= x)
     {
       err += ++x*2+1;
       if (-y == x && e2 <= y) e2 = 0;
     }
     if (e2 > y) err += ++y*2+1;
   }
   while (y <= 0);
 }
}

/**
 * @brief  Displays an Ellipse.
 * @param  Xpos: specifies the X position, can be a value from 0 to 240.
 * @param  Ypos: specifies the Y position, can be a value from 0 to 320.
 * @param  Radius: specifies Radius.
 * @param  Radius2: specifies Radius2.
 * @retval None
 */
void LCD_DrawEllipse(int Xpos, int Ypos, int Radius, int Radius2)
{
 int x = -Radius, y = 0, err = 2-2*Radius, e2;
 float K = 0, rad1 = 0, rad2 = 0;

 rad1 = Radius;
 rad2 = Radius2;

 if (Radius > Radius2)
 {
   do {
     K = (float)(rad1/rad2);
     *(__IO uint16_t*) (CurrentFrameBuffer + (2*((Xpos-x) + LCD_PIXEL_WIDTH*(Ypos+(uint16_t)(y/K))))) = CurrentTextColor;
     *(__IO uint16_t*) (CurrentFrameBuffer + (2*((Xpos+x) + LCD_PIXEL_WIDTH*(Ypos+(uint16_t)(y/K))))) = CurrentTextColor;
     *(__IO uint16_t*) (CurrentFrameBuffer + (2*((Xpos+x) + LCD_PIXEL_WIDTH*(Ypos-(uint16_t)(y/K))))) = CurrentTextColor;
     *(__IO uint16_t*) (CurrentFrameBuffer + (2*((Xpos-x) + LCD_PIXEL_WIDTH*(Ypos-(uint16_t)(y/K))))) = CurrentTextColor;

     e2 = err;
     if (e2 <= y) {
       err += ++y*2+1;
       if (-x == y && e2 <= x) e2 = 0;
     }
     if (e2 > x) err += ++x*2+1;
   }
   while (x <= 0);
 }
 else
 {
   y = -Radius2;
   x = 0;
   do {
     K = (float)(rad2/rad1);
     *(__IO uint16_t*) (CurrentFrameBuffer + (2*((Xpos-(uint16_t)(x/K)) + LCD_PIXEL_WIDTH*(Ypos+y)))) = CurrentTextColor;
     *(__IO uint16_t*) (CurrentFrameBuffer + (2*((Xpos+(uint16_t)(x/K)) + LCD_PIXEL_WIDTH*(Ypos+y)))) = CurrentTextColor;
     *(__IO uint16_t*) (CurrentFrameBuffer + (2*((Xpos+(uint16_t)(x/K)) + LCD_PIXEL_WIDTH*(Ypos-y)))) = CurrentTextColor;
     *(__IO uint16_t*) (CurrentFrameBuffer + (2*((Xpos-(uint16_t)(x/K)) + LCD_PIXEL_WIDTH*(Ypos-y)))) = CurrentTextColor;

     e2 = err;
     if (e2 <= x) {
       err += ++x*2+1;
       if (-y == x && e2 <= y) e2 = 0;
     }
     if (e2 > y) err += ++y*2+1;
   }
   while (y <= 0);
 }
}

/**
 * @brief  Displays a mono-color picture.
 * @param  Pict: pointer to the picture array.
 * @retval None
 */
void LCD_DrawMonoPict(const uint32_t *Pict)
{
 uint32_t index = 0, counter = 0;


 for(index = 0; index < 2400; index++)
 {
   for(counter = 0; counter < 32; counter++)
   {
     if((Pict[index] & (1 << counter)) == 0x00)
     {
       *(__IO uint16_t*)(CurrentFrameBuffer) = CurrentBackColor;
     }
     else
     {
       *(__IO uint16_t*)(CurrentFrameBuffer) = CurrentTextColor;
     }
   }
 }
}

/**
 * @brief  Displays a bitmap picture loaded in the internal Flash.
 * @param  BmpAddress: Bmp picture address in the internal Flash.
 * @retval None
 */
void LCD_WriteBMP(uint32_t BmpAddress)
{
 uint32_t bisize=0,index = 0, size = 0, width = 0, height = 0, bit_pixel = 0,picsize=0;
 uint32_t Address;
 uint32_t currentline = 0, linenumber = 0;
 uint16_t data=0;
 
 Address = CurrentFrameBuffer;

 /* Read bitmap size */
 size = *(__IO uint16_t *) (BmpAddress + 2);
 size |= (*(__IO uint16_t *) (BmpAddress + 4)) << 16;	
 printf("bmp->size: %d\n",size);
	
 data= *(__IO uint16_t *) (BmpAddress + size-2);
	printf("bmp->data: %04X\n",data);
	
 /* Get bitmap data address offset */
 index = *(__IO uint16_t *) (BmpAddress + 10);
 index |= (*(__IO uint16_t *) (BmpAddress + 12)) << 16;
 printf("bmp->index: %d\n",index);
	
 /* Read bisize */
 bisize = *(uint16_t *) (BmpAddress + 14);
 bisize |= (*(uint16_t *) (BmpAddress + 16)) << 16;
 printf("bmp->bisize: %d\n",bisize);
	
 /* Read bitmap width */
 width = *(uint16_t *) (BmpAddress + 18);
 width |= (*(uint16_t *) (BmpAddress + 20)) << 16;
 printf("bmp->width: %d\n",width);
	
 /* Read bitmap height */
 height = *(uint16_t *) (BmpAddress + 22);
 height |= (*(uint16_t *) (BmpAddress + 24)) << 16;
 printf("bmp->height: %d\n",height);
 
 /* Read bit/pixel */
 bit_pixel = *(uint16_t *) (BmpAddress + 28);
 printf("bmp->bit_pixel: %d\n",bit_pixel);
 
 /* Read bitmap height */
 picsize = *(uint16_t *) (BmpAddress + 34);
 picsize |= (*(uint16_t *) (BmpAddress + 36)) << 16;
 printf("bmp->picsize: %d\n",picsize);
 
 if (CurrentLayer == LCD_BACKGROUND_LAYER)
 {
   /* reconfigure layer size in accordance with the picture */
   LTDC_LayerSize(LTDC_Layer1, width, height);
   LTDC_ReloadConfig(LTDC_VBReload);

   /* Reconfigure the Layer pixel format in accordance with the picture */
   if ((bit_pixel/8) == 4)
   {
     LTDC_LayerPixelFormat(LTDC_Layer1, LTDC_Pixelformat_ARGB8888);
     LTDC_ReloadConfig(LTDC_VBReload);
   }
   else if ((bit_pixel/8) == 2)
   {
	 if((bisize==56)&& (data==0))
	   LTDC_LayerPixelFormat(LTDC_Layer1, LTDC_Pixelformat_RGB565);
	 else
       LTDC_LayerPixelFormat(LTDC_Layer1, LTDC_Pixelformat_ARGB1555);
     LTDC_ReloadConfig(LTDC_VBReload);
   }
   else
   {
     LTDC_LayerPixelFormat(LTDC_Layer1, LTDC_Pixelformat_RGB888);
     LTDC_ReloadConfig(LTDC_VBReload);
   }
 }
 else
 {
   /* reconfigure layer size in accordance with the picture */
   LTDC_LayerSize(LTDC_Layer2, width, height);
   LTDC_ReloadConfig(LTDC_VBReload);

   /* Reconfigure the Layer pixel format in accordance with the picture */
   if ((bit_pixel/8) == 4)
   {
     LTDC_LayerPixelFormat(LTDC_Layer2, LTDC_Pixelformat_ARGB8888);
     LTDC_ReloadConfig(LTDC_VBReload);
   }
   else if ((bit_pixel/8) == 2)
   {
     if((bisize==56)&& (data==0))
		LTDC_LayerPixelFormat(LTDC_Layer2, LTDC_Pixelformat_RGB565);
	  else
        LTDC_LayerPixelFormat(LTDC_Layer2, LTDC_Pixelformat_ARGB1555);
     LTDC_ReloadConfig(LTDC_VBReload);
   }
   else
   {
     LTDC_LayerPixelFormat(LTDC_Layer2, LTDC_Pixelformat_RGB888);
     LTDC_ReloadConfig(LTDC_VBReload);
   }
 }
 /* compute the real size of the picture (without the header)) */
 if(picsize && (data==0))
	size = (size - index-2);
 else
	 size = (size - index);

 /* bypass the bitmap header */
 BmpAddress += index;

 /* start copie image from the bottom */
 Address += width*(height-1)*(bit_pixel/8);

 for(index = 0; index < size; index++)
 {
   *(__IO uint8_t*) (Address) = *(__IO uint8_t *)BmpAddress;
   /*jump on next byte */
   BmpAddress++;
   Address++;
   currentline++;

   if((currentline/(bit_pixel/8)) == width)
   {
     if(linenumber < height)
     {
       linenumber++;
       Address -=(2*width*(bit_pixel/8));
       currentline = 0;
     }
   }
 }
}

/**
 * @brief  Displays a full rectangle.
 * @param  Xpos: specifies the X position, can be a value from 0 to 240.
 * @param  Ypos: specifies the Y position, can be a value from 0 to 320.
 * @param  Height: rectangle height.
 * @param  Width: rectangle width.
 * @retval None
 */
void LCD_DrawFullRect(uint16_t Xpos, uint16_t Ypos, uint16_t Width, uint16_t Height)
{
 DMA2D_InitTypeDef      DMA2D_InitStruct;

 uint32_t  Xaddress = 0;
 uint16_t Red_Value = 0, Green_Value = 0, Blue_Value = 0;

 Red_Value = (0xF800 & CurrentTextColor) >> 11;
 Blue_Value = 0x001F & CurrentTextColor;
 Green_Value = (0x07E0 & CurrentTextColor) >> 5;

 Xaddress = CurrentFrameBuffer + 2*(LCD_PIXEL_WIDTH*Ypos + Xpos);

 /* configure DMA2D */
 DMA2D_DeInit();
 DMA2D_InitStruct.DMA2D_Mode = DMA2D_R2M;
 DMA2D_InitStruct.DMA2D_CMode = DMA2D_RGB565;
 DMA2D_InitStruct.DMA2D_OutputGreen = Green_Value;
 DMA2D_InitStruct.DMA2D_OutputBlue = Blue_Value;
 DMA2D_InitStruct.DMA2D_OutputRed = Red_Value;
 DMA2D_InitStruct.DMA2D_OutputAlpha = 0x0F;
 DMA2D_InitStruct.DMA2D_OutputMemoryAdd = Xaddress;
 DMA2D_InitStruct.DMA2D_OutputOffset = (LCD_PIXEL_WIDTH - Width);
 DMA2D_InitStruct.DMA2D_NumberOfLine = Height;
 DMA2D_InitStruct.DMA2D_PixelPerLine = Width;
 DMA2D_Init(&DMA2D_InitStruct);

 /* Start Transfer */
 DMA2D_StartTransfer();

 /* Wait for CTC Flag activation */
 while(DMA2D_GetFlagStatus(DMA2D_FLAG_TC) == RESET)
 {
 }

 LCD_SetTextColor(CurrentTextColor);
}

/**
 * @brief  Displays a full circle.
 * @param  Xpos: specifies the X position, can be a value from 0 to 240.
 * @param  Ypos: specifies the Y position, can be a value from 0 to 320.
 * @param  Radius
 * @retval None
 */
void LCD_DrawFullCircle(uint16_t Xpos, uint16_t Ypos, uint16_t Radius)
{
 int32_t  D;    /* Decision Variable */
 uint32_t  CurX;/* Current X Value */
 uint32_t  CurY;/* Current Y Value */

 D = 3 - (Radius << 1);

 CurX = 0;
 CurY = Radius;

 while (CurX <= CurY)
 {
   if(CurY > 0)
   {
     LCD_DrawLine(Xpos - CurX, Ypos - CurY, 2*CurY, LCD_DIR_VERTICAL);
     LCD_DrawLine(Xpos + CurX, Ypos - CurY, 2*CurY, LCD_DIR_VERTICAL);
   }

   if(CurX > 0)
   {
     LCD_DrawLine(Xpos - CurY, Ypos - CurX, 2*CurX, LCD_DIR_VERTICAL);
     LCD_DrawLine(Xpos + CurY, Ypos - CurX, 2*CurX, LCD_DIR_VERTICAL);
   }
   if (D < 0)
   {
     D += (CurX << 2) + 6;
   }
   else
   {
     D += ((CurX - CurY) << 2) + 10;
     CurY--;
   }
   CurX++;
 }

 LCD_DrawCircle(Xpos, Ypos, Radius);
}

/**
 * @brief  Displays an uni-line (between two points).
 * @param  x1: specifies the point 1 x position.
 * @param  y1: specifies the point 1 y position.
 * @param  x2: specifies the point 2 x position.
 * @param  y2: specifies the point 2 y position.
 * @retval None
 */
void LCD_DrawUniLine(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2)
{
 int16_t deltax = 0, deltay = 0, x = 0, y = 0, xinc1 = 0, xinc2 = 0,
 yinc1 = 0, yinc2 = 0, den = 0, num = 0, numadd = 0, numpixels = 0,
 curpixel = 0;

 deltax = ABS(x2 - x1);        /* The difference between the x's */
 deltay = ABS(y2 - y1);        /* The difference between the y's */
 x = x1;                       /* Start x off at the first pixel */
 y = y1;                       /* Start y off at the first pixel */

 if (x2 >= x1)                 /* The x-values are increasing */
 {
   xinc1 = 1;
   xinc2 = 1;
 }
 else                          /* The x-values are decreasing */
 {
   xinc1 = -1;
   xinc2 = -1;
 }

 if (y2 >= y1)                 /* The y-values are increasing */
 {
   yinc1 = 1;
   yinc2 = 1;
 }
 else                          /* The y-values are decreasing */
 {
   yinc1 = -1;
   yinc2 = -1;
 }

 if (deltax >= deltay)         /* There is at least one x-value for every y-value */
 {
   xinc1 = 0;                  /* Don't change the x when numerator >= denominator */
   yinc2 = 0;                  /* Don't change the y for every iteration */
   den = deltax;
   num = deltax / 2;
   numadd = deltay;
   numpixels = deltax;         /* There are more x-values than y-values */
 }
 else                          /* There is at least one y-value for every x-value */
 {
   xinc2 = 0;                  /* Don't change the x for every iteration */
   yinc1 = 0;                  /* Don't change the y when numerator >= denominator */
   den = deltay;
   num = deltay / 2;
   numadd = deltax;
   numpixels = deltay;         /* There are more y-values than x-values */
 }

 for (curpixel = 0; curpixel <= numpixels; curpixel++)
 {
   PutPixel(x, y);             /* Draw the current pixel */
   num += numadd;              /* Increase the numerator by the top of the fraction */
   if (num >= den)             /* Check if numerator >= denominator */
   {
     num -= den;               /* Calculate the new numerator value */
     x += xinc1;               /* Change the x as appropriate */
     y += yinc1;               /* Change the y as appropriate */
   }
   x += xinc2;                 /* Change the x as appropriate */
   y += yinc2;                 /* Change the y as appropriate */
 }
}

/**
 * @brief  Displays an triangle.
 * @param  Points: pointer to the points array.
 * @retval None
 */
void LCD_Triangle(pPoint Points, uint16_t PointCount)
{
 int16_t X = 0, Y = 0;
 pPoint First = Points;

 if(PointCount != 3)
 {
   return;
 }

 while(--PointCount)
 {
   X = Points->X;
   Y = Points->Y;
   Points++;
   LCD_DrawUniLine(X, Y, Points->X, Points->Y);
 }
 LCD_DrawUniLine(First->X, First->Y, Points->X, Points->Y);
}

/**
 * @brief  Fill an triangle (between 3 points).
 * @param  x1..3: x position of triangle point 1..3.
 * @param  y1..3: y position of triangle point 1..3.
 * @retval None
 */
void LCD_FillTriangle(uint16_t x1, uint16_t x2, uint16_t x3, uint16_t y1, uint16_t y2, uint16_t y3)
{

 int16_t deltax = 0, deltay = 0, x = 0, y = 0, xinc1 = 0, xinc2 = 0,
 yinc1 = 0, yinc2 = 0, den = 0, num = 0, numadd = 0, numpixels = 0,
 curpixel = 0;

 deltax = ABS(x2 - x1);        /* The difference between the x's */
 deltay = ABS(y2 - y1);        /* The difference between the y's */
 x = x1;                       /* Start x off at the first pixel */
 y = y1;                       /* Start y off at the first pixel */

 if (x2 >= x1)                 /* The x-values are increasing */
 {
   xinc1 = 1;
   xinc2 = 1;
 }
 else                          /* The x-values are decreasing */
 {
   xinc1 = -1;
   xinc2 = -1;
 }

 if (y2 >= y1)                 /* The y-values are increasing */
 {
   yinc1 = 1;
   yinc2 = 1;
 }
 else                          /* The y-values are decreasing */
 {
   yinc1 = -1;
   yinc2 = -1;
 }

 if (deltax >= deltay)         /* There is at least one x-value for every y-value */
 {
   xinc1 = 0;                  /* Don't change the x when numerator >= denominator */
   yinc2 = 0;                  /* Don't change the y for every iteration */
   den = deltax;
   num = deltax / 2;
   numadd = deltay;
   numpixels = deltax;         /* There are more x-values than y-values */
 }
 else                          /* There is at least one y-value for every x-value */
 {
   xinc2 = 0;                  /* Don't change the x for every iteration */
   yinc1 = 0;                  /* Don't change the y when numerator >= denominator */
   den = deltay;
   num = deltay / 2;
   numadd = deltax;
   numpixels = deltay;         /* There are more y-values than x-values */
 }

 for (curpixel = 0; curpixel <= numpixels; curpixel++)
 {
   LCD_DrawUniLine(x, y, x3, y3);

   num += numadd;              /* Increase the numerator by the top of the fraction */
   if (num >= den)             /* Check if numerator >= denominator */
   {
     num -= den;               /* Calculate the new numerator value */
     x += xinc1;               /* Change the x as appropriate */
     y += yinc1;               /* Change the y as appropriate */
   }
   x += xinc2;                 /* Change the x as appropriate */
   y += yinc2;                 /* Change the y as appropriate */
 }


}
/**
 * @brief  Displays an poly-line (between many points).
 * @param  Points: pointer to the points array.
 * @param  PointCount: Number of points.
 * @retval None
 */
void LCD_PolyLine(pPoint Points, uint16_t PointCount)
{
 int16_t X = 0, Y = 0;

 if(PointCount < 2)
 {
   return;
 }

 while(--PointCount)
 {
   X = Points->X;
   Y = Points->Y;
   Points++;
   LCD_DrawUniLine(X, Y, Points->X, Points->Y);
 }
}

/**
 * @brief  Displays an relative poly-line (between many points).
 * @param  Points: pointer to the points array.
 * @param  PointCount: Number of points.
 * @param  Closed: specifies if the draw is closed or not.
 *           1: closed, 0 : not closed.
 * @retval None
 */
static void LCD_PolyLineRelativeClosed(pPoint Points, uint16_t PointCount, uint16_t Closed)
{
 int16_t X = 0, Y = 0;
 pPoint First = Points;

 if(PointCount < 2)
 {
   return;
 }
 X = Points->X;
 Y = Points->Y;
 while(--PointCount)
 {
   Points++;
   LCD_DrawUniLine(X, Y, X + Points->X, Y + Points->Y);
   X = X + Points->X;
   Y = Y + Points->Y;
 }
 if(Closed)
 {
   LCD_DrawUniLine(First->X, First->Y, X, Y);
 }
}

/**
 * @brief  Displays a closed poly-line (between many points).
 * @param  Points: pointer to the points array.
 * @param  PointCount: Number of points.
 * @retval None
 */
void LCD_ClosedPolyLine(pPoint Points, uint16_t PointCount)
{
 LCD_PolyLine(Points, PointCount);
 LCD_DrawUniLine(Points->X, Points->Y, (Points+PointCount-1)->X, (Points+PointCount-1)->Y);
}

/**
 * @brief  Displays a relative poly-line (between many points).
 * @param  Points: pointer to the points array.
 * @param  PointCount: Number of points.
 * @retval None
 */
void LCD_PolyLineRelative(pPoint Points, uint16_t PointCount)
{
 LCD_PolyLineRelativeClosed(Points, PointCount, 0);
}

/**
 * @brief  Displays a closed relative poly-line (between many points).
 * @param  Points: pointer to the points array.
 * @param  PointCount: Number of points.
 * @retval None
 */
void LCD_ClosedPolyLineRelative(pPoint Points, uint16_t PointCount)
{
 LCD_PolyLineRelativeClosed(Points, PointCount, 1);
}

/**
 * @brief  Displays a  full poly-line (between many points).
 * @param  Points: pointer to the points array.
 * @param  PointCount: Number of points.
 * @retval None
 */
void LCD_FillPolyLine(pPoint Points, uint16_t PointCount)
{

 int16_t X = 0, Y = 0, X2 = 0, Y2 = 0, X_center = 0, Y_center = 0, X_first = 0, Y_first = 0, pixelX = 0, pixelY = 0, counter = 0;
 uint16_t  IMAGE_LEFT = 0, IMAGE_RIGHT = 0, IMAGE_TOP = 0, IMAGE_BOTTOM = 0;

 IMAGE_LEFT = IMAGE_RIGHT = Points->X;
 IMAGE_TOP= IMAGE_BOTTOM = Points->Y;

 for(counter = 1; counter < PointCount; counter++)
 {
   pixelX = POLY_X(counter);
   if(pixelX < IMAGE_LEFT)
   {
     IMAGE_LEFT = pixelX;
   }
   if(pixelX > IMAGE_RIGHT)
   {
     IMAGE_RIGHT = pixelX;
   }

   pixelY = POLY_Y(counter);
   if(pixelY < IMAGE_TOP)
   {
     IMAGE_TOP = pixelY;
   }
   if(pixelY > IMAGE_BOTTOM)
   {
     IMAGE_BOTTOM = pixelY;
   }
 }

 if(PointCount < 2)
 {
   return;
 }

 X_center = (IMAGE_LEFT + IMAGE_RIGHT)/2;
 Y_center = (IMAGE_BOTTOM + IMAGE_TOP)/2;

 X_first = Points->X;
 Y_first = Points->Y;

 while(--PointCount)
 {
   X = Points->X;
   Y = Points->Y;
   Points++;
   X2 = Points->X;
   Y2 = Points->Y;

   LCD_FillTriangle(X, X2, X_center, Y, Y2, Y_center);
   LCD_FillTriangle(X, X_center, X2, Y, Y_center, Y2);
   LCD_FillTriangle(X_center, X2, X, Y_center, Y2, Y);
 }

 LCD_FillTriangle(X_first, X2, X_center, Y_first, Y2, Y_center);
 LCD_FillTriangle(X_first, X_center, X2, Y_first, Y_center, Y2);
 LCD_FillTriangle(X_center, X2, X_first, Y_center, Y2, Y_first);
}




/**
 * @brief  Sets or reset LCD control lines.
 * @param  GPIOx: where x can be B or D to select the GPIO peripheral.
 * @param  CtrlPins: the Control line.
 *   This parameter can be:
 *     @arg LCD_NCS_PIN: Chip Select pin
 *     @arg LCD_NWR_PIN: Read/Write Selection pin
 *     @arg LCD_RS_PIN: Register/RAM Selection pin
 * @param  BitVal: specifies the value to be written to the selected bit.
 *   This parameter can be:
 *     @arg Bit_RESET: to clear the port pin
 *     @arg Bit_SET: to set the port pin
 * @retval None
 */
void LCD_CtrlLinesWrite(GPIO_TypeDef* GPIOx, uint16_t CtrlPins, BitAction BitVal)
{
 /* Set or Reset the control line */
 GPIO_WriteBit(GPIOx, (uint16_t)CtrlPins, (BitAction)BitVal);
}




static void LCD_GPIO_Config(void)
{
 GPIO_InitTypeDef GPIO_InitStruct;

 /* ʹ��LCDʹ�õ�������ʱ�� */
                         //��ɫ������
 RCC_AHB1PeriphClockCmd(LTDC_R0_GPIO_CLK | LTDC_R1_GPIO_CLK | LTDC_R2_GPIO_CLK|
                        LTDC_R3_GPIO_CLK | LTDC_R4_GPIO_CLK | LTDC_R5_GPIO_CLK|
                        LTDC_R6_GPIO_CLK | LTDC_R7_GPIO_CLK |
                         //��ɫ������
                         LTDC_G0_GPIO_CLK|LTDC_G1_GPIO_CLK|LTDC_G2_GPIO_CLK|
                         LTDC_G3_GPIO_CLK|LTDC_G4_GPIO_CLK|LTDC_G5_GPIO_CLK|
                         LTDC_G6_GPIO_CLK|LTDC_G7_GPIO_CLK|
                         //��ɫ������
                         LTDC_B0_GPIO_CLK|LTDC_B1_GPIO_CLK|LTDC_B2_GPIO_CLK|
                         LTDC_B3_GPIO_CLK|LTDC_B4_GPIO_CLK|LTDC_B5_GPIO_CLK|
                         LTDC_B6_GPIO_CLK|LTDC_B7_GPIO_CLK|
                         //�����ź���
                         LTDC_CLK_GPIO_CLK | LTDC_HSYNC_GPIO_CLK |LTDC_VSYNC_GPIO_CLK|
                         LTDC_DE_GPIO_CLK  | LTDC_BL_GPIO_CLK    |LTDC_DISP_GPIO_CLK ,ENABLE);


/* GPIO���� */

/* ��ɫ������ */
 GPIO_InitStruct.GPIO_Pin = LTDC_R0_GPIO_PIN;
 GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
 GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
 GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
 GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;

 GPIO_Init(LTDC_R0_GPIO_PORT, &GPIO_InitStruct);
 GPIO_PinAFConfig(LTDC_R0_GPIO_PORT, LTDC_R0_PINSOURCE, LTDC_R0_AF);

 GPIO_InitStruct.GPIO_Pin = LTDC_R1_GPIO_PIN;
 GPIO_Init(LTDC_R1_GPIO_PORT, &GPIO_InitStruct);
 GPIO_PinAFConfig(LTDC_R1_GPIO_PORT, LTDC_R1_PINSOURCE, LTDC_R1_AF);

 GPIO_InitStruct.GPIO_Pin = LTDC_R2_GPIO_PIN;
 GPIO_Init(LTDC_R2_GPIO_PORT, &GPIO_InitStruct);
 GPIO_PinAFConfig(LTDC_R2_GPIO_PORT, LTDC_R2_PINSOURCE, LTDC_R2_AF);

 GPIO_InitStruct.GPIO_Pin = LTDC_R3_GPIO_PIN;
 GPIO_Init(LTDC_R3_GPIO_PORT, &GPIO_InitStruct);
 GPIO_PinAFConfig(LTDC_R3_GPIO_PORT, LTDC_R3_PINSOURCE, LTDC_R3_AF);

 GPIO_InitStruct.GPIO_Pin = LTDC_R4_GPIO_PIN;
 GPIO_Init(LTDC_R4_GPIO_PORT, &GPIO_InitStruct);
 GPIO_PinAFConfig(LTDC_R4_GPIO_PORT, LTDC_R4_PINSOURCE, LTDC_R4_AF);

 GPIO_InitStruct.GPIO_Pin = LTDC_R5_GPIO_PIN;
 GPIO_Init(LTDC_R5_GPIO_PORT, &GPIO_InitStruct);
 GPIO_PinAFConfig(LTDC_R5_GPIO_PORT, LTDC_R5_PINSOURCE, LTDC_R5_AF);

 GPIO_InitStruct.GPIO_Pin = LTDC_R6_GPIO_PIN;
 GPIO_Init(LTDC_R6_GPIO_PORT, &GPIO_InitStruct);
 GPIO_PinAFConfig(LTDC_R6_GPIO_PORT, LTDC_R6_PINSOURCE, LTDC_R6_AF);

 GPIO_InitStruct.GPIO_Pin = LTDC_R7_GPIO_PIN;
 GPIO_Init(LTDC_R7_GPIO_PORT, &GPIO_InitStruct);
 GPIO_PinAFConfig(LTDC_R7_GPIO_PORT, LTDC_R7_PINSOURCE, LTDC_R7_AF);

 //��ɫ������
 GPIO_InitStruct.GPIO_Pin = LTDC_G0_GPIO_PIN;
 GPIO_Init(LTDC_G0_GPIO_PORT, &GPIO_InitStruct);
 GPIO_PinAFConfig(LTDC_G0_GPIO_PORT, LTDC_G0_PINSOURCE, LTDC_G0_AF);

 GPIO_InitStruct.GPIO_Pin = LTDC_G1_GPIO_PIN;
 GPIO_Init(LTDC_G1_GPIO_PORT, &GPIO_InitStruct);
 GPIO_PinAFConfig(LTDC_G1_GPIO_PORT, LTDC_G1_PINSOURCE, LTDC_G1_AF);

 GPIO_InitStruct.GPIO_Pin = LTDC_G2_GPIO_PIN;
 GPIO_Init(LTDC_G2_GPIO_PORT, &GPIO_InitStruct);
 GPIO_PinAFConfig(LTDC_G2_GPIO_PORT, LTDC_G2_PINSOURCE, LTDC_G2_AF);

 GPIO_InitStruct.GPIO_Pin = LTDC_G3_GPIO_PIN;
 GPIO_Init(LTDC_G3_GPIO_PORT, &GPIO_InitStruct);
 GPIO_PinAFConfig(LTDC_G3_GPIO_PORT, LTDC_G3_PINSOURCE, LTDC_G3_AF);

 GPIO_InitStruct.GPIO_Pin = LTDC_G4_GPIO_PIN;
 GPIO_Init(LTDC_G4_GPIO_PORT, &GPIO_InitStruct);
 GPIO_PinAFConfig(LTDC_G4_GPIO_PORT, LTDC_G4_PINSOURCE, LTDC_G4_AF);

 GPIO_InitStruct.GPIO_Pin = LTDC_G5_GPIO_PIN;
 GPIO_Init(LTDC_G5_GPIO_PORT, &GPIO_InitStruct);
 GPIO_PinAFConfig(LTDC_G5_GPIO_PORT, LTDC_G5_PINSOURCE, LTDC_G5_AF);

 GPIO_InitStruct.GPIO_Pin = LTDC_G6_GPIO_PIN;
 GPIO_Init(LTDC_G6_GPIO_PORT, &GPIO_InitStruct);
 GPIO_PinAFConfig(LTDC_G6_GPIO_PORT, LTDC_G6_PINSOURCE, LTDC_G6_AF);

 GPIO_InitStruct.GPIO_Pin = LTDC_G7_GPIO_PIN;
 GPIO_Init(LTDC_G7_GPIO_PORT, &GPIO_InitStruct);
 GPIO_PinAFConfig(LTDC_G7_GPIO_PORT, LTDC_G7_PINSOURCE, LTDC_G7_AF);

 //��ɫ������
 GPIO_InitStruct.GPIO_Pin = LTDC_B0_GPIO_PIN;
 GPIO_Init(LTDC_B0_GPIO_PORT, &GPIO_InitStruct);
 GPIO_PinAFConfig(LTDC_B0_GPIO_PORT, LTDC_B0_PINSOURCE, LTDC_B0_AF);

 GPIO_InitStruct.GPIO_Pin = LTDC_B1_GPIO_PIN;
 GPIO_Init(LTDC_B1_GPIO_PORT, &GPIO_InitStruct);
 GPIO_PinAFConfig(LTDC_B1_GPIO_PORT, LTDC_B1_PINSOURCE, LTDC_B1_AF);

 GPIO_InitStruct.GPIO_Pin = LTDC_B2_GPIO_PIN;
 GPIO_Init(LTDC_B2_GPIO_PORT, &GPIO_InitStruct);
 GPIO_PinAFConfig(LTDC_B2_GPIO_PORT, LTDC_B2_PINSOURCE, LTDC_B2_AF);

 GPIO_InitStruct.GPIO_Pin = LTDC_B3_GPIO_PIN;
 GPIO_Init(LTDC_B3_GPIO_PORT, &GPIO_InitStruct);
 GPIO_PinAFConfig(LTDC_B3_GPIO_PORT, LTDC_B3_PINSOURCE, LTDC_B3_AF);

 GPIO_InitStruct.GPIO_Pin = LTDC_B4_GPIO_PIN;
 GPIO_Init(LTDC_B4_GPIO_PORT, &GPIO_InitStruct);
 GPIO_PinAFConfig(LTDC_B4_GPIO_PORT, LTDC_B4_PINSOURCE, LTDC_B4_AF);

 GPIO_InitStruct.GPIO_Pin = LTDC_B5_GPIO_PIN;
 GPIO_Init(LTDC_B5_GPIO_PORT, &GPIO_InitStruct);
 GPIO_PinAFConfig(LTDC_B5_GPIO_PORT, LTDC_B5_PINSOURCE, LTDC_B5_AF);

 GPIO_InitStruct.GPIO_Pin = LTDC_B6_GPIO_PIN;
 GPIO_Init(LTDC_B6_GPIO_PORT, &GPIO_InitStruct);
 GPIO_PinAFConfig(LTDC_B6_GPIO_PORT, LTDC_B6_PINSOURCE, LTDC_B6_AF);

 GPIO_InitStruct.GPIO_Pin = LTDC_B7_GPIO_PIN;
 GPIO_Init(LTDC_B7_GPIO_PORT, &GPIO_InitStruct);
 GPIO_PinAFConfig(LTDC_B7_GPIO_PORT, LTDC_B7_PINSOURCE, LTDC_B7_AF);

 //�����ź���
 GPIO_InitStruct.GPIO_Pin = LTDC_CLK_GPIO_PIN;
 GPIO_Init(LTDC_CLK_GPIO_PORT, &GPIO_InitStruct);
 GPIO_PinAFConfig(LTDC_CLK_GPIO_PORT, LTDC_CLK_PINSOURCE, LTDC_CLK_AF);

 GPIO_InitStruct.GPIO_Pin = LTDC_HSYNC_GPIO_PIN;
 GPIO_Init(LTDC_HSYNC_GPIO_PORT, &GPIO_InitStruct);
 GPIO_PinAFConfig(LTDC_HSYNC_GPIO_PORT, LTDC_HSYNC_PINSOURCE, LTDC_HSYNC_AF);

 GPIO_InitStruct.GPIO_Pin = LTDC_VSYNC_GPIO_PIN;
 GPIO_Init(LTDC_VSYNC_GPIO_PORT, &GPIO_InitStruct);
 GPIO_PinAFConfig(LTDC_VSYNC_GPIO_PORT, LTDC_VSYNC_PINSOURCE, LTDC_VSYNC_AF);

 GPIO_InitStruct.GPIO_Pin = LTDC_DE_GPIO_PIN;
 GPIO_Init(LTDC_DE_GPIO_PORT, &GPIO_InitStruct);
 GPIO_PinAFConfig(LTDC_DE_GPIO_PORT, LTDC_DE_PINSOURCE, LTDC_DE_AF);

 //BL DISP
 GPIO_InitStruct.GPIO_Pin = LTDC_DISP_GPIO_PIN;
 GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
 GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
 GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
 GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;

 GPIO_Init(LTDC_DISP_GPIO_PORT, &GPIO_InitStruct);


 GPIO_InitStruct.GPIO_Pin = LTDC_BL_GPIO_PIN;
 GPIO_Init(LTDC_BL_GPIO_PORT, &GPIO_InitStruct);

 //����ʹ��lcd
 GPIO_SetBits(LTDC_DISP_GPIO_PORT,LTDC_DISP_GPIO_PIN);
 GPIO_SetBits(LTDC_BL_GPIO_PORT,LTDC_BL_GPIO_PIN);
}

/**
 * @brief  Displays a pixel.
 * @param  x: pixel x.
 * @param  y: pixel y.
 * @retval None
 */
void PutPixel(int16_t x, int16_t y)
{
 if(x < 0 || x > LCD_PIXEL_WIDTH || y < 0 || y > LCD_PIXEL_HEIGHT)
 {
   return;
 }
#if 0
 LCD_DrawLine(x, y, 1, LCD_DIR_HORIZONTAL);
#else
 {
	  uint32_t  Xaddress = 0;
    Xaddress = CurrentFrameBuffer + 2*(LCD_PIXEL_WIDTH*y + x);
    *(__IO uint16_t*) Xaddress= CurrentTextColor;
  }
#endif
}

/**
 * @}
 */

/**
 * @}
 */

/**
 * @}
 */

/**
 * @}
 */

/**
 * @}
 */

#endif
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
