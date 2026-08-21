#ifndef LOGGER_H_
#define LOGGER_H_
#include<stdio.h>
#include"sensors.h"
#include<stdbool.h>

typedef struct{
    FILE *file;
    bool header;
    size_t rows;
}Logger;

Logger *logger_open(const char *path);
static double reading_of(const Sensor *s);
static void write_csv_field(FILE *f, const char *text);
void logger_write(Logger *l, const Sensor *s);
void logger_close(Logger *l);

#endif //LOGGER_H_