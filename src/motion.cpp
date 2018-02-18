#include "motion.h"
#include "Particle.h"
Motion_Sensor::Motion_Sensor(int pin)
{
    this->pin = pin;
    pinMode(pin, INPUT);
}

int Motion_Sensor::ReadDigital()
{
    return digitalRead(this->pin);
}
