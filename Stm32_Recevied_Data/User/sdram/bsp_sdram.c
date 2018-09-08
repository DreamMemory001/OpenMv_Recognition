/**
  ******************************************************************************
  * @file    bsp_sdram.c
  * @author  fire
  * @version V1.0
  * @date    2015-xx-xx
  * @brief   sdramӦ�ú����ӿ�
  ******************************************************************************
  * @attention
  *
  * ʵ��ƽ̨:����  STM32 F429 ������  
  * ��̳    :http://www.firebbs.cn
  * �Ա�    :https://fire-stm32.taobao.com
  *
  ******************************************************************************
  */
  
#include "./sdram/bsp_sdram.h"   

/**
  * @brief  �ӳ�һ��ʱ��
  * @param  �ӳٵ�ʱ�䳤��
  * @retval None
  */
static void SDRAM_delay(__IO uint32_t nCount)
{
  __IO uint32_t index = 0; 
  for(index = (100000 * nCount); index != 0; index--)
  {
  }
}

/**
  * @brief  ��ʼ������SDRAM��IO
  * @param  ��
  * @retval ��
  */
static void SDRAM_GPIO_Config(void)
{		
  GPIO_InitTypeDef GPIO_InitStructure;
  
  /* ʹ��SDRAM��ص�GPIOʱ�� */

                         /*��ַ�ź���*/
  RCC_AHB1PeriphClockCmd(FMC_A0_GPIO_CLK | FMC_A1_GPIO_CLK | FMC_A2_GPIO_CLK | 
                         FMC_A3_GPIO_CLK | FMC_A4_GPIO_CLK | FMC_A5_GPIO_CLK |
                         FMC_A6_GPIO_CLK | FMC_A7_GPIO_CLK | FMC_A8_GPIO_CLK |
                         FMC_A9_GPIO_CLK | FMC_A10_GPIO_CLK| FMC_A11_GPIO_CLK|  
                         /*�����ź���*/
                         FMC_D0_GPIO_CLK | FMC_D1_GPIO_CLK | FMC_D2_GPIO_CLK | 
                         FMC_D3_GPIO_CLK | FMC_D4_GPIO_CLK | FMC_D5_GPIO_CLK |
                         FMC_D6_GPIO_CLK | FMC_D7_GPIO_CLK | FMC_D8_GPIO_CLK |
                         FMC_D9_GPIO_CLK | FMC_D10_GPIO_CLK| FMC_D11_GPIO_CLK|
                         FMC_D12_GPIO_CLK| FMC_D13_GPIO_CLK| FMC_D14_GPIO_CLK|
                         FMC_D15_GPIO_CLK|  
                         /*�����ź���*/
                         FMC_CS_GPIO_CLK | FMC_BA0_GPIO_CLK | FMC_BA1_GPIO_CLK | 
                         FMC_WE_GPIO_CLK | FMC_RAS_GPIO_CLK | FMC_CAS_GPIO_CLK |
                         FMC_CLK_GPIO_CLK | FMC_CKE_GPIO_CLK | FMC_UDQM_GPIO_CLK |
                         FMC_LDQM_GPIO_CLK, ENABLE);


  
  /*-- GPIO ���� -----------------------------------------------------*/

  /* ͨ�� GPIO ���� */
  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF;       //����Ϊ���ù���
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;   
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;      //�������
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;   
  
  
  /*��ַ�ź��� �����������*/
  GPIO_InitStructure.GPIO_Pin = FMC_A0_GPIO_PIN; 
  GPIO_Init(FMC_A0_GPIO_PORT, &GPIO_InitStructure);
  GPIO_PinAFConfig(FMC_A0_GPIO_PORT, FMC_A0_PINSOURCE , FMC_A0_AF);
  
  GPIO_InitStructure.GPIO_Pin = FMC_A1_GPIO_PIN; 
  GPIO_Init(FMC_A1_GPIO_PORT, &GPIO_InitStructure);
  GPIO_PinAFConfig(FMC_A1_GPIO_PORT, FMC_A1_PINSOURCE , FMC_A1_AF);
  
  GPIO_InitStructure.GPIO_Pin = FMC_A2_GPIO_PIN; 
  GPIO_Init(FMC_A2_GPIO_PORT, &GPIO_InitStructure);
  GPIO_PinAFConfig(FMC_A2_GPIO_PORT, FMC_A2_PINSOURCE , FMC_A2_AF);
  
  GPIO_InitStructure.GPIO_Pin = FMC_A3_GPIO_PIN; 
  GPIO_Init(FMC_A3_GPIO_PORT, &GPIO_InitStructure);
  GPIO_PinAFConfig(FMC_A3_GPIO_PORT, FMC_A3_PINSOURCE , FMC_A3_AF);
  
  GPIO_InitStructure.GPIO_Pin = FMC_A4_GPIO_PIN; 
  GPIO_Init(FMC_A4_GPIO_PORT, &GPIO_InitStructure);
  GPIO_PinAFConfig(FMC_A4_GPIO_PORT, FMC_A4_PINSOURCE , FMC_A4_AF);
  
  GPIO_InitStructure.GPIO_Pin = FMC_A5_GPIO_PIN; 
  GPIO_Init(FMC_A5_GPIO_PORT, &GPIO_InitStructure);
  GPIO_PinAFConfig(FMC_A5_GPIO_PORT, FMC_A5_PINSOURCE , FMC_A5_AF);
  
  GPIO_InitStructure.GPIO_Pin = FMC_A6_GPIO_PIN; 
  GPIO_Init(FMC_A6_GPIO_PORT, &GPIO_InitStructure);
  GPIO_PinAFConfig(FMC_A6_GPIO_PORT, FMC_A6_PINSOURCE , FMC_A6_AF);
  
  GPIO_InitStructure.GPIO_Pin = FMC_A7_GPIO_PIN; 
  GPIO_Init(FMC_A7_GPIO_PORT, &GPIO_InitStructure);
  GPIO_PinAFConfig(FMC_A7_GPIO_PORT, FMC_A7_PINSOURCE , FMC_A7_AF);
  
  GPIO_InitStructure.GPIO_Pin = FMC_A8_GPIO_PIN; 
  GPIO_Init(FMC_A8_GPIO_PORT, &GPIO_InitStructure);
  GPIO_PinAFConfig(FMC_A8_GPIO_PORT, FMC_A8_PINSOURCE , FMC_A8_AF);
  
  GPIO_InitStructure.GPIO_Pin = FMC_A9_GPIO_PIN; 
  GPIO_Init(FMC_A9_GPIO_PORT, &GPIO_InitStructure);
  GPIO_PinAFConfig(FMC_A9_GPIO_PORT, FMC_A9_PINSOURCE , FMC_A9_AF);
  
  GPIO_InitStructure.GPIO_Pin = FMC_A10_GPIO_PIN; 
  GPIO_Init(FMC_A10_GPIO_PORT, &GPIO_InitStructure);
  GPIO_PinAFConfig(FMC_A10_GPIO_PORT, FMC_A10_PINSOURCE , FMC_A10_AF);
  
  GPIO_InitStructure.GPIO_Pin = FMC_A11_GPIO_PIN; 
  GPIO_Init(FMC_A11_GPIO_PORT, &GPIO_InitStructure);
  GPIO_PinAFConfig(FMC_A11_GPIO_PORT, FMC_A11_PINSOURCE , FMC_A11_AF);
  
  
  /*�����ź��� �����������*/
  GPIO_InitStructure.GPIO_Pin = FMC_D0_GPIO_PIN; 
  GPIO_Init(FMC_D0_GPIO_PORT, &GPIO_InitStructure);
  GPIO_PinAFConfig(FMC_D0_GPIO_PORT, FMC_D0_PINSOURCE , FMC_D0_AF);
  
  GPIO_InitStructure.GPIO_Pin = FMC_D1_GPIO_PIN; 
  GPIO_Init(FMC_D1_GPIO_PORT, &GPIO_InitStructure);
  GPIO_PinAFConfig(FMC_D1_GPIO_PORT, FMC_D1_PINSOURCE , FMC_D1_AF);
    
  GPIO_InitStructure.GPIO_Pin = FMC_D2_GPIO_PIN; 
  GPIO_Init(FMC_D2_GPIO_PORT, &GPIO_InitStructure);
  GPIO_PinAFConfig(FMC_D2_GPIO_PORT, FMC_D2_PINSOURCE , FMC_D2_AF);
  
  GPIO_InitStructure.GPIO_Pin = FMC_D3_GPIO_PIN; 
  GPIO_Init(FMC_D3_GPIO_PORT, &GPIO_InitStructure);
  GPIO_PinAFConfig(FMC_D3_GPIO_PORT, FMC_D3_PINSOURCE , FMC_D3_AF);
  
  GPIO_InitStructure.GPIO_Pin = FMC_D4_GPIO_PIN; 
  GPIO_Init(FMC_D4_GPIO_PORT, &GPIO_InitStructure);
  GPIO_PinAFConfig(FMC_D4_GPIO_PORT, FMC_D4_PINSOURCE , FMC_D4_AF);
  
  GPIO_InitStructure.GPIO_Pin = FMC_D5_GPIO_PIN; 
  GPIO_Init(FMC_D5_GPIO_PORT, &GPIO_InitStructure);
  GPIO_PinAFConfig(FMC_D5_GPIO_PORT, FMC_D5_PINSOURCE , FMC_D5_AF);
  
  GPIO_InitStructure.GPIO_Pin = FMC_D6_GPIO_PIN; 
  GPIO_Init(FMC_D6_GPIO_PORT, &GPIO_InitStructure);
  GPIO_PinAFConfig(FMC_D6_GPIO_PORT, FMC_D6_PINSOURCE , FMC_D6_AF);
  
  GPIO_InitStructure.GPIO_Pin = FMC_D7_GPIO_PIN; 
  GPIO_Init(FMC_D7_GPIO_PORT, &GPIO_InitStructure);
  GPIO_PinAFConfig(FMC_D7_GPIO_PORT, FMC_D7_PINSOURCE , FMC_D7_AF);
  
  GPIO_InitStructure.GPIO_Pin = FMC_D8_GPIO_PIN; 
  GPIO_Init(FMC_D8_GPIO_PORT, &GPIO_InitStructure);
  GPIO_PinAFConfig(FMC_D8_GPIO_PORT, FMC_D8_PINSOURCE , FMC_D8_AF);
  
  GPIO_InitStructure.GPIO_Pin = FMC_D9_GPIO_PIN; 
  GPIO_Init(FMC_D9_GPIO_PORT, &GPIO_InitStructure);
  GPIO_PinAFConfig(FMC_D9_GPIO_PORT, FMC_D9_PINSOURCE , FMC_D9_AF);
  
  GPIO_InitStructure.GPIO_Pin = FMC_D10_GPIO_PIN; 
  GPIO_Init(FMC_D10_GPIO_PORT, &GPIO_InitStructure);
  GPIO_PinAFConfig(FMC_D10_GPIO_PORT, FMC_D10_PINSOURCE , FMC_D10_AF);
  
  GPIO_InitStructure.GPIO_Pin = FMC_D11_GPIO_PIN; 
  GPIO_Init(FMC_D11_GPIO_PORT, &GPIO_InitStructure);
  GPIO_PinAFConfig(FMC_D11_GPIO_PORT, FMC_D11_PINSOURCE , FMC_D11_AF);
  
  GPIO_InitStructure.GPIO_Pin = FMC_D12_GPIO_PIN; 
  GPIO_Init(FMC_D12_GPIO_PORT, &GPIO_InitStructure);
  GPIO_PinAFConfig(FMC_D12_GPIO_PORT, FMC_D12_PINSOURCE , FMC_D12_AF);
  
  GPIO_InitStructure.GPIO_Pin = FMC_D13_GPIO_PIN; 
  GPIO_Init(FMC_D13_GPIO_PORT, &GPIO_InitStructure);
  GPIO_PinAFConfig(FMC_D13_GPIO_PORT, FMC_D13_PINSOURCE , FMC_D13_AF);
  
  GPIO_InitStructure.GPIO_Pin = FMC_D14_GPIO_PIN; 
  GPIO_Init(FMC_D14_GPIO_PORT, &GPIO_InitStructure);
  GPIO_PinAFConfig(FMC_D14_GPIO_PORT, FMC_D14_PINSOURCE , FMC_D14_AF);
  
  GPIO_InitStructure.GPIO_Pin = FMC_D15_GPIO_PIN; 
  GPIO_Init(FMC_D15_GPIO_PORT, &GPIO_InitStructure);
  GPIO_PinAFConfig(FMC_D15_GPIO_PORT, FMC_D15_PINSOURCE , FMC_D15_AF);
  
  /*�����ź���*/
  GPIO_InitStructure.GPIO_Pin = FMC_CS_GPIO_PIN; 
  GPIO_Init(FMC_CS_GPIO_PORT, &GPIO_InitStructure);
  GPIO_PinAFConfig(FMC_CS_GPIO_PORT, FMC_CS_PINSOURCE , FMC_CS_AF);
  
  GPIO_InitStructure.GPIO_Pin = FMC_BA0_GPIO_PIN; 
  GPIO_Init(FMC_BA0_GPIO_PORT, &GPIO_InitStructure);
  GPIO_PinAFConfig(FMC_BA0_GPIO_PORT, FMC_BA0_PINSOURCE , FMC_BA0_AF);
  
  GPIO_InitStructure.GPIO_Pin = FMC_BA1_GPIO_PIN;
  GPIO_Init(FMC_BA1_GPIO_PORT, &GPIO_InitStructure);
  GPIO_PinAFConfig(FMC_BA1_GPIO_PORT, FMC_BA1_PINSOURCE , FMC_BA1_AF);
    
  GPIO_InitStructure.GPIO_Pin = FMC_WE_GPIO_PIN; 
  GPIO_Init(FMC_WE_GPIO_PORT, &GPIO_InitStructure);
  GPIO_PinAFConfig(FMC_WE_GPIO_PORT, FMC_WE_PINSOURCE , FMC_WE_AF);
  
  GPIO_InitStructure.GPIO_Pin = FMC_RAS_GPIO_PIN; 
  GPIO_Init(FMC_RAS_GPIO_PORT, &GPIO_InitStructure);
  GPIO_PinAFConfig(FMC_RAS_GPIO_PORT, FMC_RAS_PINSOURCE , FMC_RAS_AF);
    
  GPIO_InitStructure.GPIO_Pin = FMC_CAS_GPIO_PIN; 
  GPIO_Init(FMC_CAS_GPIO_PORT, &GPIO_InitStructure);
  GPIO_PinAFConfig(FMC_CAS_GPIO_PORT, FMC_CAS_PINSOURCE , FMC_CAS_AF);
  
  GPIO_InitStructure.GPIO_Pin = FMC_CLK_GPIO_PIN; 
  GPIO_Init(FMC_CLK_GPIO_PORT, &GPIO_InitStructure);
  GPIO_PinAFConfig(FMC_CLK_GPIO_PORT, FMC_CLK_PINSOURCE , FMC_CLK_AF);
  
  GPIO_InitStructure.GPIO_Pin = FMC_CKE_GPIO_PIN; 
  GPIO_Init(FMC_CKE_GPIO_PORT, &GPIO_InitStructure);
  GPIO_PinAFConfig(FMC_CKE_GPIO_PORT, FMC_CKE_PINSOURCE , FMC_CKE_AF);
  
  GPIO_InitStructure.GPIO_Pin = FMC_UDQM_GPIO_PIN; 
  GPIO_Init(FMC_UDQM_GPIO_PORT, &GPIO_InitStructure);
  GPIO_PinAFConfig(FMC_UDQM_GPIO_PORT, FMC_UDQM_PINSOURCE , FMC_UDQM_AF);
  
  GPIO_InitStructure.GPIO_Pin = FMC_LDQM_GPIO_PIN; 
  GPIO_Init(FMC_LDQM_GPIO_PORT, &GPIO_InitStructure);
  GPIO_PinAFConfig(FMC_LDQM_GPIO_PORT, FMC_LDQM_PINSOURCE , FMC_LDQM_AF);

		
}

