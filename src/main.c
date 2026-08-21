#include "sensors.h"
#include<signal.h>
#include<windows.h>
#include "config.h"
#include "logger.h"

#define SENSOR_COUNT 21

static volatile sig_atomic_t running = 1;

static void handle_sigint(int sig) {
    (void)sig;
    running = 0;
}

int main(int argc, char **argv){
    const char *config_path = "sensors.conf";
    int duration = 0;          // 0 means "run forever"
    int tick_rate_ms = 1000;
    bool quiet = false;
    const char *log_path = NULL;
    for(int i=0; i<argc; i++){
        if(strcmp(argv[i],"--config")==0){
            if(i+1>=argc){
                printf("Please enter valid config path!\n");
                return 0;
            }
            ++i;
            config_path = argv[i];
        }
        else if(strcmp(argv[i],"--duration")==0){
            if(i+1>=argc){
                printf("Please enter valid duration!\n");
                return 0;
            }
            ++i;
            duration = atoi(argv[i]);
        }
        else if(strcmp(argv[i],"--tick-rate")==0){
            if(i+1>=argc){
                printf("Please enter valid tick-rate!\n");
                return 0;
            }
            ++i;
            tick_rate_ms = atoi(argv[i]);
        }
        else if(strcmp(argv[i],"--quiet")==0){
            quiet = true;
        }
        else if(strcmp(argv[i],"--help")==0){
            print_help_usage();
            return 0;
        }
        else if(strcmp(argv[i],"--log")==0){
            if(i+1>=argc){
                printf("Please enter valid log path!\n");
                return 0;
            }
            ++i;
            log_path = argv[i];
        }
        else{
            printf("Please provide valid commands!");
        }
    }
    size_t count = 0;
    Sensor *sensors = load_sensor_config(config_path, &count);
    if(!sensors) return 1;
    
    srand((unsigned)time(NULL));
    signal(SIGINT, handle_sigint);

    time_t start = time(NULL);
    Logger *logger = log_path ? logger_open(log_path) : NULL;

    while(running){
        for(size_t i=0; i<count; i++){
            tick_sensor(&sensors[i]);
            if(logger) logger_write(logger, &sensors[i]);
        }

        if(!quiet){
            print_all_sensors(sensors, count);
        }

        if (duration > 0 && (time(NULL) - start) >= duration) {
            break;
        }

        Sleep(tick_rate_ms);

    }
    // Sensor sensors[5];
    // sensors[0] = init_temperature_sensor(1, "Living Room", -10, 50);
    // sensors[1] = init_temperature_sensor(2, "Basement", -10, 50);
    // sensors[2] = init_temperature_sensor(3, "Attic", -10, 50);
    // sensors[3] = init_temperature_sensor(4, "Freezer", -30, 5);
    // sensors[4] = init_temperature_sensor(5, "Outside", -20, 40);

    // // Assign one fault mode per sensor
    // set_sensor_fault(&sensors[0], NORMAL);
    // set_sensor_fault(&sensors[1], STUCK);
    // set_sensor_fault(&sensors[2], DRIFTING);
    // set_sensor_fault(&sensors[3], NOISY);
    // set_sensor_fault(&sensors[4], DEAD);
    // size_t count = ARRAY_LENGTH(sensors);

    // print_all_sensors(sensors, count);
    //     FILE *fptr;
    // fptr = fopen(path, "r");
    // char line[100];

    // while(fgets(line, sizeof(line),fptr)!=NULL){
    //     if (line[0] == '#') continue;              // it's a comment
    //     if (line[0] == '\n') continue;             // it's blank
    //     if (line[0] == '\0') continue;             // also blank
    //     if(strncmp(line, "sensor",6)==0){
    //         char *p = line;
    //         p +=6; //to skip sensor
    //         while (*p == ' ') p++;
    //         char *end;
    //         long id = strtol(p, &end, 10);
    //         p = end;
    // char name[30];
    // if (*p == '"') {
    //     p++;
    //     char *end_quote = strchr(p, '"');
    //     size_t len = end_quote - p;
    //     memcpy(name, p, len);
    //     name[len] = '\0';
    //     p = end_quote + 1;
    // }
    //         printf("%s", line); // Prints each line
    //     }
     
    // }

    // fclose(fptr);

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
    if(logger) logger_close(logger);
    free(sensors);
    return 0;
}