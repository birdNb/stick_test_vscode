#ifndef __head_H
#define __head_H

#include "main.h"
#include "spi.h"
#include "stdlib.h"
#include "lcd.h"
#include "string.h"
#include "stdio.h"
#include "stdbool.h"
#include "fun.h"
#include "usart.h"
#include "math.h"

#include "tim.h"
#include "adc.h"
#include "dma.h"
#include "usart.h"
#include "FreeRTOS.h"
#include "task.h"
#include "cmsis_os.h"
#include "stm32g4xx_hal.h"
#include "stm32g4xx_hal_tim.h"

extern osThreadId defaultTaskHandle;
extern osThreadId LcdShowHandle;
extern osThreadId SendMessageHandle;

extern uint8_t cnt;

#define u8     uint8_t
#define cutoff 800

extern char text[];
extern char text_send[];
extern int8_t angleX, angleY, angleZ;
extern uint16_t lst_X, lst_Y, lst_Z, lst_T;
extern u8 rx_byte, rx_byt, rx_piont;
extern struct_Ram_Uart Uart;
extern float angle_error, normal_H;
extern u8 vol;


extern uint16_t adc[];

extern bool now_k1, now_k2, now_k3, now_k4;
extern bool last_k1, last_k2, last_k3, last_k4;
// extern bool rx_flag;
extern u8 pose;
extern u8 modle;
// extern u8 rx_buff[];
extern int16_t move_X, move_Y, move_Z; 
extern bool T;
extern u8 G_flg;
extern uint16_t adc_max, adc_min;
// extern float sigma_X,sigma_Y,sigma_Z;
typedef struct
{
		float Angle[3];		/* 三轴角度 */
		float Accel[3];		/* 三轴加速度 */
		float Gyro[3];		/* 三轴角速度 */
}JY901_GetData;
extern u8 rx_buffer[], rx_cnt; /*串口数据接收缓冲区*/
extern bool imu_flag;

extern uint8_t rx_data;
extern uint8_t z_AngleUnlock[5];
extern uint8_t z_AngleCalibration[5];
extern uint8_t z_AngleSave[5];
extern JY901_GetData IMU;

extern osSemaphoreId myBinarySem01Handle;	//操作系统定义的互斥量

#endif
