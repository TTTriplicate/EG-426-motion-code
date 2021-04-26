#include <mbed.h>
#include <math.h>
#include "Motion.h"
#include "Encoders.h"
#include "VL53L0X.h"

#define _USE_MATH_DEFINES
#define I2C_SDA D14
#define I2C_SCL D15

I2C i2c(I2C_SDA, I2C_SCL);
Timer timer;

VL53L0X sensor(i2c, timer);

Encoders hall_sensor;
BufferedSerial pc(USBTX, USBRX);
Motion robo;

void output();
void driveStraightDist(int dist);
void turnRadians(float radians, char dir);
void identifyTurn();
void driveToObstacle();
int rangeFinder();
int countToDrive(int dist);
int arcLength(float radians)

int main()
{
    // while (true)
    // {
    //     turnRadians((M_PI / 2), 'l');
    //     ThisThread::sleep_for(3s);
    //     turnRadians((M_PI / 2), 'r');
    //     ThisThread::sleep_for(3s);
    // }
    driveToObstacle();
    identifyTurn();
    driveToObstacle();
    identifyTurn();
    driveStraightDist(500);
}

void output()
{
    printf("hall sensor reads\n Left : %d \t Right : %d\n", hall_sensor.get_countA(), hall_sensor.get_countB());
}

void driveStraightDist(int dist)
{
    int polaritySwaps = countToDrive(dist);
    printf("Pole swaps: \t%d\n", polaritySwaps);
    while ((hall_sensor.get_countA() + hall_sensor.get_countB()) / 2 < polaritySwaps)
    {
        robo.drive(.3);
        ThisThread::sleep_for(10);
    }
    robo.stop();
    ThisThread::sleep_for(500);
    hall_sensor.resetAll();
}

void turnRadians(float radians, char dir)
{

    int arc_length = arcLength(radians);
    int polaritySwaps = countToDrive(arc_length);

    while ((hall_sensor.get_countA() + hall_sensor.get_countB()) / 2 < polaritySwaps)
    {
        if (dir == 'l')
        {
            robo.turnLeft(.3);
        }
        else if (dir == 'r')
        {
            robo.turnRight(.3);
        }
        ThisThread::sleep_for(10);
    }
    robo.stop();
    ThisThread::sleep_for(500);
    hall_sensor.resetAll();
}

void driveToObstacle()
{
    int distance = rangeFinder();
    driveStraightDist(distance - 150);
}

void identifyTurn()
{
    turnRadians(M_PI / 2, 'l');
    int distanceLeft = rangeFinder();
    turnRadians(M_PI, 'r');
    int distanceRight = rangeFinder();
    if (distanceLeft > distanceRight)
    {
        turnRadians(M_PI, 'l');
    }
}

int rangeFinder()
{
    sensor.init(true);
    sensor.setTimeout(500);

    int *dist = new int[3];
    for (int i = 0; i < 3; i++)
    {
        dist[i] = sensor.readRangeSingleMillimeters();
    }
    int distance;
    for (int i = 0; i < 3; i++)
    {
        distance += dist[i];
    }
    distance = distance / 3;

    delete dist;
    return distance;
}

int countToDrive(int dist)
{
    int wheelDiameterMm = 66;
    float rotations = (dist / (wheelDiameterMm * M_PI));
    int polaritySwaps = (rotations * 192);
    return polaritySwaps;
}

int arcLength(float radians)
{
    int dist = radians * (150 / 2);
    return dist;
}