#include "sensors.h"
#include<string.h>
#include<stdlib.h>
#include "statemachine.h"

/*Initialization function for temperature sensor - Constructor*/
Sensor init_temperature_sensor(uint8_t id, const char *name, short min_range, short max_range, size_t history_capacity){
    
    Sensor sensor = {0};
    sensor.id = id;
    sensor.sensorType = Temperature;
    sensor.status = ACTIVE;
    sensor.dataConfig.temperature.min_range = min_range;
    sensor.dataConfig.temperature.max_range = max_range;
    sensor.dataConfig.temperature.reading = 0.0f;
    strncpy(sensor.name, name, sizeof(sensor.name) - 1);
    sensor.name[sizeof(sensor.name) - 1] = '\0';
    rb_init(&sensor.history, history_capacity);

    return sensor;
}


/*Initialization function for humidity sensor - Constructor*/
Sensor init_humidity_sensor(uint8_t id, const char *name, float calibration, size_t history_capacity){
    
    Sensor sensor = {0};
    sensor.id = id;
    sensor.sensorType = Humidity;
    sensor.status = ACTIVE;
    sensor.dataConfig.humidity.calibration = calibration;
    sensor.dataConfig.humidity.reading = 0.0f;
    strncpy(sensor.name, name, sizeof(sensor.name) - 1);
    sensor.name[sizeof(sensor.name) - 1] = '\0';
    rb_init(&sensor.history, history_capacity);

    return sensor;
}

/*Initialization function for pressure sensor - Constructor*/
Sensor init_pressure_sensor(uint8_t id, const char *name, float altitude, size_t history_capacity){
    Sensor sensor = {0};
    sensor.id = id;
    sensor.sensorType = Pressure;
    sensor.status = ACTIVE;
    sensor.dataConfig.pressure.altitude = altitude;
    sensor.dataConfig.pressure.reading = 0.0f;
    strncpy(sensor.name, name, sizeof(sensor.name) - 1);
    sensor.name[sizeof(sensor.name) - 1] = '\0';
    rb_init(&sensor.history, history_capacity);

    return sensor;
}

/*Print all sensor values*/
void print_all_sensors(Sensor *sensors, size_t count){
    for(size_t i =0; i < count; i++){
        if(sensors[i].sensorType==0){
        printf("SEQ No: %u ID: %d Name: %s SensorType: %d Status: %d Reading: %f FAULT: %s \n",sensors[i].reading_count, sensors[i].id, sensors[i].name, sensors[i].sensorType, sensors[i].status, sensors[i].dataConfig.temperature.reading, fault_name(sensors[i].faultmode));
        }
        else if(sensors[i].sensorType==1){
        printf("SEQ No: %u ID: %d Name: %s SensorType: %d Status: %d Reading: %f FAULT: %s \n",sensors[i].reading_count, sensors[i].id, sensors[i].name, sensors[i].sensorType, sensors[i].status, sensors[i].dataConfig.humidity.reading, fault_name(sensors[i].faultmode));
        }
        else if(sensors[i].sensorType==2){
        printf("SEQ No: %u ID: %d Name: %s SensorType: %d Status: %d Reading: %f FAULT: %s \n",sensors[i].reading_count, sensors[i].id, sensors[i].name, sensors[i].sensorType, sensors[i].status, sensors[i].dataConfig.pressure.reading, fault_name(sensors[i].faultmode));
    }
    }
}

/*Generate Value for one sensor*/
// void generateSensorValue(Sensor *s){

//     switch(s->sensorType){
//         case Temperature:
//         float deltaValueTemp = (((float)rand() / (float)RAND_MAX) - 0.5f) * 0.04f;
//         s->dataConfig.temperature.reading += deltaValueTemp;
//         s->reading_count++;
//         break;

//         case Humidity:
//         float deltaValueHumidity = (((float)rand() / (float)RAND_MAX) - 0.5f) * 2.0f;
//         s->dataConfig.humidity.reading += deltaValueHumidity * s->dataConfig.humidity.calibration ;
//         s->reading_count++;
//         break;

//         case Pressure:
//         float deltaValuePressure = (((float)rand() / (float)RAND_MAX) - 0.5f) * 0.5f;
//         s->dataConfig.pressure.reading += deltaValuePressure;
//         s->reading_count++;
//         break;
//     }

//     bool validity = is_reading_valid(s);

//     if(validity == true){
//         s->consecutive_good_reads++;
//         s->consecutive_bad_reads = 0;
//     }
//     else{
//         s->consecutive_bad_reads++;
//         s->consecutive_good_reads = 0;
//     }
//     update_sensor_state(s);
// }

void set_sensor_fault(Sensor *s, FaultMode f) {
    if (!s) return;
    s->faultmode = f;
}

const char *fault_name(FaultMode f) {
    switch (f) {
        case NORMAL:   return "NORMAL";
        case STUCK:    return "STUCK";
        case DRIFTING: return "DRIFTING";
        case NOISY:    return "NOISY";
        case DEAD:     return "DEAD";
        default:       return "UNKNOWN";
    }
}

/* Generate a single "normal" reading for the sensor's type. */
static double generate_normal_reading(const Sensor *s) {
    switch (s->sensorType) {
        case Temperature: {
            double current = s->dataConfig.temperature.reading;
            double delta = ((double)rand() / RAND_MAX - 0.5) * 0.4;
            return current + delta;
        }
        case Humidity: {
            double current = s->dataConfig.humidity.reading;
            double delta = ((double)rand() / RAND_MAX - 0.5) * 2.0;
            return (current + delta) * s->dataConfig.humidity.calibration;
        }
        case Pressure: {
            double current = s->dataConfig.pressure.reading;
            if (current == 0.0) current = 1013.0;   // seed first tick
            double delta = ((double)rand() / RAND_MAX - 0.5) * 0.5;
            return current + delta;
        }
    }
    return 0.0;
}

static double get_current_reading(const Sensor *s) {
    switch (s->sensorType) {
        case Temperature: return s->dataConfig.temperature.reading;
        case Humidity:    return s->dataConfig.humidity.reading;
        case Pressure:    return s->dataConfig.pressure.reading;
    }
    return 0.0;
}


static void set_reading(Sensor *s, double value) {
    double old = get_current_reading(s);
    if (value != old) {
        s->last_update = time(NULL); 
    }
    switch (s->sensorType) {
        case Temperature: s->dataConfig.temperature.reading = (float)value; break;
        case Humidity:    s->dataConfig.humidity.reading    = (float)value; break;
        case Pressure:    s->dataConfig.pressure.reading    = (float)value; break;
    }
}

void tick_sensor(Sensor *s) {
    if (!s) return;
    if (s->status == PASSIVE) return;

    double new_reading = 0.0;

    switch (s->faultmode) {
        case NORMAL:
            new_reading = generate_normal_reading(s);
            break;

        case STUCK:
            new_reading = get_current_reading(s);
            break;

        case DRIFTING:
            new_reading = get_current_reading(s) + 0.5;  
            break;

        case NOISY:
            new_reading = ((double)rand() / RAND_MAX) * 2000.0 - 1000.0;
            break;

        case DEAD:
            // No update to reading, sequence, or timestamp. Sensor is silent.
            return;
    }

    set_reading(s, new_reading);
    s->reading_count++;
   // s->last_update = time(NULL);

    bool validity = is_reading_valid(s);

    if(validity == true){
        s->consecutive_good_reads++;
        s->consecutive_bad_reads = 0;
    }
    else{
        s->consecutive_bad_reads++;
        s->consecutive_good_reads = 0;
    }
    update_sensor_state(s);
}