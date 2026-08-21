#pragma once

#ifndef SENSOR_H
#define SENSOR_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "sensors.h"

#define MAX_LINE_LENGTH 256

Sensor *load_sensor_config(const char *path, size_t *out_count);
void print_help_usage(void);

#endif //SENSOR_H