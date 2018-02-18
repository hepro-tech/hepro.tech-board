#ifndef MOTION_HH
#define MOTION_HH

#define MOTION_SENS_STARTUP_DELAY 60000
class Motion_Sensor {
    private:
        int pin;
    public:
    Motion_Sensor(int pin);

    int ReadDigital();

};

#endif
