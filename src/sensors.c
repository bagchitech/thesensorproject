#include "sensors.h"
#include<string.h>

/*Initialization function for temperature sensor - Constructor*/
Sensor init_temperature_sensor(const uint8_t id, const char *name, const short min_range, const int max_range){
    
    Sensor sensor = {0};
    sensor->id = id;
    sensor->sensorType = Temperature;
    sensor->status = ACTIVE;
    sensor->dataConfig.temperature.min_range = min_range;
    sensor->dataConfig.temperature.max_range = max_range;
    sensor->dataConfig.temperature.reading = 0.0f;
    strncpy(sensor->name, name, 29);
    sensor->name[30] = '\0';

    return sensor;
}


/*Initialization function for humidity sensor - Constructor*/
Sensor init_humidity_sensor(const uint8_t id, const char *name, const float calibration){
    
    Sensor sensor = {0};
    sensor->id = id;
    sensor->sensorType = Humidity;
    sensor->status = ACTIVE;
    sensor->dataConfig.humidity.calibration = calibration;
    sensor->dataConfig.humidity.reading = 0.0f;
    strncpy(sensor->name, name, 29);
    sensor->name[30] = '\0';

    return sensor;
}

/*Initialization function for pressure sensor - Constructor*/
Sensor init_pressure_sensor(const uint8_t id, const char *name, const float altitude){
    sensor->id = id;
    sensor->sensorType = Pressure;
    sensor->status = ACTIVE;
    sensor->dataConfig.pressure.altitude = altitude;
    sensor->dataConfig.pressure.reading = 0.0f;
    strncpy(sensor->name, name, 29);
    sensor->name[30] = '\0';

    return sensor;
}