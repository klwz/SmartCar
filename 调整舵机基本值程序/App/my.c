/*!
 *     COPYRIGHT NOTICE
 *     Copyright (c) 2016，晨旭
 *     All rights reserved.
 *     技术讨论：晨旭的博客 http://www.chenxublog.com
 *
 *     除注明出处外，以下所有内容版权均属晨旭所有，未经允许，不得用于商业用途，
 *     修改内容时必须保留晨旭的版权声明。
 *
 * @file       my.c
 * @brief      K60自编函数
 * @author     晨旭
 * @version    v1.0
 * @date       2016-3-7
 */

#include "common.h"
#include"include.h"


/*!
 *  @brief      K60初始化
 *  @since      v1.0
 *  Sample usage:            mk60int();    //初始化K60
 */
void mk60int()
{
    ftm_pwm_init(FTM0, FTM_CH2,10*1000,0);         //初始化 FTM PWM ，使用 FTM0_CH3，频率为10k ，占空比为 100 / FTM0_PRECISON
    ftm_pwm_init(FTM0, FTM_CH3,10*1000,0);
    ftm_pwm_init(FTM0, FTM_CH4,10*1000,0);
    ftm_pwm_init(FTM0, FTM_CH5,10*1000,0);

    ftm_pwm_init(S3010_FTM, S3010_CH,S3010_HZ,100);
    control_actuator(0);

    //OLED_Init();    //OLED初始化

//    pit_init_ms(PIT0, 5);                                //初始化PIT0，定时时间为： 5ms
//    set_vector_handler(PIT0_VECTORn ,PIT0_IRQHandler);      //设置PIT0的中断服务函数为 PIT0_IRQHandler
//    enable_irq (PIT0_IRQn);                                 //使能PIT0中断

    key_init(KEY_U);
    key_init(KEY_D);
}


/*!
 *  @brief      舵机输出函数
 *  @since      v1.1
 *  @note       输入值范围-1～1   浮点型
 *  Sample usage:            control_actuator(-0.2);    //输出舵机反向0.2
 */
void control_actuator(float Voltage)
{
    if(Voltage>1)
        Voltage=1;
    else if(Voltage<-1)
        Voltage=-1;

    if(Voltage<0)
    {
        ftm_pwm_duty(S3010_FTM, S3010_CH,(int)((control_actuator_center-control_actuator_min)*Voltage+control_actuator_center));
    }else
    {
        ftm_pwm_duty(S3010_FTM, S3010_CH,(int)((control_actuator_max-control_actuator_center)*Voltage+control_actuator_center));
    }
}


/*!
 *  @brief      电机输出函数
 *  @since      v1.2
 *  @note       输入值范围-1～1   浮点型
 *  Sample usage:            SetMotorVoltage(0.1,0.2);    //输出左电机正向0.1，右电机正向0.2
 */
void SetMotorVoltage(float fLeftVoltage,float fRightVoltage)
{
    if(fLeftVoltage>1)
        fLeftVoltage=1;
    else if(fLeftVoltage<-1)
        fLeftVoltage=-1;
    if(fRightVoltage>1)
        fRightVoltage=1;
    else if(fRightVoltage<-1)
        fRightVoltage=-1;

    //int nOut;
    if(fLeftVoltage>0)
    {
	ftm_pwm_duty(FTM0,FTM_CH2,0);//左轮正向运动PWM占空比为0
	//nOut=(int)(fLeftVoltage*PERIOD);
	ftm_pwm_duty(FTM0,FTM_CH3,(int)(fLeftVoltage*PERIOD));//左轮反向运动PWM占空比为nOut
    }                                                   //左电机正转
    else
    {
	ftm_pwm_duty(FTM0,FTM_CH3,0);//左轮反向运动PWM占空比为0
	fLeftVoltage=-fLeftVoltage;
	//nOut=(int)(fLeftVoltage*PERIOD);
	ftm_pwm_duty(FTM0,FTM_CH2,(int)(-fLeftVoltage*PERIOD));//左轮正向运动PWM占空比为nOut
    }                                                    //左电机反转

    if(fRightVoltage>0)
    {
	ftm_pwm_duty(FTM0,FTM_CH4,0);//右轮正向运动PWM占空比为0
	//nOut=(int)(fRightVoltage*PERIOD);
	ftm_pwm_duty(FTM0,FTM_CH5,(int)(fRightVoltage*PERIOD));//右轮反向运动PWM占空比为nOut
    }                                                     //右电机正转
    else
    {
	ftm_pwm_duty(FTM0,FTM_CH5,0);//右轮反向运动PWM占空比为0
	fRightVoltage=-fRightVoltage;
	//nOut=(int)(fRightVoltage*PERIOD);
	ftm_pwm_duty(FTM0,FTM_CH4,(int)(-fRightVoltage*PERIOD));//右轮正向运动PWM占空比为nOut
    }                                                     //右电机反转
}
