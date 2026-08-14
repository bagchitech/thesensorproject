#include "sensors.h"
#include<signal.h>
#include<windows.h>
#include "config.h"

#define SENSOR_COUNT 21

char path[] = "";
static volatile sig_atomic_t running = 1;

static void handle_sigint(int sig) {
    (void)sig;
    running = 0;
}

int main(){
    Sensor sensors[5];
    sensors[0] = init_temperature_sensor(1, "Living Room", -10, 50);
    sensors[1] = init_temperature_sensor(2, "Basement", -10, 50);
    sensors[2] = init_temperature_sensor(3, "Attic", -10, 50);
    sensors[3] = init_temperature_sensor(4, "Freezer", -30, 5);
    sensors[4] = init_temperature_sensor(5, "Outside", -20, 40);

    // Assign one fault mode per sensor
    set_sensor_fault(&sensors[0], NORMAL);
    set_sensor_fault(&sensors[1], STUCK);
    set_sensor_fault(&sensors[2], DRIFTING);
    set_sensor_fault(&sensors[3], NOISY);
    set_sensor_fault(&sensors[4], DEAD);
    size_t count = ARRAY_LENGTH(sensors);

    print_all_sensors(sensors, count);
        FILE *fptr;
    fptr = fopen(path, "r");
    char line[100];

    while(fgets(line, sizeof(line),fptr)!=NULL){
        if (line[0] == '#') continue;              // it's a comment
        if (line[0] == '\n') continue;             // it's blank
        if (line[0] == '\0') continue;             // also blank
        if(strncmp(line, "sensor",6)==0){
            char *p = line;
            p +=6; //to skip sensor
            while (*p == ' ') p++;
            char *end;
            long id = strtol(p, &end, 10);
            p = end;
    char name[30];
    if (*p == '"') {
        p++;
        char *end_quote = strchr(p, '"');
        size_t len = end_quote - p;
        memcpy(name, p, len);
        name[len] = '\0';
        p = end_quote + 1;
    }
            printf("%s", line); // Prints each line
        }
     
    }

    fclose(fptr);

//     while (running) {  
//     for (size_t i = 0; i < count; i++) {
//         tick_sensor(&sensors[i]);
//     }
//     print_all_sensors(sensors, count);
//     printf("---\n");
//     Sleep(10000);
// }


    // printf("Printing some values to confirm flow");
    // printf("The sensor type is %d\n", sensor.sensorType);
    // printf("The id is %d\n",sensor.id);


    return 0;
}