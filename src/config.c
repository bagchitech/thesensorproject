#include "config.h"

/*Parse the contents of sensor file line*/
static int parse_sensor_line(char *line, Sensor *out, const char *path, int line_number){
        char *p = line;
        // Stage 1: skip the "sensor" keyword
        p+=6;
        while (*p == ' ' || *p == '\t') p++;
        // Stage 2: parse the ID
        char *end;
        uint8_t id = (uint8_t)strtol(p, &end, 10); 
        p = end;                                   
        while (*p == ' ' || *p == '\t') p++;          
        // Stage 3: parse the name
        char name[30];
        if (*p == '"') {
            p++;                              
            char *close = strchr(p, '"');    
            if (!close) {
                fprintf(stderr, "%s:%d: unterminated quoted name\n", path, line_number);
                return -1;
            }
            size_t len = close - p;           // number of chars between the quotes
            if (len >= sizeof(name)) len = sizeof(name) - 1; //stopping overflow
            memcpy(name, p, len);         
            name[len] = '\0';                
            p = close + 1;          
        }

        else {
            char *word_end = p;
            while (*word_end != '\0' && *word_end != ' ' && *word_end != '\t') {
                word_end++;
            }
            size_t len = word_end - p;
            if (len >= sizeof(name)) len = sizeof(name) - 1;
            memcpy(name, p, len);
            name[len] = '\0';
            p = word_end;
        }
        while (*p == ' ' || *p == '\t') p++;
        // Stage 4: parse the type, create the sensor
        char type_word[20];
        char *word_end = p;
        while (*word_end != '\0' && *word_end != ' ' && *word_end != '\t') {
            word_end++;
        }
        size_t type_len = word_end - p;
        if (type_len >= sizeof(type_word)) type_len = sizeof(type_word) - 1;
        memcpy(type_word, p, type_len);
        type_word[type_len] = '\0';
        p = word_end;
        while (*p == ' ' || *p == '\t') p++;
        //Find the history capacity
        size_t history_capacity = 0;
        char *complete;
        history_capacity = (size_t)strtol(p, &complete, 10); 
        p = complete;                                   
        while (*p == ' ' || *p == '\t') p++;          

        if(strcmp(type_word,"TEMPERATURE")==0){
            *out = init_temperature_sensor((uint8_t)id, name, 0, 0,history_capacity);
        }
        else if(strcmp(type_word,"HUMIDITY")==0){
            *out = init_humidity_sensor((uint8_t)id, name, 1.0f, history_capacity);
        }
        else if(strcmp(type_word,"PRESSURE")==0){
           *out = init_pressure_sensor((uint8_t)id, name, 0.0f,history_capacity);
        }
        else{
            fprintf(stderr, "%s:%d: unknown sensor type '%s'\n",
            path, line_number, type_word);
            return -1;
        }
        // Stage 5: loop over key=value pairs
        while (*p != '\0' && *p != '\n') {
            while (*p == ' ' || *p == '\t') p++;     // skip whitespace
            if (*p == '\0' || *p == '\n') break;     // nothing left, done

            // --- Read the key (up to the '=') ---
            char *eq = strchr(p, '=');
            if (!eq) {
                fprintf(stderr, "%s:%d: expected key=value, got '%s'\n", path, line_number, p);
                return -1;
            }
            char key[20];
            size_t key_len = eq - p;
            if (key_len >= sizeof(key)) key_len = sizeof(key) - 1;
            memcpy(key, p, key_len);
            key[key_len] = '\0';

            // --- Read the value (after '=' up to whitespace) ---
            p = eq + 1;                              // skip the '='
            char *val_end = p;
            while (*val_end != '\0' && *val_end != '\n' && *val_end != ' ' && *val_end != '\t') {
                val_end++;
            }
            char value[30];
            size_t val_len = val_end - p;
            if (val_len >= sizeof(value)) val_len = sizeof(value) - 1;
            memcpy(value, p, val_len);
            value[val_len] = '\0';
            p = val_end;

            // --- Store the value based on the key ---
            if (strcmp(key, "min") == 0) {
                out->dataConfig.temperature.min_range = (short)atoi(value);
            } else if (strcmp(key, "max") == 0) {
                out->dataConfig.temperature.max_range = (short)atoi(value);
            } else if (strcmp(key, "calibration") == 0) {
                out->dataConfig.humidity.calibration = (float)atof(value);
            } else if (strcmp(key, "altitude") == 0) {
                out->dataConfig.pressure.altitude = (float)atof(value);
            } else if (strcmp(key, "fault") == 0) {
                if      (strcmp(value, "NORMAL") == 0)   out->faultmode = NORMAL;
                else if (strcmp(value, "STUCK") == 0)    out->faultmode = STUCK;
                else if (strcmp(value, "DRIFTING") == 0) out->faultmode = DRIFTING;
                else if (strcmp(value, "NOISY") == 0)    out->faultmode = NOISY;
                else if (strcmp(value, "DEAD") == 0)     out->faultmode = DEAD;
                else {
                    fprintf(stderr, "%s:%d: unknown fault '%s'\n", path, line_number, value);
                    return -1;
                }
            } else {
                fprintf(stderr, "%s:%d: unknown key '%s'\n", path, line_number, key);
                return -1;
            }
        }

return 0;
}

