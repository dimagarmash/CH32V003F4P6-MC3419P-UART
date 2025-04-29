/*****************************************************************************

   Copyright (c) 2020 mCube, Inc.  All rights reserved.

   This source is subject to the mCube Software License.
   This software is protected by Copyright and the information and source code
   contained herein is confidential. The software including the source code
   may not be copied and the information contained herein may not be used or
   disclosed except with the written permission of mCube Inc.

   All other rights reserved.
 *****************************************************************************/

/**
   @file    MC34X9_demo.ino
   @author  mCube
   @date    13 January 2020
   @brief   Arduino example code for accelerometer mc34X9 series.
   @see     http://www.mcubemems.com
*/

#include "MC34X9.h"
#include "debug.h"
#include "accelerometer.h"
#include "Prnt.h"


/*** bSpi: I2C/SPI bus selection.  SPI: 0, I2C: 1 ***/
const uint8_t bSpi = 1;

// Chip Select & Address
uint8_t chipSelect = 0;
const uint8_t SPIChipSelectPin = 10; // SPI chipSelectPin
const uint8_t I2CAddress = 0x4c; // I2C address

// Arduino interrupt pin
const int INTERRUPT_PIN = 8;

// *** FIFO control ***/
int FIFO_THRE_SIZE = 30;
// FIFO Interrupt
const uint8_t enableFifoThrINT = false;
// For FIFO feature, enable FIFO interrupt will automatically enable FIFO feature
bool enableFIFO = false;

// *** Motion control ***/
// Enabling motion feature below also enables corresponded motion interrupt
const bool enableTILT = false;
const bool enableFLIP = false;
const bool enableANYM = false;
const bool enableSHAKE = false;
const bool enableTILT_35 = false;

// Determine if enable interrupt
const bool interruptEnabled = enableFifoThrINT || enableTILT || enableFLIP || enableANYM || enableSHAKE || enableTILT_35;



void setup(void)
{
//  print(" Accelerometer MC34X9:\r\n");
  // Define SPI chip select or I2C address
//  print("Define SPI chip select or I2C address\r\n");
  if (!start()) {
      print("invalid chip id \r\n");
    // Invalid chip ID// Block here
    while (true){
        print("Blocked by init mc34x9 oblject\r\n");
    }
    return;
  }

  // Check chip setup
//  print("call checkrange\r\n");
//  checkRange();
//  print("call checkSamplingRate\r\n");
//  checkSamplingRate();
//  print("\r\n");

  //Test read
//  print("call readAndOutput\r\n");
  //readAndOutput();

  // Enable feature & interrput
//  enableFIFO = enableFIFO || enableFifoThrINT;
//  if (enableFIFO) {
//    sensorFIFO();
//  }
//  if (enableTILT || enableFLIP || enableANYM || enableSHAKE || enableTILT_35) {
//    // Checker: These modes can only be enabled separately.
//    int counter = 0;
//    if (enableTILT)
//      counter++;
//    if (enableFLIP)
//      counter++;
//    if (enableANYM)
//      counter++;
//    if (enableSHAKE)
//      counter++;
//    if (enableTILT_35)
//      counter++;
//    if (counter > 1) {
//      // Detected: These modes can only be enabled separately.
//        print("Error: Enable too many motion feature.");
//        print("Error: These modes can only be enabled separately.");
//      // Block here
//      while (true);
//    }
//    sensorMotion();
//  }
}

//void sensorFIFO(void)
//{
//  //Enable FIFO and interrupt
//  stop();
//  SetSampleRate(MC34X9_SR_50Hz);
//  SetFIFOCtrl(MC34X9_FIFO_CTL_ENABLE, MC34X9_FIFO_MODE_WATERMARK, FIFO_THRE_SIZE);
//  SetFIFOINTCtrl(0, 0, enableFifoThrINT); //Enable FIFO threshold interrupt
//  wake();
//
//  print("Sensor FIFO enable.");
//}

