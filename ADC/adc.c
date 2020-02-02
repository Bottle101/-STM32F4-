#include "adc.h"
#include "delay.h"		 	

__IO uint16_t ADCDualConvertedValue[16];

//��ʼ��ADC															   
void  Adc_Init(void)
{    
  GPIO_InitTypeDef  GPIO_InitStructure;
	ADC_CommonInitTypeDef ADC_CommonInitStructure;
	ADC_InitTypeDef       ADC_InitStructure;	
  DMA_InitTypeDef DMA_InitStructure;    
	
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);//ʹ��GPIOA,GPIOB,GPIOC��GPIOEʱ��
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2, ENABLE);//dma clock enable
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE); //ʹ��ADC1ʱ��

    /* DMA2 Stream0 channe0 configuration **************************************/
  DMA_DeInit(DMA2_Stream0);
  DMA_InitStructure.DMA_Channel = DMA_Channel_0; 
  DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)&ADCDualConvertedValue;
  DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&(ADC1->DR);
  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;
  DMA_InitStructure.DMA_BufferSize = 16;
  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
  DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
  DMA_InitStructure.DMA_Priority = DMA_Priority_High;
  DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;//DMA_FIFOMode_Enable;         
  DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_HalfFull;
  DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
  DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
  DMA_Init(DMA2_Stream0, &DMA_InitStructure);
  DMA_Cmd(DMA2_Stream0, ENABLE);
		
//	��ʼ������		
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;//PA0-7 ͨ��0-7
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;//ģ������
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;//����������
  GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ�� GPIOA
 
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
  GPIO_Init(GPIOB, &GPIO_InitStructure);//��ʼ�� GPIOB

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5;
  GPIO_Init(GPIOC, &GPIO_InitStructure);//��ʼ�� GPIOC

	RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC1,ENABLE);	  //ADC1��λ
	RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC1,DISABLE);	//��λ����	 
 
   /* ADC Common Init **********************************************************/
//  ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div2;	
  ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;//����ģʽ
  ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;//���������׶�֮����ӳ�5��ʱ��
  ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled; //DMAʧ��
  ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div2;//Ԥ��Ƶ4��Ƶ��ADCCLK=PCLK2/4=84/4=21Mhz,ADCʱ����ò�Ҫ����36Mhz 
  ADC_CommonInit(&ADC_CommonInitStructure);//��ʼ��
	
	
   /* ADC1 Init ****************************************************************/		
  ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;//12λģʽ
  ADC_InitStructure.ADC_ScanConvMode = ENABLE;//ɨ��ģʽ	
  ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;//����ת��
  ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;//��ֹ������⣬ʹ���������
  ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;//�Ҷ���	
  ADC_InitStructure.ADC_NbrOfConversion = 16;//1��ת���ڹ��������� Ҳ����ֻת����������1 
  ADC_Init(ADC1, &ADC_InitStructure);//ADC��ʼ��
	
	
	  /* ADC1 regular channel configuration ******************************************/
  ADC_RegularChannelConfig(ADC1, ADC_Channel_0, 1, ADC_SampleTime_56Cycles);
  ADC_RegularChannelConfig(ADC1, ADC_Channel_1, 2, ADC_SampleTime_56Cycles);
  ADC_RegularChannelConfig(ADC1, ADC_Channel_2, 3, ADC_SampleTime_56Cycles);
  ADC_RegularChannelConfig(ADC1, ADC_Channel_3, 4, ADC_SampleTime_56Cycles); 
  ADC_RegularChannelConfig(ADC1, ADC_Channel_4, 5, ADC_SampleTime_56Cycles);
  ADC_RegularChannelConfig(ADC1, ADC_Channel_5, 6, ADC_SampleTime_56Cycles);
  ADC_RegularChannelConfig(ADC1, ADC_Channel_6, 7, ADC_SampleTime_56Cycles);
  ADC_RegularChannelConfig(ADC1, ADC_Channel_7, 8, ADC_SampleTime_56Cycles); 
  ADC_RegularChannelConfig(ADC1, ADC_Channel_8, 9, ADC_SampleTime_56Cycles);
  ADC_RegularChannelConfig(ADC1, ADC_Channel_9, 10, ADC_SampleTime_56Cycles);
  ADC_RegularChannelConfig(ADC1, ADC_Channel_10, 11, ADC_SampleTime_56Cycles);
  ADC_RegularChannelConfig(ADC1, ADC_Channel_11, 12, ADC_SampleTime_56Cycles); 
  ADC_RegularChannelConfig(ADC1, ADC_Channel_12, 13, ADC_SampleTime_56Cycles);
  ADC_RegularChannelConfig(ADC1, ADC_Channel_13, 14, ADC_SampleTime_56Cycles);
  ADC_RegularChannelConfig(ADC1, ADC_Channel_14, 15, ADC_SampleTime_56Cycles);
  ADC_RegularChannelConfig(ADC1, ADC_Channel_15, 16, ADC_SampleTime_56Cycles); 
	
	  /* Enable DMA request after last transfer (Single-ADC mode) */
  ADC_DMARequestAfterLastTransferCmd(ADC1, ENABLE);
	
	  /* Enable ADC1 DMA */
  ADC_DMACmd(ADC1, ENABLE);

  /* Enable ADC1 */
  ADC_Cmd(ADC1, ENABLE);//����ADת����	

  ADC_SoftwareStartConv(ADC1);

}				  





//���ADCֵ
//ch: @ref ADC_channels 
//ͨ��ֵ 0~16ȡֵ��ΧΪ��ADC_Channel_0~ADC_Channel_16
//����ֵ:ת�����
u16 Get_Adc(u8 Channel)   
{
//    u32 temp_val=0;
//        temp_val=ADCDualConvertedValue[Channel];
//    return temp_val;
	    u32 temp_val=0;
    u8 t;
    for(t=0;t<5;t++)
    {
        temp_val+=ADCDualConvertedValue[Channel];
        
    }
    return temp_val/5;
}
//��ȡͨ��ch��ת��ֵ��ȡtimes��,Ȼ��ƽ�� 
//ch:ͨ�����
//times:��ȡ����
//����ֵ:ͨ��ch��times��ת�����ƽ��ֵ
u16 Get_Adc_Average(u8 Channel,u8 times)
{
    u32 temp_val=0;
    u8 t;
    for(t=0;t<times;t++)
    {
        temp_val+=ADCDualConvertedValue[Channel];
        
    }
    return temp_val/times;
} 
	 
