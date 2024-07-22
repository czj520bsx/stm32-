#include "adc.h"
#include "delay.h"		 
#include "math.h"
//初始化ADC			
int alarm_CH4=1;
int alarm_CO=1;
int alarm_H2=1;

int CH4_flag=1;
int CO_flag=1;
int H2_flag=1;
uint16_t CH4_max=17;
uint16_t H2_max=23;
uint16_t CO_max=25;

 unsigned int CH4;
 uint16_t MQ4_ADC_Value;
  MQ4_Sensor MQ4;
 unsigned int H2;
 uint16_t MQ8_ADC_Value;
 MQ8_Sensor MQ8;

 unsigned int CO;
 uint16_t MQ7_ADC_Value;
  MQ7_Sensor MQ7;



void initialize_MQ4(MQ4_Sensor *sensor, uint16_t adc_value,unsigned int CH4) {
    sensor->R0 = MQ4_R0;  
    sensor->ADC_Value = adc_value; 
    sensor->CH4 = CH4; 
}

void MQ4_PPM_Calibration(void)
{
    float Vrl = 3.3f * MQ4.ADC_Value / 4095.f; // 计算出电压值
    float RS = (3.3f - Vrl) / Vrl * RL; // 根据分压原则计算出RS的电阻

    // 增加一个增长因子或者增量来加快R0值的增长速率
    float growth_factor = 1.25f; // 增长因子，你可以根据需求调整
    MQ4.R0 = (RS / pow(CAL_PPM / 98.322, 1 / -1.458f)) * growth_factor; // 计算出新的R0值

    // OLED_ShowNum(48, 0, R0, 4, 16); // R0计算的值显示在OLED屏幕上（调试用）
}
 // 获取传感器的值
float MQ4_GetPPM(void)
{
	float Vrl = 3.3f * MQ4.ADC_Value / 4095.f;//计算出电压值
	float RS = (3.3f - Vrl) / Vrl * RL;//根据分压原则计算出RS的电阻
	
	float ppm = 98.322f * pow(RS/MQ4.R0, -1.458f);//计算出PPM的值
	return  ppm; 
}













void initialize_MQ8(MQ8_Sensor *sensor, uint16_t adc_value,unsigned int H2) {
    sensor->R0 = MQ8_R0;  
    sensor->ADC_Value = adc_value; 
    sensor->H2 = H2;
}
void MQ8_PPM_Calibration(void)
{
    float Vrl = 3.3f * MQ8.ADC_Value / 4095.f;
    float RS = (3.3f - Vrl) / Vrl * RL;
    float growth_factor = 1.5f; // 增长因子调整为1.5
    MQ8.R0 = (RS / pow(CAL_PPM / 98.322, 1 / -1.458f)) * growth_factor;
    // OLED_ShowNum(48,0,R0,4,16);
}
 // 获取传感器的值
float MQ8_GetPPM(void)
{
	float Vrl = 3.3f * MQ8.ADC_Value / 4095.f;//计算出电压值
	float RS = (3.3f - Vrl) / Vrl * RL;//根据分压原则计算出RS的电阻
	float ppm = 98.322f * pow(RS/MQ8.R0, -1.458f);//计算出PPM的值
	return  ppm; 
}







void initialize_MQ7(MQ7_Sensor *sensor, uint16_t adc_value,unsigned int CO)
 {

    sensor->R0 = MQ7_R0;  
    sensor->ADC_Value = adc_value; 
    sensor->CO = CO;
}
  
void MQ7_PPM_Calibration(void)
{
	float Vrl = 3.3f * MQ7.ADC_Value / 4095.f;//计算出电压值
	float RS = (3.3f - Vrl) / Vrl * RL;//根据分压原则计算出RS的电阻
	
	  MQ7.R0 = (RS / pow(CAL_PPM / 98.322, 1 / -1.458f));//根据MATLAB绘制曲线推导出R0的对应公式
//	OLED_ShowNum(48,0,R0,4,16);//R0计算的值显示在OLED屏幕上（调试用）
}
 // 获取传感器的值
float MQ7_GetPPM(void)
{
	float Vrl = 3.3f * MQ7.ADC_Value / 4095.f;//计算出电压值
	float RS = (3.3f - Vrl) / Vrl * RL;//根据分压原则计算出RS的电阻
	
	 float growth_factor = 1.50f; // 增长因子，你可以根据需求调整
	float ppm = 98.322f * pow(RS/MQ7.R0, -1.458f) * growth_factor;;//计算出PPM的值
	return  ppm; 
}




void Adc_Init(void)
{ 	
	ADC_InitTypeDef ADC_InitStructure; 
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA |RCC_APB2Periph_ADC1, ENABLE );	  //使能ADC1通道时钟
 

	RCC_ADCCLKConfig(RCC_PCLK2_Div6);   //设置ADC分频因子6 72M/6=12,ADC最大时间不能超过14M

	//PA0 作为模拟通道输入引脚                         
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_4|GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;		//模拟输入引脚
	GPIO_Init(GPIOA, &GPIO_InitStructure);	
	
	

	ADC_DeInit(ADC1);  //复位ADC1 

	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;	//ADC工作模式:ADC1和ADC2工作在独立模式
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;	//模数转换工作在单通道模式
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;	//模数转换工作在单次转换模式
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;	//转换由软件而不是外部触发启动
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;	//ADC数据右对齐
	ADC_InitStructure.ADC_NbrOfChannel = 2;	//顺序进行规则转换的ADC通道的数目
	ADC_Init(ADC1, &ADC_InitStructure);	//根据ADC_InitStruct中指定的参数初始化外设ADCx的寄存器   

	ADC_Cmd(ADC1, ENABLE);	//使能指定的ADC1
	
	ADC_ResetCalibration(ADC1);	//使能复位校准  
	 
	while(ADC_GetResetCalibrationStatus(ADC1));	//等待复位校准结束
	
	ADC_StartCalibration(ADC1);	 //开启AD校准
 
	while(ADC_GetCalibrationStatus(ADC1));	 //等待校准结束
 
//	ADC_SoftwareStartConvCmd(ADC1, ENABLE);		//使能指定的ADC1的软件转换启动功能

}				  
//获得ADC值
//ch:通道值 0~3
u16 Get_Adc(u8 ch)   
{
  	//设置指定ADC的规则组通道，一个序列，采样时间
	ADC_RegularChannelConfig(ADC1, ch, 1, ADC_SampleTime_239Cycles5 );	//ADC1,ADC通道,采样时间为239.5周期	  			    
  
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);		//使能指定的ADC1的软件转换启动功能	
	 
	while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC ));//等待转换结束

	return ADC_GetConversionValue(ADC1);	//返回最近一次ADC1规则组的转换结果
}

u16 Get_Adc_Average(u8 ch,u8 times)
{
	u32 temp_val=0;
	u8 t;
	for(t=0;t<times;t++)
	{
		temp_val+=Get_Adc(ch);
		delay_ms(5);
	}
	return temp_val/times;
} 





































