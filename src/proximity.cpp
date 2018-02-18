#include "proximity.h"
#include "Particle.h"
Proximity_Sensor::Proximity_Sensor(int pin)
{
    this->pin = pin;
    pinMode(pin, INPUT);
}

float Proximity_Sensor::Read()
{
    return analogRead(this->pin)/4095.0;
}

int Proximity_Sensor::GetDistance(float dist)
{
    return (int)(dist * (MAX_DIST - MIN_DIST) + MIN_DIST);
}
