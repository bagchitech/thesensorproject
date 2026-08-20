#pragma once

#ifndef SENSORS_H
#define SENSORS_H

#include<stdio.h>
#include<stdint.h>
#include<time.h>
#include<stdbool.h>
#include "ringbuffer.h"

#define ARRAY_LENGTH(x) (sizeof(x) / sizeof((x)[0]))
typedef enum {
    ACTIVE,
    PASSIVE,
    FAULTY
}Status;

typedef enum { 
    NORMAL, 
    STUCK, 
    DRIFTING, 
    NOISY, 
    DEAD } FaultMode;

typedef enum {
    Temperature,
    Humidity,
    Pressure
}SensorType;

typedef union{
    struct{
        short int min_range;
        short int max_range;
        float reading;
    }temperature;

    struct {
        float calibration;
        float reading;
    }humidity;

    struct {
        float altitude;
        float reading;
    }pressure;
}DataConfig;

typedef struct{
    uint8_t id;
    uint8_t consecutive_bad_reads;
    uint8_t consecutive_good_reads;
    char name[30];
    DataConfig dataConfig;
    RingBuffer history;
    SensorType sensorType;
    Status status;
    FaultMode faultmode;
    uint32_t reading_count; //monotonic sequence number, increments each successful update
    time_t last_update; //seconds since epoch of last reading update
}Sensor;

/*Initialization function for temperature sensor - Constructor*/
Sensor init_temperature_sensor( uint8_t id, const char *name, short min_range, short max_range, size_t history_capacity);


/*Initialization function for humidity sensor - Constructor*/
Sensor init_humidity_sensor(uint8_t id, const char *name, float calibration, size_t history_capacity);

/*Initialization function for pressure sensor - Constructor*/
Sensor init_pressure_sensor(uint8_t id, const char *name, float altitude, size_t history_capacity);

/*Print all sensor values*/
void print_all_sensors(Sensor *sensors, size_t count);

/*Generate Value for one sensor*/

void generateSensorValue(Sensor *s);

void tick_sensor(Sensor *s);

/*Set the fault mode of the sensor*/
void set_sensor_fault(Sensor *s, FaultMode f);
const char *fault_name(FaultMode f);

#endif //SENSORS_H