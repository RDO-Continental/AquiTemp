//#define DEBUG
#include "Arduino.h"
#include <Wire.h>
#include <HMC5883L.h>

HMC5883L compass;
float magneX;

void magneto_init(void){
   #ifdef DEBUG
   Serial.begin(9600);
   #endif

  // Initialize HMC5883L
  #ifdef DEBUG
  Serial.println("Initialize HMC5883L");
  #endif
  
  while (!compass.begin())
  {
  #ifdef DEBUG
  Serial.println("Could not find a valid HMC5883L sensor, check wiring!");
  #endif
    delay(500);
  }
  
  compass.setRange(HMC5883L_RANGE_1_3GA);
  compass.setMeasurementMode(HMC5883L_CONTINOUS);
  compass.setDataRate(HMC5883L_DATARATE_15HZ);
  compass.setSamples(HMC5883L_SAMPLES_1);
}

void magneto_bgd(void) {
  Vector raw = compass.readRaw();
  Vector norm = compass.readNormalize();
  magneX=raw.XAxis;
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

