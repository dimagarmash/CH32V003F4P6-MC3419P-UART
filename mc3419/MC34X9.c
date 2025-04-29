/******************************************************************************

   Copyright (c) 2020 mCube, Inc.  All rights reserved.

   This source is subject to the mCube Software License.
   This software is protected by Copyright and the information and source code
   contained herein is confidential. The software including the source code
   may not be copied and the information contained herein may not be used or
   disclosed except with the written permission of mCube Inc.

   All other rights reserved.
 *****************************************************************************/

/**
   @file    MC34X9.c
   @author  mCube
   @date    13 January 2020
   @brief   Driver interface header file for accelerometer mc34X9 series.
   @see     http://www.mcubemems.com
*/

#include "MC34X9.h"
#include "debug.h"
#include "system.h"
#include "Prnt.h"


#define MC34X9_CFG_MODE_DEFAULT                 MC34X9_MODE_STANDBY
#define MC34X9_CFG_SAMPLE_RATE_DEFAULT    MC34X9_SR_DEFAULT_2000Hz
#define MC34X9_CFG_RANGE_DEFAULT                MC34X9_RANGE_8G


uint8_t M_bSpi;//like a bool
uint8_t M_chip_select;
short x, y, z;
    // Raw Accelerometer data
struct MC34X9_Overload AccRaw;



uint8_t CfgRange, CfgFifo;

//Initialize the MC34X9 sensor and set as the default configuration
volatile uint8_t  start()
{
  //Init Reset
 // print("call reset\r\n");
  reset();
 // print("call SetMode\r\n");
  SetMode(MC34X9_MODE_STANDBY);
  /* Check I2C connection */
  uint8_t id = readRegister8(MC34X9_REG_PROD);
  //printn("Chip ID: %d\r\n",id);
  if (id != MC34X9_CHIP_ID)
  {
    /* No MC34X9 detected ... return false */
   // print("No MC34X9 detected!\r\n");
    //printn("Chip ID: %d\r\n",id);
    return 0;

  }
  //Range: 8g
//  print("call SetRangeCtrl\r\n");
  SetRangeCtrl(MC34X9_CFG_RANGE_DEFAULT);
  //Sampling Rate: 50Hz by default
 // print("call SetSampleRate\r\n");
  SetSampleRate(MC34X9_CFG_SAMPLE_RATE_DEFAULT);
  //Mode: Active
  //print("call SetMode \r\n");
  SetMode(MC34X9_MODE_CWAKE);

//  Delay_Ms(5);

  return 1;
}

//void wake()
//{
//  //Set mode as wake
//  SetMode(MC34X9_MODE_CWAKE);
//}

//void stop()
//{
//  //Set mode as Sleep
//  SetMode(MC34X9_MODE_STANDBY);
//}

//Initial reset
void reset()
{
  // Stand by mode
  writeRegister8(MC34X9_REG_MODE, MC34X9_MODE_STANDBY);
  Delay_Ms(1);

  Delay_Ms(5);

  writeRegister8(0x06, 0x00);

  Delay_Ms(5);
}

//Set the operation mode
void SetMode(MC34X9_mode_t mode)
{
  uint8_t value;

  value = readRegister8(MC34X9_REG_MODE);
  value &= 0b11110000;
  value |= mode;

  writeRegister8(MC34X9_REG_MODE, value);
}

//Set the range control
void SetRangeCtrl(MC34X9_range_t range)
{
  uint8_t value;
  CfgRange = range;
  SetMode(MC34X9_MODE_STANDBY);
  value = readRegister8(MC34X9_REG_RANGE_C);
  value &= 0b00000111;
  value |= (range << 4) & 0x70;
  writeRegister8(MC34X9_REG_RANGE_C, value);
}

//Set the sampling rate
void SetSampleRate(MC34X9_sr_t sample_rate)
{
  uint8_t value;
  SetMode(MC34X9_MODE_STANDBY);
  value = readRegister8(MC34X9_REG_SR);
  value &= 0b00000000;
  value |= sample_rate;
  writeRegister8(MC34X9_REG_SR, value);
}

