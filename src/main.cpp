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
void turnRadiansRight(float radians);
void turnRadiansLeft(float radians);

int main()
{
    int distanceInMm[2] = {1194, 872};
    float turnRadians = (M_PI / 2);
    output();
    ThisThread::sleep_for(3s);
    driveStraightDist(distanceInMm[0]);
    turnRadiansRight(turnRadians);
    driveStraightDist(distanceInMm[1]);
    turnRadiansRight(turnRadians);
    ThisThread::sleep_for(10s);//reset for right hand test

    driveStraightDist(distanceInMm[0]);
    turnRadiansLeft(turnRadians);
    driveStraightDist(distanceInMm[1]);
    turnRadiansLeft(turnRadians);

}

void driveStraightDist(int dist)
{
    int wheelDiameterMm = 66;
    float rotations = (dist / (wheelDiameterMm * M_PI));
    int polaritySwaps = (rotations * 192);
    printf("Pole swaps: \t%d\n", polaritySwaps);
    while ((hall_sensor.get_countA() + hall_sensor.get_countB()) / 2 < polaritySwaps)
    {
        robo.drive(.45);
        ThisThread::sleep_for(10);
        output();
    }
    robo.stop();
    ThisThread::sleep_for(10s);
    hall_sensor.resetAll();
}
void turnRadiansLeft(float radians)
{
    int wheelDiameterMm = 66;
    float arcLength = radians * (182 / 2);
    printf("Arc length: \t %d\n", static_cast<int>(arcLength));
    float rotations = (arcLength / (wheelDiameterMm * M_PI));
    int polaritySwaps = (rotations * 192);
    printf("Pole swaps for turn: \t%d\n", polaritySwaps);
    while (hall_sensor.get_countA() < polaritySwaps)
    {
        robo.leftFWD(.45);
        ThisThread::sleep_for(10);
        output();
    }
    robo.stop();
    ThisThread::sleep_for(10s);
    hall_sensor.resetAll();
}

void turnRadiansRight(float radians)
{
    int wheelDiameterMm = 66;
    float arcLength = radians * (182 / 2);
    printf("Arc length: \t %d\n", static_cast<int>(arcLength));
    float rotations = (arcLength / (wheelDiameterMm * M_PI));
    int polaritySwaps = (rotations * 192);
    printf("Pole swaps for turn: \t%d\n", polaritySwaps);
    while (hall_sensor.get_countB() < polaritySwaps)
    {
        robo.rightFWD(.45);
        ThisThread::sleep_for(10);
        output();
    }
    robo.stop();
    ThisThread::sleep_for(10s);
    hall_sensor.resetAll();
}

