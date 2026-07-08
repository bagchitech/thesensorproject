#pragma once

#ifndef SENSORS_H
#define SENSORS_H

#include<stdio.h>
#include<stdint.h>

#define ARRAY_LENGTH(x) (sizeof(x) / sizeof((x)[0]))
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
Sensor init_temperature_sensor( uint8_t id, const char *name, short min_range, short max_range);


/*Initialization function for humidity sensor - Constructor*/
Sensor init_humidity_sensor(uint8_t id, const char *name, float calibration);

/*Initialization function for pressure sensor - Constructor*/
Sensor init_pressure_sensor(uint8_t id, const char *name, float altitude);

/*Print all sensor values*/
void print_all_sensors(Sensor *sensors, size_t count);

#endif //SENSORS_H