#pragma once

#ifndef SENSORS_H
#define SENSORS_H

#include<stdio.h>
#include<stdint.h>
typedef enum {
    ACTIVE,
    PASSIVE,
    ERROR
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
#endif //SENSORS_H