// Set Motion feature
//void SetMotionCtrl(uint8_t tilt_ctrl,
//        uint8_t flip_ctl,
//        uint8_t anym_ctl,
//        uint8_t shake_ctl,
//        uint8_t tilt_35_ctl) {
//  uint8_t CfgMotion = 0;
//
//  if (tilt_ctrl || flip_ctl) {
//    _M_DRV_MC34X6_SetTilt_Flip();
//    CfgMotion |= (((tilt_ctrl || flip_ctl) & 0x01) << MC34X9_TILT_FEAT);
//  }
//
//  if (anym_ctl) {
//    _M_DRV_MC34X6_SetAnym();
//    CfgMotion |= ((anym_ctl & 0x01) << MC34X9_ANYM_FEAT);
//  }
//
//  if (shake_ctl) {
//    _M_DRV_MC34X6_SetShake();
//    // Also enable anyMotion feature
//    CfgMotion |= ((shake_ctl & 0x01) << MC34X9_ANYM_FEAT) | ((shake_ctl & 0x01) << MC34X9_SHAKE_FEAT);
//  }
//
//  if (tilt_35_ctl) {
//    _M_DRV_MC34X6_SetTilt35();
//    // Also enable anyMotion feature
//    CfgMotion |= ((tilt_35_ctl & 0x01) << MC34X9_ANYM_FEAT) | ((tilt_35_ctl & 0x01) << MC34X9_TILT35_FEAT);
//  }
//
//  writeRegister8(MC34X9_REG_MOTION_CTRL, CfgMotion);
//}