/**
  * @brief  ��SDRAMоƬ���г�ʼ������
  * @param  None. 
  * @retval None.
  */
static void SDRAM_InitSequence(void)
{
  FMC_SDRAMCommandTypeDef FMC_SDRAMCommandStructure;
  uint32_t tmpr = 0;
  
/* Step 3 --------------------------------------------------------------------*/
  /* ������������ṩ��SDRAM��ʱ�� */
  FMC_SDRAMCommandStructure.FMC_CommandMode = FMC_Command_Mode_CLK_Enabled;
  FMC_SDRAMCommandStructure.FMC_CommandTarget = FMC_COMMAND_TARGET_BANK;
  FMC_SDRAMCommandStructure.FMC_AutoRefreshNumber = 1;
  FMC_SDRAMCommandStructure.FMC_ModeRegisterDefinition = 0;
  /* ���SDRAM��־���ȴ���SDRAM���� */ 
  while(FMC_GetFlagStatus(FMC_BANK_SDRAM, FMC_FLAG_Busy) != RESET)
  {
  }
  /* ������������*/
  FMC_SDRAMCmdConfig(&FMC_SDRAMCommandStructure);  
  
/* Step 4 --------------------------------------------------------------------*/
  /* Լ100ms����ʱ */
  SDRAM_delay(10);
    
/* Step 5 --------------------------------------------------------------------*/
  /* ������������е�bankԤ��� */ 
  FMC_SDRAMCommandStructure.FMC_CommandMode = FMC_Command_Mode_PALL;
  FMC_SDRAMCommandStructure.FMC_CommandTarget = FMC_COMMAND_TARGET_BANK;
  FMC_SDRAMCommandStructure.FMC_AutoRefreshNumber = 1;
  FMC_SDRAMCommandStructure.FMC_ModeRegisterDefinition = 0;
  /* ���SDRAM��־���ȴ���SDRAM���� */ 
  while(FMC_GetFlagStatus(FMC_BANK_SDRAM, FMC_FLAG_Busy) != RESET)
  {
  }
   /* ������������*/
  FMC_SDRAMCmdConfig(&FMC_SDRAMCommandStructure);
  
/* Step 6 --------------------------------------------------------------------*/
  /* ��������Զ�ˢ�� */   
  FMC_SDRAMCommandStructure.FMC_CommandMode = FMC_Command_Mode_AutoRefresh;
  FMC_SDRAMCommandStructure.FMC_CommandTarget = FMC_COMMAND_TARGET_BANK;
  FMC_SDRAMCommandStructure.FMC_AutoRefreshNumber = 4;
  FMC_SDRAMCommandStructure.FMC_ModeRegisterDefinition = 0;
  /* ���SDRAM��־���ȴ���SDRAM���� */ 
  while(FMC_GetFlagStatus(FMC_BANK_SDRAM, FMC_FLAG_Busy) != RESET)
  {
  }
   /* ���͵�1���Զ�ˢ������*/
  FMC_SDRAMCmdConfig(&FMC_SDRAMCommandStructure);
  
  /* ���SDRAM��־���ȴ���SDRAM���� */ 
  while(FMC_GetFlagStatus(FMC_BANK_SDRAM, FMC_FLAG_Busy) != RESET)
  {
  }
   /* ���͵�2���Զ�ˢ������*/
  FMC_SDRAMCmdConfig(&FMC_SDRAMCommandStructure);
  
/* Step 7 --------------------------------------------------------------------*/
  /* ����sdram�Ĵ������� */
  tmpr = (uint32_t)SDRAM_MODEREG_BURST_LENGTH_2          |
                   SDRAM_MODEREG_BURST_TYPE_SEQUENTIAL   |
                   SDRAM_MODEREG_CAS_LATENCY_3           |
                   SDRAM_MODEREG_OPERATING_MODE_STANDARD |
                   SDRAM_MODEREG_WRITEBURST_MODE_SINGLE;
  
  /* �����������SDRAM�Ĵ��� */
  FMC_SDRAMCommandStructure.FMC_CommandMode = FMC_Command_Mode_LoadMode;
  FMC_SDRAMCommandStructure.FMC_CommandTarget = FMC_COMMAND_TARGET_BANK;
  FMC_SDRAMCommandStructure.FMC_AutoRefreshNumber = 1;
  FMC_SDRAMCommandStructure.FMC_ModeRegisterDefinition = tmpr;
  /* ���SDRAM��־���ȴ���SDRAM���� */ 
  while(FMC_GetFlagStatus(FMC_BANK_SDRAM, FMC_FLAG_Busy) != RESET)
  {
  }
   /* ������������*/
  FMC_SDRAMCmdConfig(&FMC_SDRAMCommandStructure);
  
/* Step 8 --------------------------------------------------------------------*/

  /* ����ˢ�¼����� */
  /* (15.62 us x Freq) - 20 */
  FMC_SetRefreshCount(1386);
  /* ������������*/
  while(FMC_GetFlagStatus(FMC_BANK_SDRAM, FMC_FLAG_Busy) != RESET)
  {
  }
}


