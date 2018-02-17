#include "proximity.h"
#include "Particle.h"
Proximity_Sensor::Proximity_Sensor(int pin)
{
    this->pin = pin;
    pinMode(pin, INPUT);
}

int Proximity_Sensor::ReadDigital()
{
    return digitalRead(this->pin);
}
