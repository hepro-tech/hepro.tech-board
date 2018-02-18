#include "cellular_hal.h"
#include "proximity.h"
#include "stdio.h"
#include "application.h"

// Set the TELUS APN
STARTUP(cellular_credentials_set("isp.telus.com", "", "", NULL));

//Globals
Proximity_Sensor *prox;
volatile bool spaghetTouched = false;
void  BEAM_ISR() {
    spaghetTouched = true;
}

void setup() {
    // Set the keep-alive value for TELUS SIM card

    //Delay for 1 minute to allow proximity sensor to come online
    Particle.publish("status","Initializing",60,PRIVATE);
    Particle.keepAlive(30);
    prox = new Proximity_Sensor(B3);

    Serial.begin(9600);
    delay(PROX_SENS_STARTUP_DELAY);

     if (attachInterrupt(B3, BEAM_ISR, RISING)) {
        Serial.println ("Interrupt attached to B3.");
    } else {
        Serial.println ("WARNING: couldn't attach interrupt to B3.");
    }
    Particle.publish("status","Start sensing",60,PRIVATE);
}

void loop () {
    if(spaghetTouched)
    {
      Particle.publish("MotionTracking", "SOMEBODY TOUCHA MY SPAGHET", 60, PRIVATE);
      spaghetTouched = false;
    }
    delay(1000);

}
