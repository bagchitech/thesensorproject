#include "config.h"

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

    Sensors *sensors = malloc(sensor_count * sizeof(Sensor));

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
            free(sensors);
            fclose(fp);
            return NULL;
        }

        if (parse_sensor_line(p, &sensors[index], path, line_number) != 0) {
            free(sensors);
            fclose(fp);
            return NULL;
        }
    }

}