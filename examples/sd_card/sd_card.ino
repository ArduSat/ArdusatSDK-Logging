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

// Pins used for measurements
const short UVOUT = A0;      // Output from the UV sensor
const short REF_3V3 = A1;    // 3.3V power on the Arduino board
const short SD_CS_PIN = 10;  // CS pin used for SD card reader. Reader should be wired
                           // to DIO 10, 11, 12, 13

static char LOG_FILE_PREFIX[] = "MYLOG";
static bool LOG_CSV_DATA = true;

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
  Serial.begin(9600);

  if (!beginDataLog(SD_CS_PIN, LOG_FILE_PREFIX, LOG_CSV_DATA)) {
    Serial.println("Failed to initialize SD card. Stopping...");
    while (true);
  }

  beginAccelerationSensor();
  beginTemperatureSensor();
  beginInfraredTemperatureSensor();
  beginLuminositySensor();
  beginUVLightSensor();
  beginGyroSensor();
  beginMagneticSensor();
  
  // More pin initializations
  pinMode(UVOUT, INPUT);
  pinMode(REF_3V3, INPUT);
  
  /* We're ready to go! */
  Serial.println("");
}

/*
 * ===  FUNCTION  ======================================================================
 *         Name:  loop
 *  Description:  After setup runs, this loop function runs until the Arduino loses
 *                power or resets. We go through and update each of the attached
 *                sensors, write the values to the SD card, then delay before
 *                repeating the loop again.
 * =====================================================================================
 */
void loop()
{
  int bytesWritten;

  readAcceleration(accel);
  readMagnetic(mag);
  readGyro(orientation);
  readInfraredTemperature(temp);
  readLuminosity(luminosity);
  readUVLight(uv_light);

  if (LOG_CSV_DATA) {
    bytesWritten = writeAcceleration("accelerometer", accel);
    bytesWritten += writeMagnetic("magnetic", mag);
    bytesWritten += writeGyro("gyro", orientation);
    bytesWritten += writeTemperature("temp", temp);
    bytesWritten += writeLuminosity("luminosity", luminosity);
    bytesWritten += writeUVLight("uv", uv_light);
  } else {
    bytesWritten = binaryWriteAcceleration(0, accel);
    bytesWritten += binaryWriteMagnetic(1, mag);
    bytesWritten += binaryWriteGyro(2, orientation);
    bytesWritten += binaryWriteTemperature(3, temp);
    bytesWritten += binaryWriteLuminosity(4, luminosity);
    bytesWritten += binaryWriteUVLight(5, uv_light);
  }

  Serial.print("Wrote ");
  Serial.print(bytesWritten);
  Serial.println(" bytes.");

  Serial.print("Free Memory: ");
  Serial.println(freeMemory());

  delay(READ_INTERVAL);
}
