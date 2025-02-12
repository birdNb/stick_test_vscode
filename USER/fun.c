#include "fun.h"

/* z轴角度校准 */
void z_Angle_Calibration(void)
{
		HAL_UART_Transmit (&huart1 ,z_AngleUnlock,sizeof(z_AngleUnlock),0xff);
		HAL_Delay(200);
		HAL_UART_Transmit (&huart1 ,z_AngleCalibration,sizeof(z_AngleCalibration),0xff);
		HAL_Delay(5000);
		HAL_UART_Transmit (&huart1 ,z_AngleSave,sizeof(z_AngleSave),0xff);
}

void fun_init()
{
	

	// HAL_UART_Receive_IT(&huart1, &rx_byt, 1);
	HAL_ADCEx_Calibration_Start(&hadc1,ADC_SINGLE_ENDED); 
	HAL_ADC_Start_DMA(&hadc1,(uint32_t*)&adc,3); 

	// 串口二，IMU串口
	HAL_UART_Receive_DMA(&huart1, (uint8_t *)rx_buffer, 6);
//	__HAL_UART_ENABLE_IT(&huart2,UART_IT_IDLE); //启用空闲中断
	// z_Angle_Calibration();	/* z轴角度校准 */

	Lcd_Init();
	Lcd_Clear(BLACK);

	HAL_TIM_PWM_Start(&htim3,TIM_CHANNEL_2);
	HAL_TIM_PWM_Start(&htim3,TIM_CHANNEL_4);
	HAL_TIM_PWM_Start(&htim3,TIM_CHANNEL_3);

//	HAL_TIM_Base_Start(&htim6);
	HAL_Delay(1000);
	data_init();
	// HAL_TIM_IRQHandler(&htim4);
	
	analog_beep();
}

void fun_while()
{
	// if(TIM6->CNT>1000)
	// {	
		lcd_disp();
	// 	TIM6->CNT=0;	
	// }
		
	// 

}

void data_init()
{
	Lcd_Clear(BLACK);
	move_Y=0;
	move_Z=0;
	pose=4;
	T=0;
}


void lcd_disp()
{
	
	if(!modle)
	{
		sprintf(text,"  JY901S:      ");
		Lcd_ShowString(128+60,0, (u8*)text,YELLOW);
		
		sprintf(text,"VOL:%d       ",vol);
		if(vol<20)Lcd_ShowString(128+60,120, (u8*)text,RED);
		else Lcd_ShowString(128+60,120, (u8*)text,GREEN);
		
		// sprintf(text,"               ");
		// Lcd_ShowString(60+128,0, (u8*)text,WHITE);

		// sprintf(text,"               ");
		// Lcd_ShowString(60+128,20, (u8*)text,WHITE);

		sprintf(text,"angleX:%d ",angleX);
		Lcd_ShowString(128+60,40, (u8*)text,WHITE);
		
		sprintf(text,"angleY:%d ",angleY);
		Lcd_ShowString(128+60,60, (u8*)text,WHITE);
		
		sprintf(text,"angleZ:%d  ",angleZ);
		Lcd_ShowString(128+60,80, (u8*)text,WHITE);
		
		sprintf(text,"G_flg:%d   ",G_flg);
		Lcd_ShowString(128+60,100, (u8*)text,WHITE);
		
		// Lcd_DrawLine(128+45,0,128+45,164,RED);
		
		
		
		// sprintf(text,"angle_ctl_mod:  ");
		// Lcd_ShowString(128+60,0, (u8*)text,BLUE);
		
		// sprintf(text,"move_Y:%d   ",move_Y);
		// Lcd_ShowString(128+60,80, (u8*)text,WHITE);
		
		// sprintf(text,"move_Z:%d   ",move_Z);
		// Lcd_ShowString(128+60,100, (u8*)text,WHITE);
		
		// sprintf(text,"pose:%d   ",pose);
		// Lcd_ShowString(128+60,80, (u8*)text,GREEN);
		
		// sprintf(text,"trot:%d  ",T);
		// Lcd_ShowString(128+60,100, (u8*)text,YELLOW);
		sprintf(text,"X0Y%dW%dP%dT%dG%d   ",move_Y,move_X,pose,T,G_flg);
		Lcd_ShowString(60+128,140, (u8*)text,GREEN);
		


	}
	if(modle)
	{
		sprintf(text,"VOL:%d   ",adc[0]);
		if(vol<20)Lcd_ShowString(128+60,20, (u8*)text,RED);
		else Lcd_ShowString(128+60,80, (u8*)text,GREEN);

	    sprintf(text,"adc_Y_raw: %d  ",adc[2]);
		Lcd_ShowString(60+128,0, (u8*)text,WHITE);

		sprintf(text,"adc_W_raw: %d ",adc[1]);
		Lcd_ShowString(60+128,20, (u8*)text,WHITE);

		// sprintf(text,"pose:%d  ",pose);
		// Lcd_ShowString(20,80, (u8*)text,WHITE);
		
		sprintf(text,"move_Y:%d   ",move_Y);
		Lcd_ShowString(128+60,40, (u8*)text,WHITE);
		
		sprintf(text,"move_Z:%d    ",move_Z);
		Lcd_ShowString(128+60,60, (u8*)text,WHITE);

		// sprintf(text,"pose:%d ",pose);
		// Lcd_ShowString(128+60,80, (u8*)text,WHITE);
		sprintf(text,"G_flg:%d   ",G_flg);
		Lcd_ShowString(128+60,100, (u8*)text,WHITE);

		sprintf(text,"X0Y%dW%dP%dT%dG%d   ",move_Y,move_X,pose,T,G_flg);
		Lcd_ShowString(60+128,120, (u8*)text,GREEN);

		// sprintf(text,"trot:%d  ",T);
		// Lcd_ShowString(128+60,100, (u8*)text,YELLOW);

		// Lcd_DrawLine(128+45,0,128+45,164,RED);

		//手柄X：3100-4090 Y: 0-4090
		//手柄映射关系
		
	}
	
}