//Set FIFO feature
//void SetFIFOCtrl(MC34X9_fifo_ctl_t fifo_ctl,
//                         MC34X9_fifo_mode_t fifo_mode,
//                         uint8_t fifo_thr)
//{
//  if (fifo_thr > 31)  //maximum threshold
//    fifo_thr = 31;
//
//  SetMode(MC34X9_MODE_STANDBY);
//
//  CfgFifo = (MC34X9_COMB_INT_ENABLE << 3) | ((fifo_ctl << 5) | (fifo_mode << 6)) ;
//
//  writeRegister8(MC34X9_REG_FIFO_CTRL, CfgFifo);
//
//  uint8_t CfgFifoThr = fifo_thr;
//  writeRegister8(MC34X9_REG_FIFO_TH, CfgFifoThr);
//}
//
//void SetGerneralINTCtrl() {
//  // Gerneral Interrupt setup
//  uint8_t CfgGPIOINT = (((MC34X9_INTR_C_IAH_ACTIVE_LOW & 0x01) << 2) // int1
//                        | ((MC34X9_INTR_C_IPP_MODE_OPEN_DRAIN & 0x01) << 3)// int1
//                        | ((MC34X9_INTR_C_IAH_ACTIVE_LOW & 0x01) << 6)// int2
//                        | ((MC34X9_INTR_C_IPP_MODE_OPEN_DRAIN & 0x01) << 7));// int2
//
//  writeRegister8(MC34X9_REG_GPIO_CTRL, CfgGPIOINT);
//}
//
////Set interrupt control register
//void SetINTCtrl(uint8_t tilt_int_ctrl,
//                uint8_t flip_int_ctl,
//                uint8_t anym_int_ctl,
//                uint8_t shake_int_ctl,
//                uint8_t tilt_35_int_ctl)
//{
//  SetMode(MC34X9_MODE_STANDBY);
//
//  uint8_t CfgINT = (((tilt_int_ctrl & 0x01) << 0)
//                    | ((flip_int_ctl & 0x01) << 1)
//                    | ((anym_int_ctl & 0x01) << 2)
//                    | ((shake_int_ctl & 0x01) << 3)
//                    | ((tilt_35_int_ctl & 0x01) << 4)
//                    | ((MC34X9_AUTO_CLR_ENABLE & 0x01) << 6));
//  writeRegister8(MC34X9_REG_INTR_CTRL, CfgINT);
//
//  SetGerneralINTCtrl();
//}
//
////Set FIFO interrupt control register
//void SetFIFOINTCtrl(uint8_t fifo_empty_int_ctl,
//                    uint8_t fifo_full_int_ctl,
//                    uint8_t fifo_thr_int_ctl)
//{
//  SetMode(MC34X9_MODE_STANDBY);
//
//  CfgFifo = CfgFifo
//            | (((fifo_empty_int_ctl & 0x01) << 0)
//               | ((fifo_full_int_ctl & 0x01) << 1)
//               | ((fifo_thr_int_ctl & 0x01) << 2));
//
//  writeRegister8(MC34X9_REG_FIFO_CTRL, CfgFifo);
//
//  SetGerneralINTCtrl();
//}
//
////Interrupt handler (clear interrupt flag)
//void INTHandler(MC34X9_interrupt_event_t *ptINT_Event)
//{
//  uint8_t value;
//
//  value = readRegister8(MC34X9_REG_INTR_STAT);
//
//  ptINT_Event->bTILT           = ((value >> 0) & 0x01);
//  ptINT_Event->bFLIP           = ((value >> 1) & 0x01);
//  ptINT_Event->bANYM           = ((value >> 2) & 0x01);
//  ptINT_Event->bSHAKE          = ((value >> 3) & 0x01);
//  ptINT_Event->bTILT_35        = ((value >> 4) & 0x01);
//
//  value &= 0x40;
//  writeRegister8(MC34X9_REG_INTR_STAT, value);
//}
//
////FIFO Interrupt handler (clear interrupt flag)
//void FIFOINTHandler(MC34X9_fifo_interrupt_event_t *ptFIFO_INT_Event)
//{
//  uint8_t value;
//
//  value = readRegister8(MC34X9_REG_FIFO_INTR);
//
//  ptFIFO_INT_Event->bFIFO_EMPTY           = ((value >> 0) & 0x01);
//  ptFIFO_INT_Event->bFIFO_FULL            = ((value >> 1) & 0x01);
//  ptFIFO_INT_Event->bFIFO_THRESH          = ((value >> 2) & 0x01);
//}

//Get the range control
//MC34X9_range_t GetRangeCtrl(void)
//{
//  // Read the data format register to preserve bits
//  uint8_t value;
//  value = readRegister8(MC34X9_REG_RANGE_C);
//  printn("In GetRangeCtrl(): %d\r\n",value);
//
//  value &= 0x70;
//  return (MC34X9_range_t) (value >> 4);
//}
//
////Get the output sampling rate
//MC34X9_sr_t GetSampleRate(void)
//{
//  // Read the data format register to preserve bits
//  uint8_t value;
//  value = readRegister8(MC34X9_REG_SR);
//  printn("In GetCWakeSampleRate(): %d\r\n",value);
//
//  value &= 0b00001111;
//  return (MC34X9_sr_t) (value);
//}
//
////Is FIFO empty
//uint8_t IsFIFOEmpty(void)
//{
//  // Read the data format register to preserve bits
//  uint8_t value;
//  value = readRegister8(MC34X9_REG_FIFO_STAT);
//  value &= 0x01;
//  //Serial.println("FIFO_Status");
//  //Serial.println(value, HEX);
//
//  if (value ^ 0x01)
//    return 0;	//Not empty
//  else {
//    return 1;  //Is empty
//  }
//}

//Read the raw counts and SI units measurement data
 struct MC34X9_Overload readRawAccel(void)
{
  //{2g, 4g, 8g, 16g, 12g}
  float faRange[5] = { 19.614f, 39.228f, 78.456f, 156.912f, 117.684f};
  // 16bit
  float faResolution = 32768.0f;

