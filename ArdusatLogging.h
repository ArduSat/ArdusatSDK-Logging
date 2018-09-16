/**
 * @file   ArdusatLogging.h
 * @Author Ben Peters (ben@ardusat.com)
 * @date   December 3, 2014
 * @brief  Implements functions necessary for logging results to an SD card instead of stdout.
 */

#ifndef ARDUSATLOGGING_H_
#define ARDUSATLOGGING_H_

#include <Arduino.h>
#include <utility/SdFat.h>
#include <utility/SdVolume.h>
#include <utility/MemoryFree.h>
#include <utility/BinaryDataFmt.h>
#include <utility/RTClib.h>

#include "ArdusatSDK.h"

/**
 * Log functions take care of persisting data to an SD card
 *
 * Data can be written in any format, including a custom binary format for
 * space efficiency, by using the logBytes function that takes a byte array.
 *
 * The individual helpers log functions automate the process of saving data
 * for each individual value, but log in CSV format, which is easier to consume
 * after the experiment, but takes up more space.
 *
 * Finally, binaryLog functions log optimized binary representations of the
 * data. This helps save SD card space, but means that the data must be decoded
 * after download before it can be used.
 */
int logString(const char *output_buf);
int logBytes(const unsigned char *buffer, unsigned char numBytes);
int logSensor(const char *sensorName, Acceleration & sensor);
int logSensor(const char *sensorName, Gyro & sensor);
int logSensor(const char *sensorName, Luminosity & sensor);
int logSensor(const char *sensorName, Magnetic & sensor);
int logSensor(const char *sensorName, Orientation & sensor);
int logSensor(const char *sensorName, Pressure & sensor);
int logSensor(const char *sensorName, RGBLight & sensor);
int logSensor(const char *sensorName, Temperature & sensor);
int logSensor(const char *sensorName, UVLight & sensor);

int binaryLogSensor(const unsigned char sensorId, Acceleration & sensor);
int binaryLogSensor(const unsigned char sensorId, Gyro & sensor);
int binaryLogSensor(const unsigned char sensorId, Luminosity & sensor);
int binaryLogSensor(const unsigned char sensorId, Magnetic & sensor);
int binaryLogSensor(const unsigned char sensorId, Orientation & sensor);
int binaryLogSensor(const unsigned char sensorId, Pressure & sensor);
int binaryLogSensor(const unsigned char sensorId, RGBLight & sensor);
int binaryLogSensor(const unsigned char sensorId, Temperature & sensor);
int binaryLogSensor(const unsigned char sensorId, UVLight & sensor);
bool beginDataLog(int chipSelectPin, const char *fileNamePrefix, bool csvData);


/**
 * Setup and use the RTC chip, if found
 *
 * The clock must be set before it can be used. This is achieved by compiling
 * and running a basic script (examples/set_rtc). After this, the library
 * automatically uses the RTC if available to timestamp.
 */
bool setRTC();
int logRTCTimestamp();
int binaryLogRTCTimestamp();

#endif /* ARDUSATLOGGING_H_ */
