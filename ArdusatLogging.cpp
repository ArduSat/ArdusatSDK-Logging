/**
 * @file   ArdusatLogging.cpp
 * @Author Ben Peters (ben@ardusat.com)
 * @date   December 3, 2014
 * @brief  Implements functions necessary for logging results to an SD card instead of stdout.
 */

#include <stdio.h>
#include <string.h>
#include "ArdusatLogging.h"

RTC_DS1307 RTC;

SdVolume vol;
SdFat sd;
File file;
const char sd_card_error[] PROGMEM = "Not enough RAM (free: ";
const char csv_header_fmt[] PROGMEM = "time: %lu at %lu\n";

/**
 * Helper function to log null-terminated output buffer string to file.
 *
 * @param output_buf pointer to output buffer to log.
 *
 * @return number of bytes written
 */
int logString(const char *output_buf)
{
  int buf_len = strlen(output_buf);
  return logBytes((const unsigned char *)output_buf, buf_len);
}

/**
 * Log the byte array pointed to by buffer to the SD card. Note that there is
 * absolutely no safety checking on numBytes, so use with care.
 *
 * Since we use a shared buffer between the SD card library and many of the output
 * functions, performs a check first to see if we need to allocate a new buffer.
 *
 * @param buffer byte array to log
 * @param number of bytes to log 
 *
 * @return number of bytes written
 */
int logBytes(const unsigned char *buffer, unsigned char numBytes)
{
  unsigned char *buf;
  int written;
  bool buf_allocated = false;

  if (numBytes > OUTPUT_BUF_SIZE - 1) {
    numBytes = OUTPUT_BUF_SIZE - 1;
  }

  if (file.isOpen()) {
    if (buffer == (unsigned char *) _getOutBuf()) {
      buf = (unsigned char *) malloc(numBytes);
      buf_allocated = true;
      memcpy(buf, buffer, numBytes);
    } else {
      buf = (unsigned char *) buffer;
    }

    written = file.write(buf, numBytes);
    file.sync();
    if (buf_allocated)
      free(buf);
    return written;
  } else {
    return 0;
  }
}

/**
 * Logs a line of CSV formatted acceleration data to the SD card.
 *
 * @param sensorName of this sensor
 * @param data acceleration_t data to log 
 *
 * @return number of bytes written
 */
int logAcceleration(const char *sensorName, acceleration_t & data)
{
  return logString(accelerationToCSV(sensorName, data));
}

/**
 * Logs a line of CSV formatted magnetic data to the SD card.
 *
 * @param sensorName of this sensor
 * @param data magnetic_t data to log
 *
 * @return number of bytes written
 */
int logMagnetic(const char *sensorName, magnetic_t & data)
{
  return logString(magneticToCSV(sensorName, data));
}

/**
 * Logs a line of CSV formatted orientation data to the SD card.
 *
 * @param sensorName of this sensor
 * @param data gyro_t data to log
 *
 * @return number of bytes written
 */
int logGyro(const char *sensorName, gyro_t & data)
{
  return logString(gyroToCSV(sensorName, data));
}

/**
 * Logs a line of CSV formatted tempoerature data to the SD card.
 *
 * @param sensorName of this sensor
 * @param data temperature_t data to log
 *
 * @return number of bytes written
 */
int logTemperature(const char *sensorName, temperature_t & data)
{
  return logString(temperatureToCSV(sensorName, data));
}

/**
 * Logs a line of CSV formatted luminosity data to the SD card.
 *
 * @param sensorName of this sensor
 * @param data luminosity_t data to log
 *
 * @return number of bytes written
 */
int logLuminosity(const char *sensorName, luminosity_t & data)
{
  return logString(luminosityToCSV(sensorName, data));
}

/**
 * Logs a line of CSV formatted UV light data to the SD card.
 *
 * @param sensorName of this sensor
 * @param data uvlight_t data to log
 *
 * @return number of bytes written
 */
int logUVLight(const char *sensorName, uvlight_t & data)
{
  return logString(uvlightToCSV(sensorName, data));
}

