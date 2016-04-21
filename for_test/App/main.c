/*!
 *     COPYRIGHT NOTICE
 *     Copyright (c) 2013,山外科技
 *     All rights reserved.
 *     技术讨论：山外论坛 http://www.vcan123.com
 *
 *     除注明出处外，以下所有内容版权均属山外科技所有，未经允许，不得用于商业用途，
 *     修改内容时必须保留山外科技的版权声明。
 *
 * @file       main.c
 * @brief      山外K60 平台主程序
 * @author     山外科技
 * @version    v5.0
 * @date       2013-08-28
 */

#include "include.h"

uint8 imgbuff[CAMERA_SIZE];                             //定义存储接收图像的数组
uint8 img[OV7725_EAGLE_H][OV7725_EAGLE_W];              //定义存储解压图像的数组
float speed=0.2,duoji=0;
char ch;
float out[4];
int x[OV7725_EAGLE_H],y[OV7725_EAGLE_W];

struct _slope slope;

int oled_place=0;

//函数声明
void PORTA_IRQHandler();
void DMA0_IRQHandler();


/*!
 *  @brief      扫键函数
 *  @since
 *  @note
 */
void oled_display_key()
{
    switch(oled_place)
    {
      case 0 :
//        if(key_check(KEY_L) == KEY_DOWN)
//        {
//            dj+=0.01;
//            control_actuator(dj);
//        }
//        if(key_check(KEY_R) == KEY_DOWN)
//        {
//            dj-=0.01;
//            control_actuator(dj);
//        }
        break;

      case 1 :
//        if(key_check(KEY_A) == KEY_DOWN)
//        {
//            get_deformation_ratio(img,deformation_ratio);
//            for(i=0;i<OV7725_EAGLE_H;i++)
//            {
//                 printf("deformation_ratio[%d]=%d\n",i,deformation_ratio[i]);
//            }
//        }
        break;

      case 2 :
//        if(key_check(KEY_A) == KEY_DOWN)
//        {
//            get_deformation_ratio(img,deformation_ratio);
//            for(i=0;i<OV7725_EAGLE_H;i++)
//            {
//                 printf("deformation_ratio[%d]=%d\n",i,deformation_ratio[i]);
//            }
//            deformation_get=1;
//        }
        break;

      default:
        break;
    }
//        OLED_P8x16Str(0,0,"Actuator:");
//        Display_number(80,0,(int32)(dj*100));
//
//        OLED_P14x16Str(0,2,"摄像头偏差：");
//        Display_number(80,2,get_camere_center(img,10));
//        Display_number(80,3,get_camere_center(img,40));
//
//        OLED_P14x16Str(0,4,"赛道信息：");
//        if(deformation_get)
//            OLED_P14x16Str(70,4,"已提取");
//        else
//            OLED_P14x16Str(70,4,"未提取");


        if(key_check(KEY_U) == KEY_DOWN)
        {
            oled_place--;
            if(oled_place<0)
                oled_place=3;
        }
        if(key_check(KEY_D) == KEY_DOWN)
        {
            oled_place++;
            if(oled_place>3)
                oled_place=0;
        }
        OLED_P14x16Str(110,0,"　");
        OLED_P14x16Str(110,2,"　");
        OLED_P14x16Str(110,4,"　");
        OLED_P14x16Str(110,6,"　");
        OLED_P14x16Str(110,oled_place*2,"←");
}


/*!
 *  @brief      main函数
 *  @since
 *  @note
 */
void  main(void)
{

    camera_init(imgbuff);

    //配置中断服务函数
set_vector_handler(PORTA_VECTORn , PORTA_IRQHandler);   //设置LPTMR的中断服务函数为 PORTA_IRQHandler
set_vector_handler(DMA0_VECTORn , DMA0_IRQHandler);     //设置LPTMR的中断服务函数为 PORTA_IRQHandler


    mk60int();
    while(1)
    {
        camera_get_img();                                   //摄像头获取图像

        img_extract(img,imgbuff,CAMERA_SIZE);               //解压到img中

        get_slope(img,&slope);


        out[0]=slope.left;
        out[1]=slope.right;
        out[2]=(float)slope.left_count/10;
        out[3]=(float)slope.right_count/10;
        vcan_sendware(out,sizeof(out));
        vcan_sendimg(imgbuff,CAMERA_SIZE);                  //串口显示

    }
}



/*!
 *  @brief      PORTA中断服务函数
 *  @since      v5.0
 */
void PORTA_IRQHandler()
{
    uint8  n;    //引脚号
    uint32 flag;

    while(!PORTA_ISFR);
    flag = PORTA_ISFR;
    PORTA_ISFR  = ~0;                                   //清中断标志位

    n = 29;                                             //场中断
    if(flag & (1 << n))                                 //PTA29触发中断
    {
        camera_vsync();
    }
#if ( CAMERA_USE_HREF == 1 )                            //使用行中断
    n = 28;
    if(flag & (1 << n))                                 //PTA28触发中断
    {
        camera_href();
    }
#endif


}

/*!
 *  @brief      DMA0中断服务函数
 *  @since      v5.0
 */
void DMA0_IRQHandler()
{
    camera_dma();
}


