/********************************** (C) COPYRIGHT *******************************
 * File Name          : main.c
 * Author             : WCH
 * Version            : V1.0.0
 * Date               : 2023/12/25
 * Description        : Main program body.
 *********************************************************************************
 * Copyright (c) 2021 Nanjing Qinheng Microelectronics Co., Ltd.
 * Attention: This software (modified or not) and binary are used for 
 * microcontroller manufactured by Nanjing Qinheng Microelectronics.
 *******************************************************************************/

/*
 *@Note
 *Multiprocessor communication mode routine:
 *Master:USART1_Tx(PD5)\USART1_Rx(PD6).
 *This routine demonstrates that USART1 receives the data sent by CH341 and inverts
 *it and sends it (baud rate 115200).
 *
 *Hardware connection:PD5 -- Rx
 *                     PD6 -- Tx
 *
 */

#include "debug.h"
//#include "string.h"
#include "../mc3419/accelerometer.h"
#include "system.h"
#include "../mc3419/MC34X9.h"
#include "Prnt.h"
//#include <math.h> eated to much resources
/* SPI Mode Definition */



/*********************************************************************
 * @fn      main
 *
 * @brief   Main program.
 *
 * @return  none
 *///
/* Global Variable */
vu8 val;

//in main for optimize
float my_sqrt(float number) { //fast sqrt like quake style

    const float threehalfs = 1.5F;

       float x2 = number * 0.5F;
       float y = number;

       // ���ѧԧڧ�֧�ܧѧ� �ܧ�ߧ��ѧߧ��
       uint32_t i = *(uint32_t *)&y;          // ����֧է��ѧӧݧ֧ߧڧ� float �� �ӧڧէ� ��֧ݧ�ԧ� ��ڧ�ݧ�
       i = 0x5f3759df - (i >> 1);             // ���ѧԧڧ�֧�ܧѧ� �ܧ�ߧ��ѧߧ�� �� ��էӧڧ�
       y = *(float *)&i;                      // ���ҧ�ѧ�ߧ�� ���֧�ҧ�ѧ٧�ӧѧߧڧ� �� float

       // ���էߧ� �ڧ�֧�ѧ�ڧ� �ާ֧��է� �������ߧ� �էݧ� �����ߧ֧ߧڧ� ��֧٧�ݧ��ѧ��
       y = y * (threehalfs - (x2 * y * y));  // y = y * (1.5 - 0.5 * x * y^2)
       y = y * (threehalfs - (x2 * y * y));  // ������ѧ� �ڧ�֧�ѧ�ڧ�
       return 1.0F / y;  // ����٧ӧ�ѧ�ѧ֧� �ܧӧѧէ�ѧ�ߧ�� �ܧ��֧ߧ�
}
//in main for optimize
float GetOverload(struct MC34X9_Overload ValuebyAccel) {
    short totalOverload=0;
    float G= 9.8f;
    float f=0;
    ValuebyAccel.XAxis_g=(float)ValuebyAccel.XAxis_g/10;
    ValuebyAccel.YAxis_g=(float)ValuebyAccel.YAxis_g/10;
    ValuebyAccel.ZAxis_g=(float)ValuebyAccel.ZAxis_g/10;
    //print("|---------------------------------------------------------|\r\n"); printn("SELECT AXIS: %d\r\n",GetParameters()[SELECT_AXIS].UCurrentValue);
    //restricted because its demo pjct
    switch (1) {

    case 1:
        f=((ValuebyAccel.XAxis_g*ValuebyAccel.XAxis_g) + (ValuebyAccel.YAxis_g*ValuebyAccel.YAxis_g) + (ValuebyAccel.ZAxis_g*ValuebyAccel.ZAxis_g));
        f=(my_sqrt(f)-G)*10;
        totalOverload =(short)f;
//       print("Total Overload ");
//        printfl(totalOverload);
//        print("Total Overload \r\n");
        //totalOverload-=G;
        if (totalOverload<0) {
            totalOverload=totalOverload * -1;
        }
        break;
    case 0:
        f=((ValuebyAccel.XAxis_g*ValuebyAccel.XAxis_g) + (ValuebyAccel.YAxis_g*ValuebyAccel.YAxis_g));
        f=(my_sqrt(f))*10;
        totalOverload =(short)f;
        //totalOverload = my_sqrt(( (ValuebyAccel.XAxis_g*ValuebyAccel.XAxis_g) + (ValuebyAccel.YAxis_g*ValuebyAccel.YAxis_g)));
//        totalOverload-=G;
//        if (totalOverload<0) {
//           totalOverload=totalOverload * -1;
//        }
        break;
    case 2:
        totalOverload = ValuebyAccel.XAxis_g;
        break;
    case 3:
        totalOverload = ValuebyAccel.YAxis_g;
        break;
    case 4:
        totalOverload = ValuebyAccel.ZAxis_g;
        break;
    }

    return totalOverload;
}



int main(void)
{

    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
    SystemCoreClockUpdate();
    Delay_Init();
    Delay_Ms(50);
#if (SDI_PRINT == SDI_PR_OPEN)
    SDI_Printf_Enable();
#else
    USART_Printf_Init(115200);
#endif
    printn("SystemClk:%d\r\n",SystemCoreClock);
    printn( "ChipID:%08x\r\n", DBGMCU_GetCHIPID() );
    print( "Call ConfigsInit \r\n");
  
    SPI_FullDuplex_Init();
    GPIO_INIT();
    TMR_2_Init(PERIOD_FOR_TIMER);

struct MC34X9_Overload ValuebyAccel;
   short totalOverload=0;
   
   setup();

   while(1)
    {
        Delay_Ms(1);
        ValuebyAccel= readMeanOverload(); //readAndOutput();
        totalOverload = GetOverload(ValuebyAccel);
        printn("totalOverload: \t");printfl(totalOverload); print("\t\r\n");
    }
}
