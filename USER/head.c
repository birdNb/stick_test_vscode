#include "head.h"

char text[50];
char text_send[50];

int8_t angleX,angleY,angleZ;


uint16_t lst_X,lst_Y,lst_Z,lst_T;
float angle_error=10,normal_H;

int16_t move_X,move_Y,move_Z;
bool T=0;
u8 G_flg=0;
//float sigma_X,sigma_Y,sigma_Z;

uint8_t vol;
bool now_k1,now_k2,now_k3,now_k4;
bool last_k1,last_k2,last_k3,last_k4;
bool rx_flag;
uint16_t adc[3];

u8 cnt;
u8 rx_byte,rx_byt,rx_piont;
u8 rx_buff[18];
u8 modle=1;
u8 pose;
uint16_t adc_max=4100,adc_min=2300;
//陀螺仪相关：
uint8_t rx_data;		
uint8_t rx_buffer[6], rx_cnt; /*串口数据接收缓冲区*/
bool imu_flag=0;
JY901_GetData IMU;
uint8_t z_AngleUnlock[5]={0xFF,0xAA,0x69,0x88,0xB5};
uint8_t z_AngleCalibration[5]={0xFF,0xAA,0x01,0x08,0x00};
uint8_t z_AngleSave[5]={0xFF,0xAA,0x00,0x00,0x00};








