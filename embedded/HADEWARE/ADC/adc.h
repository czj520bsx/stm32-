#ifndef __ADC_H
#define __ADC_H	
#include "sys.h"



#define CAL_PPM  10  // 校准环境中PPM值
#define RL	10  // RL阻值

//MQ4
static float MQ4_R0;
extern uint16_t CH4_max;
extern uint16_t H2_max;
extern uint16_t CO_max;

extern int alarm_CH4;
extern int alarm_CO;
extern int alarm_H2;

extern int CH4_flag;
extern int H2_flag;
extern int CO_flag;


typedef struct {
    float R0;      // R0 resistance value
    uint16_t ADC_Value;  // ADC voltage value
    unsigned int CH4;
} MQ4_Sensor;
extern MQ4_Sensor MQ4;
extern unsigned int CH4;
extern uint16_t MQ4_ADC_Value;

void MQ4_PPM_Calibration(void);
float MQ4_GetPPM(void);
extern void initialize_MQ4(MQ4_Sensor *sensor, uint16_t adc_value,unsigned int CH4);



//MQ8
static float MQ8_R0;
 

typedef struct {
    float R0;      // R0 resistance value
    uint16_t ADC_Value;  // ADC voltage value
    unsigned int H2;
} MQ8_Sensor;
extern MQ8_Sensor MQ8;
extern unsigned int H2;
extern uint16_t MQ8_ADC_Value;

void MQ8_PPM_Calibration(void);
float MQ8_GetPPM(void);
void initialize_MQ8(MQ8_Sensor *sensor, uint16_t adc_value,unsigned int H2);



//MQ7
static float MQ7_R0;

typedef struct {
    float R0;      // R0 resistance value
    uint16_t ADC_Value;  // ADC voltage value
    unsigned int CO;
} MQ7_Sensor;
extern MQ7_Sensor MQ7;
extern unsigned int CO;
extern uint16_t MQ7_ADC_Value;


void MQ7_PPM_Calibration(void);
float MQ7_GetPPM(void);
void initialize_MQ7(MQ7_Sensor *sensor, uint16_t adc_value,unsigned int CO);






void Adc_Init(void);
u16  Get_Adc(u8 ch); 
u16 Get_Adc_Average(u8 ch,u8 times); 








#endif 
