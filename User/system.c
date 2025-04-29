/*
 * system.c
 *
 *  Created on: Jan 25, 2025
 *      Author: Dmitry
 */

#include "debug.h"
#include "system.h"

#include "Prnt.h"


/* SPI Mode Definition */
#define HOST_MODE     1
#define SLAVE_MODE    0

/* SPI Communication Mode Selection */
#define SPI_MODE      HOST_MODE


/* Global define */
#define Size          18


 uint16_t PeriodForTimer=0xbb80;//this value is a divider for the clock bus // 0xbb80~1000ms
 //§ï§ä§Ñ §á§Ö§â§Ö§Þ§Ö§ß§ß§Ñ§ñ §ñ§Ó§Ý§ñ§Ö§ä§ã§ñ §Õ§Ö§Ý§Ú§ä§Ö§Ý§Ö§Þ §Õ§Ý§ñ §à§ã§ß§à§Ó§ß§à§Û §ê§Ú§ß§í §ä§Ñ§Ü§ä§Ú§â§à§Ó§Ñ§ß§Ú§ñ §Ú §Ù§ß§Ñ§é§Ö§ß§Ú§Ö §Õ§Ö§Ý§Ö§ß§Ú§ñ §Ú §Ò§å§Õ§Ö§ä §é§Ñ§ã§ä§à§ä§à§Û §ä§Ñ§Û§Þ§Ö§â§Ñ
 uint32_t MillisAfterStart=0;

/* Global Variable */



/* Global define */



/*********************************************************************
 * @fn      USARTx_CFG
 *
 * @brief   Initializes the USART2 & USART3 peripheral.
 *
 * @return  none
 */
uint8_t  LedRedState=0;
uint8_t LedGreenState=0;
uint8_t LedBlueState=0;
uint8_t Btn1State=0;
uint8_t Btn2State=0;
uint8_t GateState=0;

void SPI_FullDuplex_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure = {0};
    SPI_InitTypeDef  SPI_InitStructure = {0};

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC | RCC_APB2Periph_SPI1, ENABLE);


    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOC, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOC, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOC, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOC, &GPIO_InitStructure);

    SPI_SSOutputCmd(SPI1, ENABLE);
    SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
    SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
    SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
    SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;//high
    SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;//2edge
    SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
    SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_16;
    SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
    SPI_Init(SPI1, &SPI_InitStructure);

    SPI_Cmd(SPI1, ENABLE);
    ChipSelectHigh();
}

void SpiSend8bit(uint8_t Data){

    SPI_I2S_SendData(SPI1, Data);
    while (SPI_I2S_GetFlagStatus(SPI1,SPI_I2S_FLAG_TXE) == RESET ) {}
    SPI_I2S_ClearFlag(SPI1, SPI_I2S_FLAG_TXE);
}
uint8_t SpiRecive8bit(void){
    uint8_t result=0;
    if(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) != RESET)
    {
        result = SPI_I2S_ReceiveData(SPI1);
        SPI_I2S_ClearFlag(SPI1, SPI_I2S_FLAG_RXNE);
       // print("ReadFlagOk\r\n");
    }
    else {
      //  print("ReadFlagNotOk\r\n");
    }

    return result;
}
void ClearSpiTxFlag(void){
    SPI_I2S_ClearFlag(SPI1, SPI_I2S_FLAG_TXE);
}
void ClearSpiRxFlag(void){
    SPI_I2S_ClearFlag(SPI1, SPI_I2S_FLAG_RXNE);
}
void ChipSelectHigh(void){
    GPIO_WriteBit(GPIOC, GPIO_Pin_1, Bit_SET);
}
void ChipSelectLow(void){
    GPIO_WriteBit(GPIOC, GPIO_Pin_1, Bit_RESET);
}
void USARTx_CFG(void)

{
    GPIO_InitTypeDef  GPIO_InitStructure = {0};
    USART_InitTypeDef USART_InitStructure = {0};

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD | RCC_APB2Periph_USART1, ENABLE);

    /* USART1 TX-->D.5   RX-->D.6 */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_30MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOD, &GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOD, &GPIO_InitStructure);

    USART_InitStructure.USART_BaudRate = 115200;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;

    USART_Init(USART1, &USART_InitStructure);
    USART_Cmd(USART1, ENABLE);
}

void GPIO_INIT(void){

    GPIO_InitTypeDef  GPIO_InitStructure = {0};
    RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOD | RCC_APB2Periph_USART1, ENABLE);
    RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOC | RCC_APB2Periph_SPI1, ENABLE );
    RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOA, ENABLE );

    //ledg init
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOC, &GPIO_InitStructure);

    //ledr init
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOC, &GPIO_InitStructure);

    //ledb init
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOC, &GPIO_InitStructure);

    //gate init
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOD, &GPIO_InitStructure);

    //Btn1 init
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    //Btn2 init
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    LedRedState=0;
    LedGreenState=0;
    LedBlueState=0;
    Btn1State=0;
    Btn2State=0;
    GateState=0;



}
void TMR_2_Init(uint16_t Period) // 1 ms
{
    TIM_TimeBaseInitTypeDef  TimeBase_InitStructure;
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

    TimeBase_InitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TimeBase_InitStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TimeBase_InitStructure.TIM_Period = Period; // 0x5dc0; // ~ 1000 §¤§¸
    TimeBase_InitStructure.TIM_Prescaler = 1;
    TimeBase_InitStructure.TIM_RepetitionCounter= 0;
    //TIM2->CNT = 10000;
    TIM_TimeBaseInit( TIM2, &TimeBase_InitStructure );
    TIM_ClearITPendingBit( TIM2, 0X0FF);

    TIM_ITConfig( TIM2, TIM_IT_Update, ENABLE );
    NVIC_EnableIRQ( TIM2_IRQn );
    TIM_Cmd( TIM2, ENABLE );
}


// *********************************************************************

uint16_t Count_RF_LED = 0;

void TIM2_IRQHandler(void) __attribute__((interrupt("WCH-Interrupt-fast")));
void TIM2_IRQHandler(void)
{
    //LedBlueToggle();
    if(TIM_GetFlagStatus( TIM2, TIM_FLAG_Update )!=RESET)
        {
            TIM_ClearITPendingBit( TIM2, TIM_IT_Update );     /* Clear Flag */
        }
    MillisAfterStart++;
}
// *********************************************************************

uint32_t GetTime(void){
    return MillisAfterStart;
}

