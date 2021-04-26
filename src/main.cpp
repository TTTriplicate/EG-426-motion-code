#include <mbed.h>
#include <math.h>
#include <vector>
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
void findForwardGap();
int countToDrive(int dist);
int arcLength(float radians);
int rangeFinder();

int main()
{
    ThisThread::sleep_for(3s);
    findForwardGap();
    driveStraightDist(1000);
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
        robo.drive(.25);
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
    distance = rangeFinder();
    driveStraightDist(distance - 150);
}

void findForwardGap()
{
    sensor.init(true);
    sensor.setTimeout(500);
    std::vector<std::pair<int, int>> distanceAndHeading;
    int counts = countToDrive(arcLength(M_PI / 2));
    int count = 0;
    while (count < counts)
    {
        robo.turnLeft(.25);
        count = (hall_sensor.get_countB() + hall_sensor.get_countA()) / 2;
        int read = sensor.readRangeSingleMillimeters();
        if (read > 1000)
            distanceAndHeading.push_back(std::make_pair(read, count * -1));
        ThisThread::sleep_for(10ms);
    }
    robo.stop();
    ThisThread::sleep_for(500);
    hall_sensor.resetAll();
    count = 0;
    while (count < counts)
    {
        robo.turnRight(.25);
        count = (hall_sensor.get_countB() + hall_sensor.get_countA()) / 2;
        ThisThread::sleep_for(10);
    }
    robo.stop();
    ThisThread::sleep_for(500);
    hall_sensor.resetAll();
    count = 0;
    while (count < counts)
    {
        robo.turnRight(.25);
        count = (hall_sensor.get_countB() + hall_sensor.get_countA()) / 2;
        int read = sensor.readRangeSingleMillimeters();
        if (read > 1000)
            distanceAndHeading.push_back(std::make_pair(read, count));
        ThisThread::sleep_for(10ms);
    }
    robo.stop();
    ThisThread::sleep_for(500ms);
    hall_sensor.resetAll();
    int maxDist = distanceAndHeading[0].first;
    int headingCount;

    for (unsigned i = 1; i < distanceAndHeading.size(); i++)
    {
        printf("%d : %d\n", distanceAndHeading[i].first, distanceAndHeading[i].second);
        if (distanceAndHeading[i].first > maxDist)
        {
            maxDist = distanceAndHeading[i].first;
            headingCount = distanceAndHeading[i].second;
        }
    }

    int finalCount = (headingCount > 0 ? counts - headingCount : counts + abs(headingCount));
    printf("final count: %d\n", finalCount);
    count = 0;

    while (count < finalCount)
    {
        robo.leftREV(.27);
        robo.rightFWD(.25);
        count = (hall_sensor.get_countB() + hall_sensor.get_countA()) / 2;
        ThisThread::sleep_for(10);
    }
    robo.stop();
    ThisThread::sleep_for(500);
    hall_sensor.resetAll();
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
