/*
    Bussola.h - Biblioteca com a Classe que define a bussola
  Author: André Eppinghaus
  Data: 04/09/2016
  Instrucoes: README.txt

  Created by David A. Mellis, November 2, 2007.
  Released into the public domain.
*/
#include "../TesteClass/TesteClass.h"
#include "Arduino.h"
#include "../HMC5883L/HMC5883L.h";
#include "Bussola.h"

void Bussola::setup() {
	Serial.begin(9600);
	
	Serial.println("configurando magnetrometro");
	int error = this->SetScale(1.3); // Set the scale of the compass.
	
	if(error != 0) // If there is an error, print it out.
		Serial.println(this->GetErrorText(error));
  
	Serial.println("Setting measurement mode to continous.");
	error = this->SetMeasurementMode(0); // Set the measurement mode to Continuous
	if(error != 0) {  // If there is an error, print it out.
		Serial.println("Nao ok");
		Serial.println(this->GetErrorText(error));
		this->error =  0; //false
	}else {
		this->error = 1; //true
		Serial.println("ok");
	}
	
}

int Bussola::getStatus() {
	return this->error;
}

float Bussola::getGraus() {
	
	// Retrive the raw values from the compass (not scaled).
  MagnetometerRaw raw = this->ReadRawAxis();
  // Retrived the scaled values from the compass (scaled to the configured scale).
  MagnetometerScaled scaled = this->ReadScaledAxis();
  
  // Values are accessed like so:
  int MilliGauss_OnThe_XAxis = scaled.XAxis;// (or YAxis, or ZAxis)

  // Calculate heading when the magnetometer is level, then correct for signs of axis.
  float heading = atan2(scaled.YAxis, scaled.XAxis);
  
  // Once you have your heading, you must then add your 'Declination Angle', which is the 'Error' of the magnetic field in your location.
  // Find yours here: http://www.magnetic-declination.com/
  // Mine is: 2� 37' W, which is 2.617 Degrees, or (which we need) 0.0456752665 radians, I will use 0.0457
  // If you cannot find your Declination, comment out these two lines, your compass will be slightly off.
  float declinationAngle = 0.0457;
  heading += declinationAngle;
  
  // Correct for when signs are reversed.
  if(heading < 0)
    heading += 2*PI;
    
  // Check for wrap due to addition of declination.
  if(heading > 2*PI)
    heading -= 2*PI;
   
  // Convert radians to degrees for readability.
  float headingDegrees = heading * 180/M_PI; 
  
  //exibe valores da classe
  /*
   Serial.print("Raw:\t");
   Serial.print(raw.XAxis);
   Serial.print("   ");   
   Serial.print(raw.YAxis);
   Serial.print("   ");   
   Serial.print(raw.ZAxis);
   Serial.print("   \tScaled:\t");
   
   Serial.print(scaled.XAxis);
   Serial.print("   ");   
   Serial.print(scaled.YAxis);
   Serial.print("   ");   
   Serial.print(scaled.ZAxis);

   Serial.print("   \tHeading:\t");
   Serial.print(heading);
   Serial.print(" Radians   \t");
   Serial.print(headingDegrees);
   Serial.println(" Degrees   \t");
   */
   
  // Normally we would delay the application by 66ms to allow the loop
  // to run at 15Hz (default bandwidth for the HMC5883L).
  // However since we have a long serial out (104ms at 9600) we will let
  // it run at its natural speed.
  // delay(66);
  
  return headingDegrees;
	
}