/**
  * @brief  ��ʼ������ʹ��SDRAM��FMC��GPIO�ӿڣ�
  *         ��������SDRAM��д����ǰ��Ҫ������
  * @param  None
  * @retval None
  */
void SDRAM_Init(void)
{
  FMC_SDRAMInitTypeDef  FMC_SDRAMInitStructure;
  FMC_SDRAMTimingInitTypeDef  FMC_SDRAMTimingInitStructure; 
  
  /* ����FMC�ӿ���ص� GPIO*/
  SDRAM_GPIO_Config();
  
  /* ʹ�� FMC ʱ�� */
  RCC_AHB3PeriphClockCmd(RCC_AHB3Periph_FMC, ENABLE);
 
  /* ���� FMC ��ز��� ---------------------------------------------------------*/
  /* SDCLK�� 90 Mhz (HCLK/2 :180Mhz/2) */
  /* TMRD: 2 Clock cycles */
  FMC_SDRAMTimingInitStructure.FMC_LoadToActiveDelay    = 2;      
  /* TXSR: min=70ns (7x11.11ns) */
  FMC_SDRAMTimingInitStructure.FMC_ExitSelfRefreshDelay = 7;
  /* TRAS: min=42ns (4x11.11ns) max=120k (ns) */
  FMC_SDRAMTimingInitStructure.FMC_SelfRefreshTime      = 4;
  /* TRC:  min=70 (7x11.11ns) */        
  FMC_SDRAMTimingInitStructure.FMC_RowCycleDelay        = 7;         
  /* TWR:  min=1+ 7ns (1+1x11.11ns) */
  FMC_SDRAMTimingInitStructure.FMC_WriteRecoveryTime    = 2;      
  /* TRP:  20ns => 2x11.11ns */
  FMC_SDRAMTimingInitStructure.FMC_RPDelay              = 2;                
  /* TRCD: 20ns => 2x11.11ns */
  FMC_SDRAMTimingInitStructure.FMC_RCDDelay             = 2;

/* FMC SDRAM �������� */
  FMC_SDRAMInitStructure.FMC_Bank = FMC_BANK_SDRAM;
  /* �е�ַ�߿��: [7:0] */
  FMC_SDRAMInitStructure.FMC_ColumnBitsNumber = FMC_ColumnBits_Number_8b;
  /* �е�ַ�߿��: [11:0] */
  FMC_SDRAMInitStructure.FMC_RowBitsNumber = FMC_RowBits_Number_12b;
  /* �����߿�� */
  FMC_SDRAMInitStructure.FMC_SDMemoryDataWidth = SDRAM_MEMORY_WIDTH;
  /* SDRAM�ڲ�bank����*/
  FMC_SDRAMInitStructure.FMC_InternalBankNumber = FMC_InternalBank_Number_4;
  /* CASǱ���� */
  FMC_SDRAMInitStructure.FMC_CASLatency = SDRAM_CAS_LATENCY; 
  /* ��ֹд����*/
  FMC_SDRAMInitStructure.FMC_WriteProtection = FMC_Write_Protection_Disable;
  /* SDCLKʱ�ӷ�Ƶ���ӣ�SDCLK = HCLK/SDCLOCK_PERIOD*/
  FMC_SDRAMInitStructure.FMC_SDClockPeriod = SDCLOCK_PERIOD; 
  /* ͻ����ģʽ����*/  
  FMC_SDRAMInitStructure.FMC_ReadBurst = SDRAM_READBURST;
  /* ���ӳ����� */
  FMC_SDRAMInitStructure.FMC_ReadPipeDelay = FMC_ReadPipe_Delay_1;
  /* SDRAMʱ����� */
  FMC_SDRAMInitStructure.FMC_SDRAMTimingStruct = &FMC_SDRAMTimingInitStructure;
  
  /* FMC SDRAM bank initialization */
  FMC_SDRAMInit(&FMC_SDRAMInitStructure); 
  
  /* FMC SDRAM device initialization sequence */
  SDRAM_InitSequence(); 
  
}



