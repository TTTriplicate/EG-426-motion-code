#include <mbed.h>
#include "Motion.h"
#include "Encoders.h"

Encoders hall_sensor;

void output(){
  printf("hall sensor reads\n Left : %d \t Right : %d\n", hall_sensor.get_countA(), hall_sensor.get_countB());
}

int main() {

  // put your setup code here, to run once:
  BufferedSerial pc(USBTX, USBRX);
  Motion robo;

  bool slalom = 1;
  printf("code running...");

  robo.drive();
  ThisThread::sleep_for(1s);
  while(1) {
    output();
    // put your main code here, to run repeatedly:
    if(slalom){
      if (hall_sensor.get_countA() > 1000){
        hall_sensor.resetB();
        slalom = 0;
        robo.leftFWD(.4);
        robo.rightFWD(.25);
      }
      else{
        ThisThread::sleep_for(200);
      }
    }
    else{
      if (hall_sensor.get_countB() > 1000){
        hall_sensor.resetA();
        slalom = 1;
        robo.leftFWD(.25);
        robo.rightFWD(.4);
      }
      else{
        ThisThread::sleep_for(200);
      }

    }
  }
  output();
}