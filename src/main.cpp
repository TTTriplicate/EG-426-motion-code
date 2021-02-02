#define _USE_MATH_DEFINES

#include <mbed.h>
#include <math.h>
#include "Motion.h"
#include "Encoders.h"

Encoders hall_sensor;
BufferedSerial pc(USBTX, USBRX);
Motion robo;

void output()
{
    printf("hall sensor reads\n Left : %d \t Right : %d\n", hall_sensor.get_countA(), hall_sensor.get_countB());
}

void driveStraightDist(int dist);
void turnDegreesLeft(float radians);

int main()
{
    int distanceInMm = 1000;
    float turnRadians = (M_PI/2);
    while (1)
    {
        driveStraightDist(distanceInMm);
        ThisThread::sleep_for(10s);
        hall_sensor.resetAll();
        turnDegreesLeft(turnRadians);
        ThisThread::sleep_for(10s);
        hall_sensor.resetAll();
    }
    //output();
}

void driveStraightDist(int dist)
{
    int wheelDiameterMm = 66;
    float rotations = (dist / (wheelDiameterMm * M_PI));
    int polaritySwaps = (rotations * 192);
    printf("Pole swaps: \t%d\n", polaritySwaps);
    while ((hall_sensor.get_countA() + hall_sensor.get_countB()) / 2 < polaritySwaps)
    {
        robo.drive(.4);
        ThisThread::sleep_for(100);
        output();
    }
    robo.stop();
}
void turnDegreesLeft(float radians)
{
    int wheelDiameterMm = 66;
    float arcLength = radians * (182/2);
    printf("Arc length: \t %d\n", static_cast<int>(arcLength));
    float rotations = (arcLength / (wheelDiameterMm * M_PI));
    int polaritySwaps = (rotations * 192);
    printf("Pole swaps for turn: \t%d\n", polaritySwaps);
    while(hall_sensor.get_countA() < polaritySwaps){
        robo.leftFWD(.4);
        ThisThread::sleep_for(100);
        output();
    }
    robo.stop();
}
