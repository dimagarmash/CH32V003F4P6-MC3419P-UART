/*
 * Prnt.c
 *
 *  Created on: Feb 21, 2025
 *      Author: Dmitry
 */
#include "debug.h"
#include "system.h"
#include <stdio.h>  // ���ݧ� vsprintf
#include <stdarg.h> // ���ݧ� ��ѧҧ��� �� ��֧�֧ާ֧ߧߧ�� ��ڧ�ݧ�� �ѧ�ԧ�ާ֧ߧ���
//#include <math.h>

// its all for optimization because standart method  so terrible
void print(char *data){
    while(*data)
       {

           while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET)
           {
           }
           USART_SendData(USART1,*data++);
       }
}
void printn(char *format, ...){
    char buffer[128];  // �����֧� �էݧ� ���ѧߧ֧ߧڧ� ����ާѧ�ڧ��ӧѧߧߧ�� �����ܧ�
        va_list args;      // ����ڧ��� �ѧ�ԧ�ާ֧ߧ���

        // ���ߧڧ�ڧѧݧڧ٧ѧ�ڧ� ���ڧ�ܧ� �ѧ�ԧ�ާ֧ߧ���
        va_start(args, format);

        // �����ާѧ�ڧ��ӧѧߧڧ� �����ܧ� �� �ڧ���ݧ�٧�ӧѧߧڧ֧� vsprintf
        vsprintf(buffer, format, args);

        // ���ѧӧ֧��֧ߧڧ� ��ѧҧ��� �� �ѧ�ԧ�ާ֧ߧ�ѧާ�
        va_end(args);

        // ������ѧӧܧ� ����ާѧ�ڧ��ӧѧߧߧ�� �����ܧ� ��� UART
        print(buffer);
}
 void printfl(short f ) {



         // ����ݧ��ѧ֧� ��֧ݧ�� ��ѧ���
         short s = f/10;
         short ui= (f-(s*10));
         if(ui<0)
         {ui *= -1;}
         //printn("|%d|",f);
         printn("|%d.%d|",s,ui);


         // ����ݧ��ѧ֧� �է��ҧߧ�� ��ѧ���



}

