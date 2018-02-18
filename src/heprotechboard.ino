#include "cellular_hal.h"
#include "motion.h"
#include "proximity.h"
#include "stdio.h"
#include "application.h"
#include "severity.h"

// Set the TELUS APN
STARTUP(cellular_credentials_set("isp.telus.com", "", "", NULL));

Severity securityLevel = DISARMED;

//Globals
Motion_Sensor *motion;
Proximity_Sensor *prox;

Thread * proximity_thread;

volatile bool motionDetected = false;
volatile bool measureProximity = false;
volatile int centimetres = 550;

void  BEAM_ISR() {
    motionDetected = true;
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

void ArmMotion(bool arm)
{
  if(arm)
  {
    attachInterrupt(B3, BEAM_ISR, RISING);
  }
  else
  {
    detachInterrupt(B3);
  }
}

void ArmProximity(bool arm)
{
  if(arm)
  {
    proximity_thread = new Thread("proximity", MeasureProximity, 0);
    measureProximity = true;
  }
  else
  {
    delete proximity_thread;
    measureProximity = false;
  }
}

void ToggleArmed(const char *event, const char *data)
{
    if(strcmp(data, "ARM") == 0)
    {
        ArmMotion(true);
        securityLevel = MOTION;
    }
    else
    {
        ArmMotion(false);
        ArmProximity(false);
        securityLevel = DISARMED;
    }
}


void ChangeSeverity(const char * event, const char * data)
{
    if(strcmp(data, "ELEVATE") == 0)
    {
      switch(securityLevel)
      {
        case DISARMED:
          ArmMotion(true);
          securityLevel = MOTION;
          break;
        case MOTION:
          ArmProximity(true);
          securityLevel = PROXIMITY;
          break;
        case PROXIMITY:
          securityLevel = WARNING;
          break;
        case WARNING:
          break;
      }
    } else if (strcmp(data, "REDUCE") == 0) {
      switch(securityLevel)
      {
        case DISARMED:
          break;
        case MOTION:
          ArmMotion(false);
          securityLevel = DISARMED;
          break;
        case PROXIMITY:
          ArmProximity(false);
          securityLevel = MOTION;
          break;
        case WARNING:
          securityLevel = PROXIMITY;
          break;
      }
    }
}

void setup() {
    // Set the keep-alive value for TELUS SIM card

    //Delay for 1 minute to allow motion sensor to come online
    Particle.publish("status","Initializing",60,PRIVATE);
    Particle.keepAlive(30);
    motion = new Motion_Sensor(B3);
    prox = new Proximity_Sensor(B4);
    pinMode(B0, OUTPUT);

    delay(MOTION_SENS_STARTUP_DELAY);
    Particle.publish("status","Start sensing",60,PRIVATE);

    Particle.subscribe("ARM", ToggleArmed);
    Particle.subscribe("SECURITY", ChangeSeverity);
}

void loop () {
    if(motionDetected)
    {
      Particle.publish("MotionTracking", "Motion Detected", 60, PRIVATE);
      motionDetected = false;
    }
    
    if(measureProximity)
    {
        digitalWrite(B0, HIGH);
        char publishMessage[30] = {0};
        snprintf(publishMessage, 30, "%d", centimetres);
        Particle.publish("ProximitySensing", publishMessage, 60, PRIVATE);
    }
    delay(100);
    digitalWrite(B0, LOW);
    delay(100);
}
