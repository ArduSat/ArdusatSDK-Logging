/*
 * =====================================================================================
 *
 *       Filename:  ArdusatLogging.h
 *
 *    Description:  Implements functions necessary for logging results to an SD
 *                  card instead of serially printing.
 *
 *                  This SDK uses many third-party libraries available from
 *                  Adafruit (https://github.com/adafruit). These libraries are
 *                  mostly under an Apache License, Version 2.0.
 *
 *                  http://www.apache.org/licenses/LICENSE-2.0
 *
 *   Organization:  Ardusat
 *        Version:  2.0
 *       Compiler:  Arduino
 *
 *        Created:  12/3/2014
 *         Author:  Ben Peters (ben@ardusat.com)
 *   Last Updated:  8/25/2015
 *   Last Updater:  Sam Olds (sam@ardusat.com)
 *
 * =====================================================================================
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

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Write functions take care of persisting data to an SD card
 *
 * Data can be written in any format, including a custom binary format for
 * space efficiency, by using the writeBytes function that takes a byte array.
 *
 * The individual helpers write functions automate the process of saving data
 * for each individual value, but write in CSV format, which is easier to consume
 * after the experiment, but takes up more space.
 *
 * Finally, binaryWrite functions write optimized binary representations of the
 * data. This helps save SD card space, but means that the data must be decoded
 * after download before it can be used.
 */
int writeString(const char *output_buf);
int writeBytes(const unsigned char *buffer, unsigned char numBytes);
int writeAcceleration(const char *sensorName, acceleration_t & data);
int writeMagnetic(const char *sensorName, magnetic_t & data);
int writeGyro(const char *sensorName, gyro_t & data);
int writeTemperature(const char *sensorName, temperature_t & data);
int writeLuminosity(const char *sensorName, luminosity_t & data);
int writeUVLight(const char *sensorName, uvlight_t & data);
int writeOrientation(const char *sensorName, orientation_t & data);
int writePressure(const char *sensorName, pressure_t & data);

int binaryWriteAcceleration(const unsigned char sensorId, acceleration_t & data);
int binaryWriteMagnetic(const unsigned char sensorId, magnetic_t & data);
int binaryWriteGyro(const unsigned char sensorId, gyro_t & data);
int binaryWriteTemperature(const unsigned char sensorId, temperature_t & data);
int binaryWriteLuminosity(const unsigned char sensorId, luminosity_t & data);
int binaryWriteUVLight(const unsigned char sensorId, uvlight_t & data);
int binaryWriteOrientation(const unsigned char sensorId, orientation_t & data);
int binaryWritePressure(const unsigned char sensorId, pressure_t & data);

bool beginDataLog(int chipSelectPin, const char *fileNamePrefix, bool csvData);

/**
 * Setup and use the RTC chip, if found
 *
 * The clock must be set before it can be used. This is achieved by compiling
 * and running a basic script (examples/set_rtc). After this, the library
 * automatically uses the RTC if available to timestamp.
 */
bool setRTC();

#ifdef __cplusplus
} // extern "C"
#endif

#endif /* ARDUSATLOGGING_H_ */
