#include "sensors.h"
#include<string.h>
#include<stdlib.h>

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
        if(sensors[i].sensorType==1){
        printf("ID: %d Name: %s SensorType: %d Status: %d Reading: %f \n",sensors[i].id, sensors[i].name, sensors[i].sensorType, sensors[i].status, sensors[i].dataConfig.temperature.reading);
        }
        else if(sensors[i].sensorType==2){
        printf("ID: %d Name: %s SensorType: %d Status: %d Reading: %f \n",sensors[i].id, sensors[i].name, sensors[i].sensorType, sensors[i].status, sensors[i].dataConfig.humidity.reading);
        }
        else if(sensors[i].sensorType==2){
        printf("ID: %d Name: %s SensorType: %d Status: %d Reading: %f \n",sensors[i].id, sensors[i].name, sensors[i].sensorType, sensors[i].status, sensors[i].dataConfig.pressure.reading);
    }
    }
}

/*Generate Value for one sensor*/
void generateSensorValue(Sensor *s){
    switch(s->sensorType){
        case Temperature:
        float deltaValueTemp = (((float)rand() / (float)RAND_MAX) - 0.5f) * 0.04f;
        s->dataConfig.temperature.reading += deltaValueTemp;

        if(s->dataConfig.temperature.reading < s->dataConfig.temperature.min_range){
            s->dataConfig.temperature.reading = s->dataConfig.temperature.min_range;
        }
        else if (s->dataConfig.temperature.reading > s->dataConfig.temperature.max_range){
            s->dataConfig.temperature.reading = s->dataConfig.temperature.max_range;
        }
        break;

        case Humidity:
        float deltaValueHumidity = (((float)rand() / (float)RAND_MAX) - 0.5f) * 2.0f;
        s->dataConfig.humidity.reading += deltaValueHumidity * s->dataConfig.humidity.calibration ;

        if( s->dataConfig.humidity.reading < 0.0f){
             s->dataConfig.humidity.reading = 0.0f;
        } else if( s->dataConfig.humidity.reading > 100.0f){
             s->dataConfig.humidity.reading = 100.0f;
        }
        break;

        case Pressure:
        float deltaValuePressure = (((float)rand() / (float)RAND_MAX) - 0.5f) * 0.5f;
        s->dataConfig.pressure.reading += deltaValuePressure;

        if( s->dataConfig.pressure.reading < 0.0f){
             s->dataConfig.pressure.reading = 1013.0f;
        }
        break;
    }
}