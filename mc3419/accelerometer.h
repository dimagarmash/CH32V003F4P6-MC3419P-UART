/*
 * accelerometer.h
 *
 *  Created on: Feb 1, 2025
 *      Author: Dmitry
 */

#ifndef MC3419_ACCELEROMETER_H_
#define MC3419_ACCELEROMETER_H_
#define COUNT_MEASUREMENT 20
#define MEASURMENT_DELAY 1
#include "stdbool.h"
void setup(void);
//void sensorFIFO(void);
void checkRange(void);
//void sensorMotion(void);
void checkSamplingRate(void);
//struct MC34X9_Overload readAndOutput(void);
struct MC34X9_Overload readMeanOverload(void);
//bool interruptChecker(void);
//void interruptLoop(void);
//void loop(void);

#endif /* MC3419_ACCELEROMETER_H_ */
