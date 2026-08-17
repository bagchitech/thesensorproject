#ifndef LOGGER_H_
#define LOGGER_H_
#include<stdio.h>
#include"sensors.h"

typedef struct{
    FILE *file;
    size_t rows;
}Logger;

Logger *logger_open(const char *path);
void logger_write(Logger *l, const Sensor *s);
void logger_close(Logger *l);

#endif //LOGGER_H_