#include "sensors.h"
#include<string.h>

/*Initialization function for temperature sensor - Constructor*/
Sensor init_temperature_sensor(uint8_t id, const char *name, short min_range, short max_range){
    
    Sensor sensor = {0};
    sensor.id = id;
    sensor.sensorType = Temperature;
    sensor.status = ACTIVE;
    sensor.dataConfig.temperature.min_range = min_range;
    sensor.dataConfig.temperature.max_range = max_range;
    sensor.dataConfig.temperature.reading = 0.0f;
    strncpy(sensor.name, name, sizeof(sensor.name) - 1);
    sensor.name[sizeof(sensor.name) - 1] = '\0';

    return sensor;
}


/*Initialization function for humidity sensor - Constructor*/
Sensor init_humidity_sensor(uint8_t id, const char *name, float calibration){
    
    Sensor sensor = {0};
    sensor.id = id;
    sensor.sensorType = Humidity;
    sensor.status = ACTIVE;
    sensor.dataConfig.humidity.calibration = calibration;
    sensor.dataConfig.humidity.reading = 0.0f;
    strncpy(sensor.name, name, sizeof(sensor.name) - 1);
    sensor.name[sizeof(sensor.name) - 1] = '\0';

    return sensor;
}

/*Initialization function for pressure sensor - Constructor*/
Sensor init_pressure_sensor(uint8_t id, const char *name, float altitude){
    Sensor sensor = {0};
    sensor.id = id;
    sensor.sensorType = Pressure;
    sensor.status = ACTIVE;
    sensor.dataConfig.pressure.altitude = altitude;
    sensor.dataConfig.pressure.reading = 0.0f;
    strncpy(sensor.name, name, sizeof(sensor.name) - 1);
    sensor.name[sizeof(sensor.name) - 1] = '\0';

    return sensor;
}

/*Print all sensor values*/
void print_all_sensors(Sensor *sensors, size_t count){
    for(size_t i =0; i < count; i++){
        printf("ID: %d Name: %s SensorType: %d Status: %d \n",sensors[i].id, sensors[i].name, sensors[i].sensorType, sensors[i].status);
    }
}