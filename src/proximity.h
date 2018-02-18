#ifndef PROXIMITY_HH
#define PROXIMITY_HH

#define PROX_SENS_STARTUP_DELAY 60000
class Proximity_Sensor {
    private:
        int pin;
    public:
    Proximity_Sensor(int pin);

    int ReadDigital();

};

#endif
