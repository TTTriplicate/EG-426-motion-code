#include "Encoders.h"

InterruptIn motorRgt(RIGHT_PIN);
InterruptIn motorLft(LEFT_PIN);



int cntA;
int cntB;

bool dirA = 1;
bool dirB = 1;



int Encoders::get_countA()
{
    return cntA;
}

int Encoders::get_countB()
{
    return cntB;
}
void Encoders::set_dirA(bool A)
{
    dirA = A;
}

void Encoders::set_dirB(bool B)
{
    dirB = B;
}


void ISR1()
{
    if(dirA) ++cntA;

    else --cntA;

}

void ISR2()
{
    if(dirB) ++cntB;

    else --cntB;
}

void Encoders::resetA()
{
    cntA=0;
    }

void Encoders::resetB()
{
    cntB=0;
    }

Encoders::Encoders()
{
cntA = 0;
cntB = 0;

    motorRgt.mode(PullUp);

    motorLft.mode(PullUp);

    motorRgt.rise(&ISR1);

    motorLft.rise(&ISR2);


}
