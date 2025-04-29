



#ifndef USER_SYSTEM_H_
#define USER_SYSTEM_H_
#include "stdint.h"
#define PERIOD_FOR_TIMER 0x5dc0
#define  LedBlinkTime 150 //ms
void SPI_FullDuplex_Init(void);
void SpiSend8bit(uint8_t Data);
uint8_t SpiRecive8bit(void);
void ClearSpiTxFlag(void);
void ClearSpiRxFlag(void);
void ChipSelectHigh(void);
void ChipSelectLow(void);
void USARTx_CFG(void);

void GPIO_INIT(void);
void TMR_2_Init(uint16_t Period);
uint32_t GetTime(void);




#endif
