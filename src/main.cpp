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

void driveStraightDist(int dist, bool first = true);
void turnRadiansRight(float radians);
void turnRadiansLeft(float radians);

int main()
{
    printf("Start...\r\n");
    sensor.init(true);
    printf("Initialisation completed!\r\n");
    sensor.setTimeout(500);
    while (1)
    {
        printf("%u\r\n", sensor.readRangeSingleMillimeters());
        if (sensor.timeoutOccurred())
        {
            printf("TIMEOUT!\r\n");
        }
    }
}

void driveStraightDist(int dist, bool first)
{
    int wheelDiameterMm = 66;
    float rotations = (dist / (wheelDiameterMm * M_PI));
    int polaritySwaps = (rotations * 192);
    printf("Pole swaps: \t%d\n", polaritySwaps);
    while ((first ? hall_sensor.get_countB() : hall_sensor.get_countA()) < polaritySwaps)
    {
        robo.drive(.5);
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
    float arcLength = radians * (182);
    printf("Arc length: \t %d\n", static_cast<int>(arcLength));
    float rotations = (arcLength / (wheelDiameterMm * M_PI));
    int polaritySwaps = (rotations * 192);
    printf("Pole swaps for turn: \t%d\n", polaritySwaps);
    while (hall_sensor.get_countA() < polaritySwaps)
    {
        robo.leftFWD(.5);
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
    float arcLength = radians * (182);
    printf("Arc length: \t %d\n", static_cast<int>(arcLength));
    float rotations = (arcLength / (wheelDiameterMm * M_PI));
    int polaritySwaps = (rotations * 192);
    printf("Pole swaps for turn: \t%d\n", polaritySwaps);
    while (hall_sensor.get_countB() < polaritySwaps)
    {
        robo.rightFWD(.5);
        ThisThread::sleep_for(10);
        output();
    }
    robo.stop();
    ThisThread::sleep_for(10s);
    hall_sensor.resetAll();
}
