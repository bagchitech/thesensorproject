#pragma once

#ifndef STATEMACHINE_H
#define STATEMACHINE_H

#include<stdbool.h>
#include<stdint.h>
#include"sensors.h"

#define ERROR_THRESHOLD 5 
#define RECOVERY_THRESHOLD 3
#define STALE_THRESHOLD 10

/*checks type-specific bounds*/
bool is_reading_valid(const Sensor *s);

/*Apply the transition rules*/
void update_sensor_state(Sensor *s);

#endif //STATEMACHINE_H