/**
  * @brief  �ԡ��֡�Ϊ��λ��sdramд������ 
  * @param  pBuffer: ָ�����ݵ�ָ�� 
  * @param  uwWriteAddress: Ҫд���SDRAM�ڲ���ַ
  * @param  uwBufferSize: Ҫд�����ݴ�С
  * @retval None.
  */
void SDRAM_WriteBuffer(uint32_t* pBuffer, uint32_t uwWriteAddress, uint32_t uwBufferSize)
{
  __IO uint32_t write_pointer = (uint32_t)uwWriteAddress;

  /* ��ֹд���� */
  FMC_SDRAMWriteProtectionConfig(FMC_Bank2_SDRAM, DISABLE);
  
  /* ���SDRAM��־���ȴ���SDRAM���� */ 
  while(FMC_GetFlagStatus(FMC_Bank2_SDRAM, FMC_FLAG_Busy) != RESET)
  {
  }

  /* ѭ��д������ */
  for (; uwBufferSize != 0; uwBufferSize--) 
  {
    /* �������ݵ�SDRAM */
    *(uint32_t *) (SDRAM_BANK_ADDR + write_pointer) = *pBuffer++;

    /* ��ַ����*/
    write_pointer += 4;
  }
    
}

/**
  * @brief  ��SDRAM�ж�ȡ���� 
  * @param  pBuffer: ָ��洢���ݵ�buffer
  * @param  ReadAddress: Ҫ��ȡ���ݵĵ�ʮ
  * @param  uwBufferSize: Ҫ��ȡ�����ݴ�С
  * @retval None.
  */