/* Load the contents of the file */
Sensor *load_sensor_config(const char *path, size_t *out_count){
    if (!path || !out_count) return NULL;
    *out_count = 0;

    FILE *fp = fopen(path, "r");
    if (!fp) {
        fprintf(stderr, "Error: cannot open '%s'\n", path);
        return NULL;
    }

    size_t sensor_count = 0;
    char line[MAX_LINE_LENGTH];
    while(fgets(line, sizeof(line),fp)!=NULL){
        char *p = line;
        while (*p == ' ' || *p == '\t') p++;   // skip leading whitespace
        if (*p == '\0' || *p == '\n' || *p == '#') continue;   // blank or comment
        if (strncmp(p, "sensor", 6) == 0) sensor_count++;
    }
    fclose(fp);

    if (sensor_count == 0) {
        fprintf(stderr, "Warning: no sensors found in '%s'\n", path);
        return NULL;
    }

    Sensor *sensors = malloc(sensor_count * sizeof(Sensor));

    if (!sensors) {
        fprintf(stderr, "Error: out of memory\n");
        return NULL;
    }

    fp = fopen(path, "r");
    if (!fp) {
        free(sensors);
        return NULL;
    }

    size_t index = 0;
    int line_number = 0;

    while (fgets(line, sizeof(line), fp)!=NULL) {
        line_number++;

        char *p = line;
        while (*p == ' ' || *p == '\t') p++;
        if (*p == '\0' || *p == '\n' || *p == '#') continue;

        if (strncmp(p, "sensor", 6) != 0) {
            fprintf(stderr, "%s:%d: expected 'sensor' keyword\n", path, line_number);
            continue;
           // free(sensors);
            //fclose(fp);
            //return NULL;
        }

        if (parse_sensor_line(p, &sensors[index], path, line_number) != 0) {
            fprintf(stderr, "%s:%d: warning: could not parse, skipping\n",
                path, line_number);
                continue; 
        }
        index++;
    }
    fclose(fp);                   
    *out_count = sensor_count;    
    return sensors;                
    }


    void print_help_usage(void){
        printf("\n HELP for command usage \n");
        printf("\n --config <path of the config file>; Default is sensors.config\n");
        printf("\n --duration <time in seconds>; Default is run until Ctrl+c \n");
        printf("\n --tick-rate <time in ms>; Default is 1000 \n");
        printf("\n --quiet ; Stops the printing of messages\n");
        printf("\n --history-capacity <value of the number of readings>; Default is 100\n");
    }

