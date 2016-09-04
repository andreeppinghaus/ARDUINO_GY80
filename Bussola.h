/*
  Bussola.h -  Biblioteca para uso do magnetrometro
  
*/
#ifndef Bussola_h
#define Bussola_h
#include "Arduino.h"
#include "../HMC5883L/HMC5883L.h";
#include "../TesteClass/TesteClass.h"

class Bussola : public HMC5883L
{
  public:
    
    void setup();
    int getStatus();
    float getGraus();
   

  private:
  int error; //erro da inicializacao
   
};
#endif