void SDRAM_ReadBuffer(uint32_t* pBuffer, uint32_t uwReadAddress, uint32_t uwBufferSize)
{
  __IO uint32_t write_pointer = (uint32_t)uwReadAddress;
  
   
  /* ���SDRAM��־���ȴ���SDRAM���� */  
  while(FMC_GetFlagStatus(FMC_Bank2_SDRAM, FMC_FLAG_Busy) != RESET)
  {
  }
  
  /*��ȡ���� */
  for(; uwBufferSize != 0x00; uwBufferSize--)
  {
   *pBuffer++ = *(__IO uint32_t *)(SDRAM_BANK_ADDR + write_pointer );
    
   /* ��ַ����*/
    write_pointer += 4;
  } 
}

/**
  * @brief  ����SDRAM�Ƿ����� 
  * @param  None
  * @retval ��������1���쳣����0
  */
uint8_t SDRAM_Test(void)
{
  /*д�����ݼ�����*/
  uint32_t counter=0;
  
  /* 8λ������ */
  uint8_t ubWritedata_8b = 0, ubReaddata_8b = 0;  
  
  /* 16λ������ */
  uint16_t uhWritedata_16b = 0, uhReaddata_16b = 0; 
  
  SDRAM_INFO("���ڼ��SDRAM����8λ��16λ�ķ�ʽ��дsdram...");


  /*��8λ��ʽ��д���ݣ���У��*/
  
  /* ��SDRAM����ȫ������Ϊ0 ��IS42S16400J_SIZE����16λΪ��λ�� */
  for (counter = 0x00; counter < IS42S16400J_SIZE; counter++)
  {
    *(__IO uint8_t*) (SDRAM_BANK_ADDR + counter) = (uint8_t)0x0;
  }
  
  /* ������SDRAMд������  8λ */
  for (counter = 0; counter < IS42S16400J_SIZE; counter++)
  {
    *(__IO uint8_t*) (SDRAM_BANK_ADDR + counter) = (uint8_t)(ubWritedata_8b + counter);
  }
  
  /* ��ȡ SDRAM ���ݲ����*/
  for(counter = 0; counter<IS42S16400J_SIZE;counter++ )
  {
    ubReaddata_8b = *(__IO uint8_t*)(SDRAM_BANK_ADDR + counter);  //�Ӹõ�ַ��������
    
    if(ubReaddata_8b != (uint8_t)(ubWritedata_8b + counter))      //������ݣ�������ȣ���������,���ؼ��ʧ�ܽ����
    {
      SDRAM_ERROR("8λ���ݶ�д����");
      return 0;
    }
  }
	
  
  /*��16λ��ʽ��д���ݣ������*/
  
  /* ��SDRAM����ȫ������Ϊ0 */
  for (counter = 0x00; counter < IS42S16400J_SIZE/2; counter++)
  {
    *(__IO uint16_t*) (SDRAM_BANK_ADDR + 2*counter) = (uint16_t)0x00;
  }
  
  /* ������SDRAMд������  16λ */
  for (counter = 0; counter < IS42S16400J_SIZE/2; counter++)
  {
    *(__IO uint16_t*) (SDRAM_BANK_ADDR + 2*counter) = (uint16_t)(uhWritedata_16b + counter);
  }
  
    /* ��ȡ SDRAM ���ݲ����*/
  for(counter = 0; counter<IS42S16400J_SIZE/2;counter++ )
  {
    uhReaddata_16b = *(__IO uint16_t*)(SDRAM_BANK_ADDR + 2*counter);  //�Ӹõ�ַ��������
    
    if(uhReaddata_16b != (uint16_t)(uhWritedata_16b + counter))      //������ݣ�������ȣ���������,���ؼ��ʧ�ܽ����
    {
      SDRAM_ERROR("16λ���ݶ�д����");

      return 0;
    }
  }

  
  SDRAM_INFO("SDRAM��д����������");
  /*���������return 1 */
  return 1;
  

}


/*********************************************END OF FILE**********************/

