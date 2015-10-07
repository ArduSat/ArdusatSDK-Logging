/*
 * =====================================================================================
 *
 *       Filename:  sd_card.ino
 *
 *    Description:  Example saving sensor data to a SD card.
 *
 *                  This utility reads data from all the sensors in the space kit,
 *                  then logs it to the SD card in either CSV or binary format (see
 *                  define flags).
 *
 *                  Data is stored in the /DATA directory on the FAT32 formated SD
 *                  card. The filename of a datalog is /DATA/PREFIX0.CSV for CSV
 *                  data or /DATA/PREFIX0.BIN for binary data. See the docs for more
 *                  information about binary formats. Note that prefix will be
 *                  truncated to 7 character or less if a longer filename is given.
 *
 *                  This example uses many third-party libraries available from
 *                  Adafruit (https://github.com/adafruit). These libraries are
 *                  mostly under an Apache License, Version 2.0.
 *
 *                  http://www.apache.org/licenses/LICENSE-2.0
 *
 *        Version:  1.0
 *        Created:  12/21/2014
 *       Revision:  none
 *       Compiler:  Arduino
 *
 *         Author:  Ben Peters (ben@ardusat.com)
 *   Organization:  Ardusat
 *         Edited:  8/25/2015
 *      Edited By:  Sam Olds (sam@ardusat.com)
 *
 * =====================================================================================
 */

/*-----------------------------------------------------------------------------
 *  Includes
 *-----------------------------------------------------------------------------*/
#include <Arduino.h>
#include <Wire.h>
#include <ArdusatSDK.h>
#include <ArdusatLogging.h>

/*-----------------------------------------------------------------------------
 *  Constant Definitions
 *-----------------------------------------------------------------------------*/
const short READ_INTERVAL = 10000; // interval, in ms, to wait between readings

// CS pin used for SD card reader. Reader should be wired to DIO 10, 11, 12, 13
const short SD_CS_PIN = 10;

static char LOG_FILE_PREFIX[] = "MYLOG";
static bool LOG_CSV_DATA = true; // otherwise, binary data is logged

temperature_t temp;
luminosity_t luminosity;
uvlight_t uv_light;
acceleration_t accel;
magnetic_t mag;
gyro_t orientation;

/*
 * ===  FUNCTION  ======================================================================
 *         Name:  setup
 *  Description:  This function runs when the Arduino first turns on/resets. This is
 *                our chance to take care of all one-time configuration tasks to get
 *                the program ready to begin logging data.
 * =====================================================================================
 */
void setup()
{
  if (!beginDataLog(SD_CS_PIN, LOG_FILE_PREFIX, LOG_CSV_DATA)) {
    while (true);
  }

  beginAccelerationSensor();
  beginTemperatureSensor();
  beginInfraredTemperatureSensor();
  beginLuminositySensor();
  beginUVLightSensor();
  beginGyroSensor();
  beginMagneticSensor();
}

/*
 * ===  FUNCTION  ======================================================================
 *         Name:  loop
 *  Description:  After setup runs, this loop function runs until the Arduino loses
 *                power or resets. We go through and update each of the attached
 *                sensors, log the values to the SD card, then delay before
 *                repeating the loop again.
 * =====================================================================================
 */
void loop()
{
  readAcceleration(accel);
  readTemperature(temp);
  readMagnetic(mag);
  readGyro(orientation);
  readLuminosity(luminosity);
  readUVLight(uv_light);

  if (LOG_CSV_DATA) {
    logAcceleration("accel", accel);
    logMagnetic("mag", mag);
    logGyro("gyro", orientation);
    logTemperature("temp", temp);

    readInfraredTemperature(temp);
    logTemperature("ir", temp);

    logLuminosity("lux", luminosity);
    logUVLight("uv", uv_light);
  } else {
    binaryLogAcceleration(0, accel);
    binaryLogMagnetic(1, mag);
    binaryLogGyro(2, orientation);
    binaryLogTemperature(3, temp);
    readInfraredTemperature(temp);
    binaryLogTemperature(4, temp);
    binaryLogLuminosity(5, luminosity);
    binaryLogUVLight(6, uv_light);
  }

  delay(READ_INTERVAL);
}