/**
 * Logs a line of CSV formatted orientation data to SD card.
 *
 * @param sensorName of this sensor
 * @param data orientation_t data to log
 *
 * @return number of bytes written
 */
int logOrientation(const char *sensorName, orientation_t & data)
{
  return logString(orientationToCSV(sensorName, data));
}

/**
 * Logs a line of CSV formatted pressure data to SD card.
 *
 * @param sensorName of this sensor
 * @param data pressure_t data to log
 *
 * @return number of bytes written
 */
int logPressure(const char *sensorName, pressure_t & data)
{
  return logString(pressureToCSV(sensorName, data));
}

#define init_data_struct(type_def, type_enum) \
  type_def bin_data; \
  bin_data.type = type_enum; \
  bin_data.id = sensorId; \
  bin_data.timestamp = data.header.timestamp;

int binaryLogAcceleration(const unsigned char sensorId, acceleration_t & data)
{
  init_data_struct(acceleration_bin_t, ARDUSAT_SENSOR_TYPE_ACCELERATION)
  bin_data.x = data.x;
  bin_data.y = data.y;
  bin_data.z = data.z;

  return logBytes((unsigned char *) &bin_data, sizeof(acceleration_bin_t));
}

int binaryLogMagnetic(const unsigned char sensorId, magnetic_t & data)
{
  init_data_struct(magnetic_bin_t, ARDUSAT_SENSOR_TYPE_MAGNETIC)
  bin_data.x = data.x;
  bin_data.y = data.y;
  bin_data.z = data.z;

  return logBytes((unsigned char *) &bin_data, sizeof(magnetic_bin_t));
}

int binaryLogGyro(const unsigned char sensorId, gyro_t & data)
{
  init_data_struct(gyro_bin_t, ARDUSAT_SENSOR_TYPE_GYRO)
  bin_data.x = data.x;
  bin_data.y = data.y;
  bin_data.z = data.z;

  return logBytes((unsigned char *) &bin_data, sizeof(gyro_bin_t));
}

int binaryLogTemperature(const unsigned char sensorId, temperature_t & data)
{
  init_data_struct(temperature_bin_t, ARDUSAT_SENSOR_TYPE_TEMPERATURE)
  bin_data.temp = data.t;

  return logBytes((unsigned char *) &bin_data, sizeof(temperature_bin_t));
}

int binaryLogLuminosity(const unsigned char sensorId, luminosity_t & data)
{
  init_data_struct(luminosity_bin_t, ARDUSAT_SENSOR_TYPE_LUMINOSITY)
  bin_data.luminosity = data.lux;

  return logBytes((unsigned char *) &bin_data, sizeof(luminosity_bin_t));
}

int binaryLogUVLight(const unsigned char sensorId, uvlight_t & data)
{
  init_data_struct(uv_light_bin_t, ARDUSAT_SENSOR_TYPE_UV)
  bin_data.uv = data.uvindex;

  return logBytes((unsigned char *) &bin_data, sizeof(uv_light_bin_t));
}

int binaryLogOrientation(const unsigned char sensorId, orientation_t & data)
{
  init_data_struct(orientation_bin_t, ARDUSAT_SENSOR_TYPE_ORIENTATION)
  bin_data.roll = data.roll;
  bin_data.pitch = data.pitch;
  bin_data.heading = data.heading;

  return logBytes((unsigned char *) &bin_data, sizeof(orientation_bin_t));
}

int binaryLogPressure(const unsigned char sensorId, pressure_t & data)
{
  init_data_struct(pressure_bin_t, ARDUSAT_SENSOR_TYPE_PRESSURE)
  bin_data.pressure = data.pressure;

  return logBytes((unsigned char *) &bin_data, sizeof(pressure_bin_t));
}


/*
 * Helper function to log to the top of the CSV header with the current time
 */
int _log_csv_time_header(DateTime & now, unsigned long curr_millis)
{
  char fmt_buf[32];
  _resetOutBuf();

  strcpy_P(fmt_buf, csv_header_fmt);

  sprintf(_getOutBuf(), fmt_buf, now.unixtime(), curr_millis);
  return logString(_getOutBuf());
}

