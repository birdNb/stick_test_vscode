#ifndef __fun_H
#define __fun_H

#include "head.h"




void fun_init(void);
void lcd_disp(void);
void im948_proc(void);
void angle_proc(void);
void  key_scan(void);
void  data_init(void);
void adc_get_vol(void);
int mapAngleToOutput(int angle);
void SendMessage(void);
void analog_beep(void);
void send_order(void);
void imu_prog(void);





#endif