  uint8_t rawData[6];//like a byte
  // Read the six raw data registers into data array
  mcube_read_regs( MC34X9_REG_XOUT_LSB, rawData, 6);
  uint8_t i=0;
  while(i<6){
      //its been need for debugging
//     printn("pos: %d ",i);
//     printn("Data:0x%X\r\n" ,rawData[i]);
     i++;
  }
  x = (short)((((unsigned short)rawData[1]) << 8) | rawData[0]);
  y = (short)((((unsigned short)rawData[3]) << 8) | rawData[2]);
  z = (short)((((unsigned short)rawData[5]) << 8) | rawData[4]);

  //AccRaw.XAxis = (short) (x);
//  AccRaw.YAxis = (short) (y);
 // AccRaw.ZAxis = (short) (z);
  AccRaw.XAxis_g = ((float) (x) / faResolution * faRange[CfgRange])*10;
  AccRaw.YAxis_g = ((float) (y) / faResolution * faRange[CfgRange])*10;
  AccRaw.ZAxis_g = ((float) (z) / faResolution * faRange[CfgRange])*10;

  return AccRaw;
}


uint8_t mcube_read_regs( uint8_t reg,  uint8_t * value, uint8_t size)
{
  for (uint8_t i = 0; i < size; i++) {
    value[i] = readRegister8( reg + i);
  }
  return 0;
}

/** I2C/SPI write function */
/** bSpi : I2C/SPI bus selection.        SPI: 0,       I2C: 1           */
/** chip_select : Chip selection.        SPI: CS pins, I2C: I2C address */
/** reg : Sensor registers. */
/** value : Write value.*/
/** size : data length */
uint8_t mcube_write_regs(uint8_t reg,uint8_t *value, uint8_t size)
{
  for (uint8_t i = 0; i < size; i++) {
    writeRegister8( reg + i, value[i]);
  }
  return 0;
}

// Read 8-bit from register
uint8_t readRegister8(uint8_t reg)
{
  uint8_t value;
  ChipSelectLow();
  //Send the register address
  SpiSend8bit(reg | 0x80);
  //Delay_Us(3);
 // value = SpiRecive8bit();

  SpiSend8bit(0x00);
 // Delay_Us(3);
 // value = SpiRecive8bit();
  ClearSpiRxFlag();
  ClearSpiTxFlag();
  //Read the value from the register
  Delay_Us(3);
  value = SpiRecive8bit();

  SpiSend8bit(0x00);
  Delay_Us(3);
  value = SpiRecive8bit();
  //Raise CS
  ChipSelectHigh();
  return value;
}

