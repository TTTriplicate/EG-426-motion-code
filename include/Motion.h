#ifndef Motion_h
#define Motion_h

#include <mbed.h>

#define    DEFAULT_SPEED    0.5
#define    LOW       0
#define    HIGH      1
#define LEFT_PIN PTE20
#define RIGHT_PIN PTE31
#define PWM_PERIOD 0.01


class Motion
{

private:


public:
    void stop();
    void leftStop();
    void rightStop();
    void drive();
    void drive(float);
    void reverse();
    void reverse(float);
    void turnLeft();
    void turnLeft(float);
    void turnRight();
    void turnRight(float);
    void leftFWD();
    void leftFWD(float);
    void rightFWD();
    void rightFWD(float);
    void leftREV();
    void leftREV(float);
    void rightREV();
    void rightREV(float);

    Motion();

};

#endif
