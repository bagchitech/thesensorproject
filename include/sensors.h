#pragma once

#ifndef SENSORS_H
#define SENSORS_H

#include<stdio.h>
#include<stdint.h>
typedef enum {
    ACTIVE,
    PASSIVE,
    FAULTY
}Status;

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
    char name[30];
    DataConfig dataConfig;
    SensorType sensorType;
    Status status;
}Sensor;

/*Initialization function for temperature sensor - Constructor*/
Sensor init_temperature_sensor(const uint8_t id, const char *name, const short min_range, const int max_range);


/*Initialization function for humidity sensor - Constructor*/
Sensor init_humidity_sensor(const uint8_t id, const char *name, const float calibration);

/*Initialization function for pressure sensor - Constructor*/
Sensor init_pressure_sensor(const uint8_t id, const char *name, const float altitude);


#endif //SENSORS_H