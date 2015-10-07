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

#ifdef __cplusplus
extern "C" {
#endif

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
int logAcceleration(const char *sensorName, acceleration_t & data);
int logMagnetic(const char *sensorName, magnetic_t & data);
int logGyro(const char *sensorName, gyro_t & data);
int logTemperature(const char *sensorName, temperature_t & data);
int logLuminosity(const char *sensorName, luminosity_t & data);
int logUVLight(const char *sensorName, uvlight_t & data);
int logOrientation(const char *sensorName, orientation_t & data);
int logPressure(const char *sensorName, pressure_t & data);

int binaryLogAcceleration(const unsigned char sensorId, acceleration_t & data);
int binaryLogMagnetic(const unsigned char sensorId, magnetic_t & data);
int binaryLogGyro(const unsigned char sensorId, gyro_t & data);
int binaryLogTemperature(const unsigned char sensorId, temperature_t & data);
int binaryLogLuminosity(const unsigned char sensorId, luminosity_t & data);
int binaryLogUVLight(const unsigned char sensorId, uvlight_t & data);
int binaryLogOrientation(const unsigned char sensorId, orientation_t & data);
int binaryLogPressure(const unsigned char sensorId, pressure_t & data);
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

#ifdef __cplusplus
} // extern "C"
#endif

#endif /* ARDUSATLOGGING_H_ */