int _log_binary_time_header(DateTime & now, unsigned long curr_millis)
{
  unsigned char buf[10];
  unsigned long unixtime = now.unixtime();

  buf[0] = 0xFF;
  buf[1] = 0xFF;
  memcpy(buf + 2, &unixtime, 4);
  memcpy(buf + 6, &curr_millis, 4);
  return logBytes(buf, 10);
}

/**
 * Function starts the SD card service and makes sure that the appropriate directory
 * structure exists, then creates the file to use for logging data. Data will be logged
 * to a file called fileNamePrefix_0.csv or fileNamePrefix_0.bin (number will be incremented
 * for each new log file)
 *
 * @param chipSelectPin Arduino pin SD card reader CS pin is attached to
 * @param fileNamePrefix string to prefix at beginning of data file
 * @param csvData boolean flag whether we are writing csv data or binary data (used for filename)
 *
 * @return true if successful, false if failed
 */
bool beginDataLog(int chipSelectPin, const char *fileNamePrefix, bool csvData)
{
  bool ret;
  int i = 0;
  int max_len;
  char fileName[19];
  char prefix[8];
  char rootPath[] = "/data";


  if (_output_buffer != NULL) {
    delete []_output_buffer;
  }
  OUTPUT_BUF_SIZE = 512;
  _output_buffer = vol.cacheAddress()->output_buf;

  if (freeMemory() < 400) {
    strcpy_P(_getOutBuf(), sd_card_error);
    Serial.print(_getOutBuf());
    Serial.print(freeMemory());
    Serial.println(", need 400)");
    ret = false;
  } else {
    ret = sd.begin(chipSelectPin, SPI_FULL_SPEED);
  }

  //Filenames need to fit the 8.3 filename convention, so truncate down the
  //given filename if it is too long.
  memcpy(prefix, fileNamePrefix, 7);
  prefix[7] = '\0';

  if (ret) {
    if (!sd.exists(rootPath))
      ret = sd.mkdir(rootPath);
    if (ret) {
      while (true) {
	if (i < 10) {
	  max_len = 7;
	} else if (i < 100) {
	  max_len = 6;
	} else if (i < 1000) {
	  max_len = 5;
	} else {
	  break;
	}

	prefix[max_len - 1] = '\0';
	sprintf(fileName, "%s/%s%d.%s", rootPath, prefix, i,
		csvData ? "csv" : "bin");
	if (!sd.exists(fileName)) {
	  file = sd.open(fileName, FILE_WRITE);
	  break;
	}
	i++;
      }
    }
  }
  return file.isOpen();
}

/*
 * @brief Helper function to get current time from RTC & Arduino.
 *
 * @return 0 on success, -1 on failure
 */
int _getCurrentTime( DateTime *now, unsigned long *curr_millis )
{
  // Try to get the current time from the RTC, if available. This will be prepended to the log file
  // to be used to convert relative timestamps to real time values.
  Wire.begin();
  RTC.begin();
  if (RTC.isrunning()) {
    *now = RTC.now();
    *curr_millis = millis();
    return 0;
  }
  return -1;
}

/**
 * @brief Logs the current RTC timestamp and internal millis clock, if both are available
 *
 * @return number of bytes written
 */
int logRTCTimestamp()
{
  unsigned long curr_millis;
  DateTime now;

  if (_getCurrentTime( &now, &curr_millis ) == 0) {
    return _log_csv_time_header(now, curr_millis);
  }
  return 0;
}

/**
 * @brief Logs the current RTC timestamp and internal millis clock, if both are available
 *
 * @return number of bytes written
 */
int binaryLogRTCTimestamp()
{
  unsigned long curr_millis;
  DateTime now;

  if (_getCurrentTime( &now, &curr_millis ) == 0) {
    return _log_binary_time_header(now, curr_millis);
  }
  return 0;
}

bool setRTC()
{
  Wire.begin();
  RTC.begin();
  // Sets RTC to the date & time sketch was compiled
  RTC.adjust(DateTime(__DATE__, __TIME__));
  return true;
}