void send_order()
{
    static  int16_t old_move_Y;      // 存储上一次的 move_Y 值
    static  int16_t old_move_Z;      // 存储上一次的 move_Z 值
    static  uint8_t old_pose;        // 存储上一次的 pose 值
    static  bool old_T;           // 存储上一次的 T 值
    static  bool old_G_flg;       // 存储上一次的 G_flg 值

    // 判断各变量是否发生变化
    if (move_Y != old_move_Y || move_Z != old_move_Z || pose != old_pose || T != old_T || G_flg != old_G_flg)
    {
		DMA1_Channel4->CCR |= DMA_CCR_EN; // 开启DMA传输
		
        SendMessage();
		
		DMA1_Channel4->CCR &= ~(DMA_CCR_EN); // 关闭DMA传输
    }
    // 如果有变量发生变化，则更新静态变量
    old_move_Y = move_Y;
    old_move_Z = move_Z;
    old_pose = pose;                                                           
    old_T = T;
    old_G_flg = G_flg;

}

void angle_proc()
{
	// sscanf(rx_buffer, "x%dy%dz%d", &angleX, &angleY, &angleZ);
	// angleX=IMU.Angle[0];
	// angleY=IMU.Angle[1];
	// angleZ=IMU.Angle[2];

	if(!modle)
	{
//		if(angleY>angle_error&&angleY<65||angleY<-angle_error&&angleY>-65)
//		TIM3->CCR3=(abs((int)angleY)-10)*1;
//		else TIM3->CCR4=0;
//	
//		if(angleX>angle_error&&angleX<65||angleX<-angle_error&&angleX>-65)
//		TIM3->CCR4=(abs((int)angleX)-10)*1;
//		else TIM3->CCR3=0;
//	
//		if(angleZ>angle_error&&angleZ<65||angleZ<-angle_error&&angleZ>-65)
//		TIM3->CCR2=(abs((int)angleZ)-10)*1;
//		else TIM3->CCR2=0;// no line
		
		
		
	}
	else
	{
		TIM3->CCR3=(abs((int)move_Y))/3;
		TIM3->CCR4=(abs((int)move_Z))/8;
	}
}

void logic()
{
	//限幅
	
	if(move_Y>250)move_Y=250;
	if(move_Z>250)move_Z=250;
	if(move_Y<-250)move_Y=-250;
	if(move_Z<-250)move_Z=-250;
	
	adc_get_vol();
	angle_proc();
	if(!modle)
	{
		move_Y = -mapAngleToOutput((int16_t)angleY);
		move_Z = -mapAngleToOutput((int16_t)angleX)/3;
	}
	if(modle)
	{
		move_Z= (adc[1] - 3100) * 450 / 890 - 200;
		move_Y= (adc[2] * 500) / 4090 - 250;

		if(abs(move_Z)<30)move_Z=0;
		if(abs(move_Y)<30 || abs(move_Z)>50)move_Y=0;
	}
	if(!now_k1&&last_k1)
	{
		analog_beep();
		if(pose++ >= 4)pose = 1;
		T=0;
	}

	if(!now_k2&&last_k2)
	{	
		analog_beep();
		modle = !modle;
//		Lcd_Clear(BLACK);

	}
	
	//craw contral
	if(!now_k3&&last_k3)
	{
		analog_beep();
		cnt++; 
		if(cnt%2)
		{
			T=0;//停用trot步态
		}
		else
		{
			T=1;//启用trot步态
			pose = 0;
		}
		
	}
	
	if(!now_k4&&last_k4)
	{
		analog_beep();
		// G_flg=!G_flg;
		if(G_flg++ >= 2)G_flg = 0;
	}

}