// Write 8-bit to register
void writeRegister8( uint8_t reg, uint8_t value)
{

    //Set active-low CS low to start the SPI cycle
    ChipSelectLow();
    //Send the register address
    SpiSend8bit(reg);
    Delay_Us(3);
    //Send value to write into register
    SpiSend8bit(value);
    Delay_Us(3);
    //Raise CS
    ChipSelectHigh();

  return;
}
// ***MC34X9 dirver motion part***
//void M_DRV_MC34X6_SetTFThreshold(uint16_t threshold) {
//  uint8_t _bFTThr[2] = {0};
//
//  _bFTThr[0] = (threshold & 0x00ff);
//  _bFTThr[1] = ((threshold & 0x7f00) >> 8 );
//
//
//  // set threshold
//  writeRegister8(MC34X9_REG_TF_THRESH_LSB, _bFTThr[0]);
//  writeRegister8(MC34X9_REG_TF_THRESH_MSB, _bFTThr[1]);
//}
//
//void M_DRV_MC34X6_SetTFDebounce(uint8_t debounce) {
//  // set debounce
// writeRegister8(MC34X9_REG_TF_DB, debounce);
//}
//
//void M_DRV_MC34X6_SetANYMThreshold(uint16_t threshold) {
//  uint8_t _bANYMThr[2] = {0};
//
//  _bANYMThr[0] = (threshold & 0x00ff);
//  _bANYMThr[1] = ((threshold & 0x7f00) >> 8 );
//
//  // set threshold
//  writeRegister8(MC34X9_REG_AM_THRESH_LSB, _bANYMThr[0]);
//  writeRegister8(MC34X9_REG_AM_THRESH_MSB, _bANYMThr[1]);
//}
//
//void M_DRV_MC34X6_SetANYMDebounce(uint8_t debounce) {
//  writeRegister8(MC34X9_REG_AM_DB, debounce);
//}
//
//void M_DRV_MC34X6_SetShakeThreshold(uint16_t threshold) {
//  uint8_t _bSHKThr[2] = {0};
//
//  _bSHKThr[0] = (threshold & 0x00ff);
//  _bSHKThr[1] = ((threshold & 0xff00) >> 8 );
//
//  // set threshold
//  writeRegister8(MC34X9_REG_SHK_THRESH_LSB, _bSHKThr[0]);
//  writeRegister8(MC34X9_REG_SHK_THRESH_MSB, _bSHKThr[1]);
//}
//
//void M_DRV_MC34X6_SetShake_P2P_DUR_THRESH(uint16_t threshold, uint8_t shakeCount) {
//
//  uint8_t _bSHKP2PDuration[2] = {0};
//
//  _bSHKP2PDuration[0] = (threshold & 0x00ff);
//  _bSHKP2PDuration[1] = ((threshold & 0x0f00) >> 8);
//  _bSHKP2PDuration[1] |= ((shakeCount & 0x7) << 4);
//
//  // set peak to peak duration and count
//  writeRegister8(MC34X9_REG_PK_P2P_DUR_THRESH_LSB, _bSHKP2PDuration[0]);
//  writeRegister8(MC34X9_REG_PK_P2P_DUR_THRESH_MSB, _bSHKP2PDuration[1]);
//}
//
//void M_DRV_MC34X6_SetTILT35Threshold(uint16_t threshold) {
//  M_DRV_MC34X6_SetTFThreshold(threshold);
//}
//
//void M_DRV_MC34X6_SetTILT35Timer(uint8_t timer) {
//  uint8_t value;
//
//  value = readRegister8(MC34X9_REG_TIMER_CTRL);
//  value &= 0b11111000;
//  value |= MC34X9_TILT35_2p0;
//
//  writeRegister8(MC34X9_REG_TIMER_CTRL, timer);
//}
//
//// Tilt & Flip
//void _M_DRV_MC34X6_SetTilt_Flip() {
//  // set threshold
//  M_DRV_MC34X6_SetTFThreshold(s_bCfgFTThr);
//  // set debounce
//  M_DRV_MC34X6_SetTFDebounce(s_bCfgFTDebounce);
//  return;
//}
//
//// AnyMotion
//void _M_DRV_MC34X6_SetAnym() {
//  // set threshold
//  M_DRV_MC34X6_SetANYMThreshold(s_bCfgANYMThr);
//
//  // set debounce
//  M_DRV_MC34X6_SetANYMDebounce(s_bCfgANYMDebounce);
//  return;
//}
//
//// Shake
//void _M_DRV_MC34X6_SetShake() {
//  // Config anymotion
//  _M_DRV_MC34X6_SetAnym();
//
//  // Config shake
//  // set threshold
//  M_DRV_MC34X6_SetShakeThreshold(s_bCfgShakeThr);
//
//  // set peak to peak duration and count
//  M_DRV_MC34X6_SetShake_P2P_DUR_THRESH(s_bCfgShakeP2PDuration, s_bCfgShakeCount);
//  return;
//}
//
//// Tilt 35
//void _M_DRV_MC34X6_SetTilt35() {
//  // Config anymotion
//  _M_DRV_MC34X6_SetAnym();
//
//  // Config Tilt35
//  // set threshold
//  M_DRV_MC34X6_SetTILT35Threshold(s_bCfgTILT35Thr);
//
//  //set timer
//  M_DRV_MC34X6_SetTILT35Timer(MC34X9_TILT35_2p0);
//  return;
//}
