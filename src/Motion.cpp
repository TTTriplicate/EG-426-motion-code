
#include "Motion.h"

PwmOut leftMotor(LEFT_PIN);
PwmOut rightMotor(RIGHT_PIN);

DigitalOut LCTRL1(PTC6);
DigitalOut LCTRL2(PTC5);
DigitalOut RCTRL1(PTC12);
DigitalOut RCTRL2(PTC13);



void Motion::stop()
{
    leftStop();
    rightStop();
}


void Motion::drive()
{
    leftFWD();
    rightFWD();

}

void Motion::drive(float speed)
{
    float mspeed;
    mspeed=speed;
    leftFWD(mspeed);
    rightFWD(mspeed);


}

void Motion::reverse()
{
    leftREV();
    rightREV();
}

void Motion::reverse(float speed)
{

    float mspeed;
    mspeed=speed;
    leftREV(mspeed);
    rightREV(mspeed);
}

void Motion::turnLeft()
{
    leftREV();
    rightFWD();
}

void Motion::turnLeft(float speed)
{

    float mspeed;
    mspeed=speed;
    leftREV(mspeed);
    rightFWD(mspeed);
}

void Motion::turnRight()
{
    leftFWD();
    rightREV();
}

void Motion::turnRight(float speed)
{

    float mspeed;
    mspeed=speed;
    leftFWD(mspeed);
    rightREV(mspeed);
}

//Functions used to build Motor function
//Control individual motors (left & right)

void Motion::leftStop()
{
    LCTRL1 = LOW;
    LCTRL2 = LOW;
    leftMotor = 0;
}

void Motion::rightStop()
{
    RCTRL1 = LOW;
    RCTRL2 = LOW;
    rightMotor = 0;
}

void Motion::leftFWD()
{
    LCTRL1 = HIGH;
    LCTRL2 = LOW;
    leftMotor = DEFAULT_SPEED;
}

void Motion::leftFWD(float speed)
{
    LCTRL1 = HIGH;
    LCTRL2 = LOW;
    leftMotor = speed;
}

void Motion::rightFWD()
{
    RCTRL1 = HIGH;
    RCTRL2 = LOW;
    rightMotor = DEFAULT_SPEED;
}

void Motion::rightFWD(float speed)
{
    RCTRL1 = HIGH;
    RCTRL2 = LOW;
    rightMotor = speed;
}

void Motion::leftREV()
{
    LCTRL1 = LOW;
    LCTRL2 = HIGH;
    leftMotor = DEFAULT_SPEED;
}

void Motion::leftREV(float speed)
{
    LCTRL1 = LOW;
    LCTRL2 = HIGH;
    leftMotor = speed;
}


void Motion::rightREV()
{
    RCTRL1 = LOW;
    RCTRL2 = HIGH;
    rightMotor = DEFAULT_SPEED;
}


void Motion::rightREV(float speed)
{
    RCTRL1 = LOW;
    RCTRL2 = HIGH;
    rightMotor = speed;
}

 Motion::Motion(){
    leftMotor.period(PWM_PERIOD);
    rightMotor.period(PWM_PERIOD);
    }
