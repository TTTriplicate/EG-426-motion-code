#include <mbed.h>
#include "Motion.h"
#include "Encoders.h"

Encoders hall_sensor;

void output()
{
  printf("hall sensor reads\n Left : %d \t Right : %d\n", hall_sensor.get_countA(), hall_sensor.get_countB());
}

int main()
{

  // put your setup code here, to run once:
  BufferedSerial pc(USBTX, USBRX);
  Motion robo;

  bool slalom = 1;
  int turnDist = (192 * 3);
  printf("code running...");

  robo.drive(.5);
  while (1)
  {
    //output();
    //put your main code here, to run repeatedly:
    if (slalom)
    {
      if (hall_sensor.get_countA() > turnDist)
      {
        hall_sensor.resetB();
        slalom = 0;
        robo.leftFWD(.8);
        robo.rightFWD(.66);
      }
      else
      {
        ThisThread::sleep_for(200);
      }
    }
    else
    {
      if (hall_sensor.get_countB() > turnDist)
      {
        hall_sensor.resetA();
        slalom = 1;
        robo.rightFWD(.8);
        robo.leftFWD(.66);
      }
      else
      {
        ThisThread::sleep_for(200);
      }
    }
  }
  //output();
}