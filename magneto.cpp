//#define DEBUG
#include "debug.h"

#include "Arduino.h"
#include <Wire.h>
#include <HMC5883L.h>

HMC5883L compass;
float magneX;
byte MagnetoFound;
byte MagnetoDiagCounter;

void magneto_init(void){

  // Initialize HMC5883L
  DEBUG_PRINTLN("Initialize HMC5883L");
  
  MagnetoDiagCounter=0;
  MagnetoFound=0;
  while ((!compass.begin()) && (MagnetoDiagCounter<5))
  {
    DEBUG_PRINTLN("Could not find a valid HMC5883L sensor, check wiring!");
    delay(500);
    MagnetoDiagCounter++;
  }
  if (MagnetoDiagCounter<5) {
    MagnetoFound=1;
  compass.setRange(HMC5883L_RANGE_1_3GA);
  compass.setMeasurementMode(HMC5883L_CONTINOUS);
  compass.setDataRate(HMC5883L_DATARATE_15HZ);
  compass.setSamples(HMC5883L_SAMPLES_1);
  }
}

void magneto_bgd(void) {
  if (MagnetoDiagCounter<5) {
    Vector raw = compass.readRaw();
  Vector norm = compass.readNormalize();
  magneX=raw.XAxis;
  }
  
  /*Serial.print(" Xraw = ");
  Serial.print(raw.XAxis);
  Serial.print(" Yraw = ");
  Serial.print(raw.YAxis);
  Serial.print(" Zraw = ");
  Serial.println(raw.ZAxis);
  Serial.print(" Xnorm = ");
  Serial.print(norm.XAxis);
  Serial.print(" Ynorm = ");
  Serial.print(norm.YAxis);
  Serial.print(" ZNorm = ");
  Serial.print(norm.ZAxis);
  Serial.println();  */
}

