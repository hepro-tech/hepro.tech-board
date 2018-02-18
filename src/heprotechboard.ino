#include "cellular_hal.h"
#include "motion.h"
#include "proximity.h"
#include "stdio.h"
#include "application.h"

// Set the TELUS APN
STARTUP(cellular_credentials_set("isp.telus.com", "", "", NULL));

//Globals
Motion_Sensor *motion;
Proximity_Sensor *prox;

Thread * proximity_thread;

volatile bool spaghetTouched = false;
bool measureProximity = true;
int centimetres = 550;

void  BEAM_ISR() {
    spaghetTouched = true;
}

os_thread_return_t MeasureProximity(void * param)
{
    
    while(true)
    {
      if(measureProximity)
      {
          centimetres = prox->GetDistance(prox->Read());
      }
      delay(50);
    }
}

void setup() {
    // Set the keep-alive value for TELUS SIM card

    //Delay for 1 minute to allow motion sensor to come online
    Particle.publish("status","Initializing",60,PRIVATE);
    Particle.keepAlive(30);
    motion = new Motion_Sensor(B3);
    prox = new Proximity_Sensor(B4);

    Serial.begin(9600);
    delay(MOTION_SENS_STARTUP_DELAY);

     if (attachInterrupt(B3, BEAM_ISR, RISING)) {
        Serial.println ("Interrupt attached to B3.");
    } else {
        Serial.println ("WARNING: couldn't attach interrupt to B3.");
    }
    Particle.publish("status","Start sensing",60,PRIVATE);

    proximity_thread = new Thread("proximity", MeasureProximity, 0);
    
}

void loop () {
    if(spaghetTouched)
    {
      Particle.publish("MotionTracking", "SOMEBODY TOUCHA MY SPAGHET", 60, PRIVATE);
      spaghetTouched = false;
    }
    char publishMessage[30] = {0};
    snprintf(publishMessage, 30, "%d", centimetres);
    Particle.publish("MotionTracking", publishMessage, 60, PRIVATE);
    delay(500);
}
