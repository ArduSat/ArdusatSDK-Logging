/**
 * @file   BinaryDataFmt.h
 * @Author Ben Peters (ben@ardusat.com)
 * @date   December 19, 2014
 * @brief  Definitions for binary data format types. These definitions are used
 *         to encode/decode binary data.
 */

#ifndef BINARY_DATA_FMT_H_
#define BINARY_DATA_FMT_H_

/**
 * Binary data structures are designed to be as space-efficient as possible, 
 * given the sensor's native resolution.
 */
typedef enum {
  ARDUSAT_SENSOR_TYPE_ACCELERATION = 0,
  ARDUSAT_SENSOR_TYPE_MAGNETIC,
  ARDUSAT_SENSOR_TYPE_GYRO,
  ARDUSAT_SENSOR_TYPE_ORIENTATION,
  ARDUSAT_SENSOR_TYPE_TEMPERATURE,
  ARDUSAT_SENSOR_TYPE_LUMINOSITY,
  ARDUSAT_SENSOR_TYPE_UV,
  ARDUSAT_SENSOR_TYPE_PRESSURE,
} ardusat_sensor_types_e;

#define _bin_data_header uint8_t type; uint8_t id; uint32_t timestamp;

typedef struct {
	_bin_data_header
	float x;
	float y;
	float z;
} acceleration_bin_t;

typedef struct {
	_bin_data_header
	float x;
	float y;
	float z;
} magnetic_bin_t;

typedef struct {
	_bin_data_header
	float x;
	float y;
	float z;
} gyro_bin_t;

typedef struct {
	_bin_data_header
	float temp;
} temperature_bin_t;

typedef struct {
	_bin_data_header
	float luminosity;
} luminosity_bin_t;

typedef struct {
	_bin_data_header
	float uv;
} uv_light_bin_t;

typedef struct {
	_bin_data_header
	float roll;
	float pitch;
	float heading;
} orientation_bin_t;

typedef struct {
  _bin_data_header
  float pressure;
} pressure_bin_t;
#endif