//void sensorMotion(void) {
//  //Enable motion feature and motion interrupt
//  stop();
//  SetSampleRate(MC34X9_SR_DEFAULT_2000Hz);
//  SetMotionCtrl(enableTILT, enableFLIP, enableANYM, enableSHAKE, enableTILT_35);
//  SetINTCtrl(enableTILT, enableFLIP, enableANYM, enableSHAKE, enableTILT_35);
//  wake();
//
//  print("Sensor motion enable.");
//}

//void checkRange(void)
//{
//  switch (GetRangeCtrl())
//  {
//    case MC34X9_RANGE_16G:
//        print("Range: +/- 16 g\r\n");
//      break;
//    case MC34X9_RANGE_12G:
//        print("Range: +/- 12 g\r\n");
//      break;
//    case MC34X9_RANGE_8G:
//        print("Range: +/- 8 g\r\n");
//      break;
//    case MC34X9_RANGE_4G:
//        print("Range: +/- 4 g\r\n");
//      break;
//    case MC34X9_RANGE_2G:
//        print("Range: +/- 2 g\r\n");
//      break;
//    default:
//        print("Range: +/- ?? g\r\n");
//      break;
//  }
//}
//
//void checkSamplingRate(void)
//{
//    print("Low Power Mode SR\r\n");
//  switch (GetSampleRate())
//  {
//
//    case MC34X9_SR_50Hz:
//        print("Output Sampling Rate: 50 Hz\r\n");
//      break;
//
//    case MC34X9_SR_100Hz:
//        print("Output Sampling Rate: 100 Hz\r\n");
//      break;
//    case MC34X9_SR_125Hz:
//        print("Output Sampling Rate: 125 Hz\r\n");
//      break;
//    case MC34X9_SR_200Hz:
//        print("Output Sampling Rate: 200 Hz\r\n");
//      break;
//    case MC34X9_SR_250Hz:
//        print("Output Sampling Rate: 250 Hz\r\n");
//      break;
//    case MC34X9_SR_500Hz:
//        print("Output Sampling Rate: 500 Hz\r\n");
//      break;
//    case MC34X9_SR_1000Hz:
//        print("Output Sampling Rate: 1000 Hz\r\n");
//      break;
//    case MC34X9_SR_DEFAULT_2000Hz:
//        print("Output Sampling Rate: 2000 Hz");
//      break;
//    default:
//        print("Output Sampling Rate: ?? Hz\r\n");
//      break;
//  }
//}


// struct MC34X9_Overload readAndOutput(void) {
//  // Read the raw sensor data count
//  struct MC34X9_Overload rawAccel = readRawAccel();
//
//  // Output Count
//  print("|---------------------------------------------------------|\r\n");
////  its been need for debugging
////  printn("X:%hd\t",rawAccel.XAxis); print("\t \r\n");
////  printn("Y:%hd\t",rawAccel.YAxis); print("\t\r\n");
////  printn("Z:%hd\t",rawAccel.ZAxis); print("\t\r\n");
////  print("counts\r\n");
////  print("|---------------------------------------------------------|\r\n");
//
//  // Display the results (acceleration is measured in m/s^2)
//  printn("X: \t");printfl(rawAccel.XAxis_g); print("\t\r\n");
//  printn("Y: \t");printfl(rawAccel.YAxis_g);print("\t\r\n");
//  printn("Z: \t");printfl(rawAccel.ZAxis_g); print("\t\r\n");
//  print("|---------------------------------------------------------|\r\n");
////its been need for debugging
////  printn("X:%hd\t",(short)rawAccel.XAxis_g); print("\t\r\n");
////  printn("Y:%hd\t",(short)rawAccel.YAxis_g);print("\t\r\n");
////  printn("Z:%hd\t",(short)rawAccel.ZAxis_g); print("\t\r\n");
////  print("m/s^2\r\n");
////
////  print("|---------------------------------------------------------|\r\n");
//  return rawAccel;
//}
 struct MC34X9_Overload readMeanOverload(void) {
   struct MC34X9_Overload rawAccel ;
   struct MC34X9_Overload Mean;
   Mean.XAxis_g=0;
   Mean.YAxis_g=0;
   Mean.ZAxis_g=0;
   for ( uint8_t var = 0;   var < COUNT_MEASUREMENT; var++) {
          rawAccel=readRawAccel();
          Mean.XAxis_g+=rawAccel.XAxis_g;
          Mean.YAxis_g+=rawAccel.YAxis_g;
          Mean.ZAxis_g+=rawAccel.ZAxis_g;
          Delay_Ms(MEASURMENT_DELAY);
   }
   Mean.XAxis_g=Mean.XAxis_g/COUNT_MEASUREMENT;
   Mean.YAxis_g=Mean.YAxis_g/COUNT_MEASUREMENT;
   Mean.ZAxis_g=Mean.ZAxis_g/COUNT_MEASUREMENT;

  print("|---------------------------------------------------------1|\r\n");
  // Display the results (acceleration is measured in m/s^2)
  printn("X: \t");printfl(Mean.XAxis_g); print("\t\r\n");
  printn("Y: \t");printfl(Mean.YAxis_g);print("\t\r\n");
  printn("Z: \t");printfl(Mean.ZAxis_g); print("\t\r\n");
  print("|---------------------------------------------------------1|\r\n");

   return rawAccel;
 }

