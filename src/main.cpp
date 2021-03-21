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

void output()
{
    printf("hall sensor reads\n Left : %d \t Right : %d\n", hall_sensor.get_countA(), hall_sensor.get_countB());
}

void driveStraightDist(int dist);
void turnRadiansRight(float radians);
void turnRadiansLeft(float radians);
void turnRadiansStationary(float radians, char dir);
void driveToObstacle();

int main()
{
    driveToObstacle();
    turnRadiansStationary(M_PI / 2, 'l');
    driveToObstacle();
    turnRadiansStationary(M_PI / 2, 'r');
    driveStraightDist(1000);
}

void driveStraightDist(int dist)
{
    int wheelDiameterMm = 66;
    float rotations = (dist / (wheelDiameterMm * M_PI));
    int polaritySwaps = (rotations * 192);
    printf("Pole swaps: \t%d\n", polaritySwaps);
    while ((hall_sensor.get_countA() + hall_sensor.get_countB()) / 2 < polaritySwaps)
    {
        robo.drive(.25);
        ThisThread::sleep_for(10);
    }
    robo.stop();
    ThisThread::sleep_for(500);
    hall_sensor.resetAll();
}
void turnRadiansRight(float radians)
{
    int wheelDiameterMm = 66;
    float arcLength = radians * (182);
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
    float arcLength = radians * (182);
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

void turnRadiansStationary(float radians, char dir)
{
    int wheelDiameterMm = 66;
    float arcLength = radians * (182/2);
    float rotations = (arcLength / (wheelDiameterMm * M_PI));
    int polaritySwaps = (rotations * 192);

    while (hall_sensor.get_countA() < polaritySwaps && hall_sensor.get_countB() < polaritySwaps)
    {
        if (dir == 'l')
        {
            robo.leftFWD(.3);
            robo.rightREV(.3);
        }
        else if (dir == 'r')
        {
            robo.rightFWD(.3);
            robo.leftREV(.3);
        }
        ThisThread::sleep_for(10);
    }
    robo.stop();
    ThisThread::sleep_for(500);
    hall_sensor.resetAll();
}

void driveToObstacle(){
    sensor.init(true);
    printf("Initialisation completed!\r\n");
    sensor.setTimeout(500);
    int dist = sensor.readRangeSingleMillimeters();
    driveStraightDist(dist - 100);
}