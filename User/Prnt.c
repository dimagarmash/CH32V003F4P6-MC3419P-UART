/*
 * Prnt.c
 *
 *  Created on: Feb 21, 2025
 *      Author: Dmitry
 */
#include "debug.h"
#include "system.h"
#include <stdio.h>  // §¥§Ý§ñ vsprintf
#include <stdarg.h> // §¥§Ý§ñ §â§Ñ§Ò§à§ä§í §ã §á§Ö§â§Ö§Þ§Ö§ß§ß§í§Þ §é§Ú§ã§Ý§à§Þ §Ñ§â§Ô§å§Þ§Ö§ß§ä§à§Ó
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
    char buffer[128];  // §¢§å§æ§Ö§â §Õ§Ý§ñ §ç§â§Ñ§ß§Ö§ß§Ú§ñ §æ§à§â§Þ§Ñ§ä§Ú§â§à§Ó§Ñ§ß§ß§à§Û §ã§ä§â§à§Ü§Ú
        va_list args;      // §³§á§Ú§ã§à§Ü §Ñ§â§Ô§å§Þ§Ö§ß§ä§à§Ó

        // §ª§ß§Ú§è§Ú§Ñ§Ý§Ú§Ù§Ñ§è§Ú§ñ §ã§á§Ú§ã§Ü§Ñ §Ñ§â§Ô§å§Þ§Ö§ß§ä§à§Ó
        va_start(args, format);

        // §¶§à§â§Þ§Ñ§ä§Ú§â§à§Ó§Ñ§ß§Ú§Ö §ã§ä§â§à§Ü§Ú §ã §Ú§ã§á§à§Ý§î§Ù§à§Ó§Ñ§ß§Ú§Ö§Þ vsprintf
        vsprintf(buffer, format, args);

        // §©§Ñ§Ó§Ö§â§ê§Ö§ß§Ú§Ö §â§Ñ§Ò§à§ä§í §ã §Ñ§â§Ô§å§Þ§Ö§ß§ä§Ñ§Þ§Ú
        va_end(args);

        // §°§ä§á§â§Ñ§Ó§Ü§Ñ §æ§à§â§Þ§Ñ§ä§Ú§â§à§Ó§Ñ§ß§ß§à§Û §ã§ä§â§à§Ü§Ú §á§à UART
        print(buffer);
}
 void printfl(short f ) {



         // §±§à§Ý§å§é§Ñ§Ö§Þ §è§Ö§Ý§å§ð §é§Ñ§ã§ä§î
         short s = f/10;
         short ui= (f-(s*10));
         if(ui<0)
         {ui *= -1;}
         //printn("|%d|",f);
         printn("|%d.%d|",s,ui);


         // §±§à§Ý§å§é§Ñ§Ö§Þ §Õ§â§à§Ò§ß§å§ð §é§Ñ§ã§ä§î



}