// Interrupt checker: read interrupt register and determine if interrupt happen
//bool interruptChecker(void) {
//  // Init interrupt table
//  bool retCode = false;
//  MC34X9_interrupt_event_t evt_mc34X9 = {0};
//  MC34X9_fifo_interrupt_event_t fifo_evt_mc34X9 = {0};
//
//  // Read interrupt table
//  FIFOINTHandler(&fifo_evt_mc34X9);
//  INTHandler(&evt_mc34X9);
//
//  // Whether there is interrupt
//  uint8_t* iter = (uint8_t*) &evt_mc34X9;
//  for (int i = 0; i < sizeof (MC34X9_interrupt_event_t); i++, iter++) {
//    if ((*iter) & 0x01)
//      retCode = true;
//  }
//  iter = (uint8_t*) &fifo_evt_mc34X9;
//  for (int i = 0; i < sizeof (MC34X9_fifo_interrupt_event_t); i++, iter++) {
//    if ((*iter) & 0x01)
//      retCode = true;
//  }
//
//  if (retCode)
//      print("Get interrupt: ");
//  if (enableFIFO) {
//    if (fifo_evt_mc34X9.bFIFO_EMPTY) {
//        print("FIFO empty. ");
//    }
//    if (fifo_evt_mc34X9.bFIFO_FULL) {
//        print("FIFO full. ");
//    }
//    if (fifo_evt_mc34X9.bFIFO_THRESH) {
//        print("FIFO threshold. ");
//    }
//  }
//  if (evt_mc34X9.bTILT) {
//      print("Tilt. ");
//  }
//  if (evt_mc34X9.bFLIP) {
//      print("Flip. ");
//  }
//  if (evt_mc34X9.bANYM) {
//      print("Any Motion. ");
//  }
//  if (evt_mc34X9.bSHAKE) {
//      print("Shake. ");
//  }
//  if (evt_mc34X9.bTILT_35) {
//      print("Tilt 35. ");
//  }
//  if (retCode)
//      print("\r\n");
//  return retCode;
//}

// Function for enabled interrupt mode
//void interruptLoop(void) {
//  if (interruptChecker()) {
//    // When interrupt happen
//
//    if (enableFIFO) {
//      while (!(IsFIFOEmpty()))
//      {
//        // Read and output all data in FIFO
//        readAndOutput();
//      }
//    } else {
//      readAndOutput();
//    }
//  }
//  return;
//}

// Arduino declare function
//void loop(void)
//{
//  if (interruptEnabled) {
//    interruptLoop();
//    Delay_Us(1);
//  } else {
//    // without enable interrupt
//    // read and output data periodically
//    readAndOutput();
//    Delay_Us(10);
//  }
//  return;
//}
