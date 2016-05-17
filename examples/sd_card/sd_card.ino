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

Acceleration accel;
Gyro gyro;
Luminosity luminosity;
Magnetic mag;
Temperature am_temp;
//Temperature ir_temp = Temperature(SENSORID_MLX90614);
//UVLight uv_light;

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

  accel.begin();
  gyro.begin();
  luminosity.begin();
  mag.begin();
  am_temp.begin();
  //ir_temp.begin();
  //uv_light.begin();
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
  accel.read();
  gyro.read();
  luminosity.read();
  mag.read();
  am_temp.read();
  //ir_temp.read();
  //uv_light.read();

  if (LOG_CSV_DATA) {
    logSensor("accel", accel);
    logSensor("gyro", gyro);
    logSensor("lux", luminosity);
    logSensor("mag", mag);
    logSensor("temp", am_temp);
    //logSensor("ir", ir_temp);
    //logSensor("uv", uv_light);
  } else {
    binaryLogSensor(1, accel);
    binaryLogSensor(3, gyro);
    binaryLogSensor(6, luminosity);
    binaryLogSensor(2, mag);
    binaryLogSensor(4, am_temp);
    //binaryLogSensor(5, ir_temp);
    //binaryLogSensor(7, uv_light);
  }

  delay(READ_INTERVAL);
}
