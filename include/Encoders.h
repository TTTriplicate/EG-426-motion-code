#ifndef Encoders_h
#define Encoders_h
#include <mbed.h>

#define RIGHT_PIN D4
#define LEFT_PIN D5


class Encoders
{
  public:
  int get_countA();
  int get_countB();
  void set_dirA(bool);
  void set_dirB(bool);

  void resetA();
  void resetB();

  Encoders();

  private:





};

#endif
