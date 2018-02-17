#ifndef PROXIMITY_HH
#define PROXIMITY_HH


class Proximity_Sensor {
    private:
        int pin;
    public:
    Proximity_Sensor(int pin);

    int ReadDigital();

};

#endif
