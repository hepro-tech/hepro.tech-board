#include "cellular_hal.h"
#include "proximity.h"
#include "stdio.h"
// Set the TELUS APN
  STARTUP(cellular_credentials_set("isp.telus.com", "", "", NULL));
  Proximity_Sensor *prox;
void setup() {
  // Set the keep-alive value for TELUS SIM card
  Particle.keepAlive(30);
  prox = new Proximity_Sensor(B3);
}
void loop () {
  char digi[30] = {0 };
  snprintf(digi, 30, "%d", prox->ReadDigital());
  Particle.publish("beamStatus",digi,60,PRIVATE);
  delay(1000);
}