void SendMessage()
{
	sprintf(text_send,"X0Y%dW%dP%dT%dG%d",move_Y,move_Z,pose,T,G_flg);
	HAL_UART_Transmit(&huart1,(uint8_t *)text_send,strlen(text_send),10);
	// osDelay(5);
}

void adc_get_vol()
{
	vol =(adc[0] - adc_min) * (100 - 1) / (adc_max - adc_min) + 1;//mriror function
	if(vol<0)vol=0;	

}

// 函数用于将角度映射到-200到+200的值
int mapAngleToOutput(int angle) {
    // 检查是否在无效值范围内
    if (angle >= -15 && angle <= 15 &&angle>=-45&&angle<=45) {
        return 0; // 返回无效值
    }
    // 计算映射值
    return (40 * angle) / 9;
}



void key_scan()
{
	now_k1=HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_3);
	now_k2=HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_0);
	now_k3=HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_1);
	now_k4=HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_2);
	
	logic();
	
	last_k1=now_k1;
	last_k2=now_k2;
	last_k3=now_k3;
	last_k4=now_k4;
}

void analog_beep()
{
    //电机模拟蜂鸣器
	TIM3->CCR4=10;
	HAL_Delay(50);
	TIM3->CCR4=0;
}

void imu_prog()
{

	
		// if (imu_flag || rx_cnt >= 11) /* 一组数据接收完毕 */
		if (imu_flag)
		{
			switch (rx_buffer[1])
			{
			case 0x51:																				/* 三轴加速度获取 */
				IMU.Accel[0] = (short)(((short)rx_buffer[3] << 8) | rx_buffer[2]) / 32768.0 * 16.0; /* xÖá¼ÓËÙ¶È */
				IMU.Accel[1] = (short)(((short)rx_buffer[5] << 8) | rx_buffer[4]) / 32768.0 * 16.0; /* yÖá¼ÓËÙ¶È */
				IMU.Accel[2] = (short)(((short)rx_buffer[7] << 8) | rx_buffer[6]) / 32768.0 * 16.0; /* zÖá¼ÓËÙ¶È */
				break;

			case 0x52:																				 /* 三轴角速度获取 */
				IMU.Gyro[0] = (short)(((short)rx_buffer[3] << 8) | rx_buffer[2]) / 32768.0 * 2000.0; /* xÖá½ÇËÙ¶È */
				IMU.Gyro[1] = (short)(((short)rx_buffer[5] << 8) | rx_buffer[4]) / 32768.0 * 2000.0; /* yÖá½ÇËÙ¶È */
				IMU.Gyro[2] = (short)(((short)rx_buffer[7] << 8) | rx_buffer[6]) / 32768.0 * 2000.0; /* zÖá½ÇËÙ¶È */
				break;

			case 0x53:																				 /* 三轴角度获取 */
				IMU.Angle[0] = (short)(((short)rx_buffer[3] << 8) | rx_buffer[2]) / 32768.0 * 180.0; /* ¹ö×ª½Ç¶ÁÈ¡ */
				IMU.Angle[1] = (short)(((short)rx_buffer[5] << 8) | rx_buffer[4]) / 32768.0 * 180.0; /* ¸©Ñö½Ç¶ÁÈ¡ */
				IMU.Angle[2] = (short)(((short)rx_buffer[7] << 8) | rx_buffer[6]) / 32768.0 * 180.0; /* Æ«º½½Ç¶ÁÈ¡ */
				break;
			}
			memset(rx_buffer ,0, rx_cnt);
			// rx_cnt = 0; /* 清零 */
			imu_flag = 0;
		}
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	
			if(huart->Instance == USART1)
		{
			sscanf(rx_buffer, "x%dy%dz%d", &angleX, &angleY, &angleZ);
			memset(rx_buffer ,0, 6);
			HAL_UART_Receive_DMA(&huart1, rx_buffer, 6);
		}
	// 	if (huart->Instance == USART2 && !modle)
	// {
		// if(strlen(rx_buffer)>=10)
		// {
		// 	imu_flag=1;
		// }
		// HAL_UART_Receive_DMA(&huart2,rx_buffer,10);
		// __HAL_TIM_SET_COUNTER(&htim7,0); //清空计数器

		// // static uint8_t rx_buffer[11], rx_cnt; /*串口数据接收缓冲区*/

		// if(0 == rx_cnt)											//如果是首字符（每帧数据开头）则开启定时器
		// {
		// 	__HAL_TIM_CLEAR_FLAG(&htim7, TIM_FLAG_UPDATE);			//清除中断标志位
		// 	HAL_TIM_Base_Start_IT(&htim7);							//开启基本定时器
		// }

		// rx_buffer[rx_cnt++] = rx_data;
		// if (rx_buffer[0] != 0x55)rx_cnt = 0;
		// if(rx_cnt >= 11)//如果接收到的数据长度为11
		// {
		// 	imu_flag = 1;
		// }											
		// HAL_UART_Receive_IT(&huart2, &rx_data, 1);

		
	// }
	
	
}



// void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
// {
	
// }