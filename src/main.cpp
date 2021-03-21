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
void turnRadiansRight(float radians);
void turnRadiansLeft(float radians);
void turnRadians(float radians, char dir);
void driveToObstacle();

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
    turnRadians(M_PI / 2, 'l');
    driveToObstacle();
    turnRadians(M_PI / 2, 'r');
    driveToObstacle();
    turnRadians(M_PI, 'l');
    driveToObstacle();
    turnRadians(M_PI/2, 'l');
    driveStraightDist(1000);
    turnRadians(2 * M_PI, 'r');
}

void output()
{
    printf("hall sensor reads\n Left : %d \t Right : %d\n", hall_sensor.get_countA(), hall_sensor.get_countB());
}

void driveStraightDist(int dist)
{
    int wheelDiameterMm = 66;
    float rotations = (dist / (wheelDiameterMm * M_PI));
    int polaritySwaps = (rotations * 192);
    printf("Pole swaps: \t%d\n", polaritySwaps);
    while ((hall_sensor.get_countA() + hall_sensor.get_countB()) / 2 < polaritySwaps)
    {
        robo.leftFWD(.27);
        robo.rightFWD(.25);
        ThisThread::sleep_for(10);
    }
    robo.stop();
    ThisThread::sleep_for(500);
    hall_sensor.resetAll();
}
void turnRadiansRight(float radians)
{
    int wheelDiameterMm = 66;
    float arcLength = radians * (150);
    printf("Arc length: \t %d\n", static_cast<int>(arcLength));
    float rotations = (arcLength / (wheelDiameterMm * M_PI));
    int polaritySwaps = (rotations * 192);
    printf("Pole swaps for turn: \t%d\n", polaritySwaps);
    while (hall_sensor.get_countA() < polaritySwaps)
    {
        robo.leftFWD(.3);
        ThisThread::sleep_for(10);
        output();
    }
    robo.stop();
    ThisThread::sleep_for(500);
    hall_sensor.resetAll();
}

void turnRadiansLeft(float radians)
{
    int wheelDiameterMm = 66;
    float arcLength = radians * (150);
    printf("Arc length: \t %d\n", static_cast<int>(arcLength));
    float rotations = (arcLength / (wheelDiameterMm * M_PI));
    int polaritySwaps = (rotations * 192);
    printf("Pole swaps for turn: \t%d\n", polaritySwaps);
    while (hall_sensor.get_countB() < polaritySwaps)
    {
        robo.rightFWD(.3);
        ThisThread::sleep_for(10);
        output();
    }
    robo.stop();
    ThisThread::sleep_for(500);
    hall_sensor.resetAll();
}

void turnRadians(float radians, char dir)
{
    int wheelDiameterMm = 66;
    float arcLength = radians * (150 / 2);
    float rotations = (arcLength / (wheelDiameterMm * M_PI));
    int polaritySwaps = (rotations * 192);

    while ((hall_sensor.get_countA() + hall_sensor.get_countB()) / 2 < polaritySwaps)
    {
        if (dir == 'l')
        {
            robo.turnLeft(.25);
        }
        else if (dir == 'r')
        {
            robo.turnRight(.25);
        }
        ThisThread::sleep_for(10);
    }
    robo.stop();
    ThisThread::sleep_for(500);
    hall_sensor.resetAll();
}

void driveToObstacle()
{
    sensor.init(true);
    sensor.setTimeout(500);
    int* dist = new int[3];
    
    for (int i = 0; i < 3; i++){
        dist[i] = sensor.readRangeSingleMillimeters();
    }
    int distance;
    for (int i = 0; i < 3; i++){
        distance += dist[i];
    }
    distance = distance/3;
    driveStraightDist(distance - 150);
    delete dist;
}