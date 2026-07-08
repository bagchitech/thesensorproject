#include "sensors.h"

#define SENSOR_COUNT 21

int main(){
    Sensor sensor = {.id = 1,
    .name = "Test1",
    .dataConfig={.humidity = {
        .calibration = 0.01,
        .reading = 87.2,
    },
    },
    .sensorType = Humidity,
    .status = ACTIVE,

    };

    Sensor sensors[SENSOR_COUNT];

    uint8_t sensor_type=0;
    char sensor_name[] = "Sensor";
    short min_range = -10;
    short max_range = 50;
    float calibration = 0.01f;
    float alt = 150.0f;

for (uint8_t i = 0; i < SENSOR_COUNT / 3; i++) {
    sensors[3*i]     = init_temperature_sensor(3*i, sensor_name, min_range, max_range);
    sensors[3*i + 1] = init_humidity_sensor(3*i + 1, sensor_name, calibration);
    sensors[3*i + 2] = init_pressure_sensor(3*i + 2, sensor_name, alt);

    min_range += 1;
    max_range += 1;
    calibration += 0.01f;
    alt += 10.0f;
}

    size_t count = ARRAY_LENGTH(sensors);

    print_all_sensors(sensors, count);

    printf("Printing some values to confirm flow");
    printf("The sensor type is %d\n", sensor.sensorType);
    printf("The id is %d\n",sensor.id);


    return 0;
}