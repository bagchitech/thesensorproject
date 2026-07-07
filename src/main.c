#include "sensors.h"

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

    printf("Printing some values to confirm flow");
    printf("The sesnor type is %d\n", sensor.sensorType);
    printf("The id is %d\n",sensor.id);


    return 0;
}