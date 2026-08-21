#include<stdlib.h>
#include<time.h>
#include<string.h>
#include"logger.h"

Logger *logger_open(const char *path){
    Logger *l = malloc(sizeof(Logger));
    if(!l) return NULL;
   
    l->file = fopen(path, "a");
    if(!l->file) {
         free(l);
         return NULL;
    }
    l->header = false;
    l->rows = 0;

    fseek(l->file, 0, SEEK_END);
    if (ftell(l->file) == 0) {
        fprintf(l->file, "TIMESTAMP,SENSOR_ID,SENSOR_NAME,SENSOR_TYPE,READING,STATUS,FAULT\n");
        fflush(l->file);
    }
    l->header = true;
    return l;
}

static double reading_of(const Sensor *s) {
    switch (s->sensorType) {
        case Temperature: return s->dataConfig.temperature.reading;
        case Humidity:    return s->dataConfig.humidity.reading;
        case Pressure:    return s->dataConfig.pressure.reading;
    }
    return 0.0;
}

static void write_csv_field(FILE *f, const char *text) {

    if (strchr(text, ',') || strchr(text, '"')) {
        fputc('"', f);
        for (const char *c = text; *c; c++) {
            if (*c == '"') fputc('"', f);  
            fputc(*c, f);
        }
        fputc('"', f);
    } else {
        fputs(text, f);   
    }
}

void logger_write(Logger *l, const Sensor *s) {
    if (!l || !l->file || !s) return;

    fprintf(l->file, "%lld,", (long long)time(NULL));

    fprintf(l->file, "%u,", s->id);

    write_csv_field(l->file, s->name);
    fputc(',', l->file);

    fputs(sensor_type_name(s->sensorType), l->file);
    fputc(',', l->file);

    fprintf(l->file, "%.2f,", reading_of(s));


    fputs(status_name(s->status), l->file);
    fputc('\n', l->file);

    l->rows++;

    fflush(l->file);
}


void logger_close(Logger *l) {
    if (!l) return;
    if (l->file) {
        fclose(l->file);
    }
    free(l);
}