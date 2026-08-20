#include "statemachine.h"


/*checks type-specific bounds*/
bool is_reading_valid(const Sensor *s){
    bool validity = true;
    switch(s->sensorType){
        case Temperature:
           if(s->dataConfig.temperature.reading < s->dataConfig.temperature.min_range){
            validity = false;
        }
        else if (s->dataConfig.temperature.reading > s->dataConfig.temperature.max_range){
           validity = false;
        }
        break;
        case Humidity:
         if( s->dataConfig.humidity.reading < 0.0f){
            validity = false;
        } else if( s->dataConfig.humidity.reading > 100.0f){
            validity = false;
        }
        break;
        case Pressure:
        if( s->dataConfig.pressure.reading < 900.0f){
            validity = false;
        }
        else  if(s->dataConfig.pressure.reading > 1100.0f){
            validity = false;
        }
        break;
    }
    return validity;
}

/*Apply the transition rules*/
void update_sensor_state(Sensor *s){
    if (s->status == PASSIVE) return;
    if(s->faultmode == STUCK){
            if ((s->status == ACTIVE) && ((time(NULL) - s->last_update) > STALE_THRESHOLD)) {
        s->status = FAULTY;
        s->consecutive_good_reads = 0;
        printf("Sensor %u (%s) → FAULTY (stale)\n", s->id, s->name);
    }

    }
    else if ((s->status == ACTIVE) && (s->consecutive_bad_reads >=5)) s->status = FAULTY;
    else if ((s->status == FAULTY) && (s->consecutive_good_reads >=3)) s->status = ACTIVE;
}