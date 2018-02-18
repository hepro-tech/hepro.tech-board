#ifndef PROXIMITY_HH
#define PROXIMITY_HH

class Proximity_Sensor {
    private:
        int pin;
        static const int MAX_DIST = 550;
        static const int MIN_DIST = 100;
    public:
    Proximity_Sensor(int pin);

    float Read();

    int GetDistance(float dist);

};

#endif
