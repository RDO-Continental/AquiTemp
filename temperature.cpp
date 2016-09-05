//#define DEBUG
#include "Arduino.h"
#include <OneWire.h>
#include <DallasTemperature.h>
#include "temperature.h"

#define ONE_WIRE_BUS 6            /* PIN utilis�e pour la ligne de capteur 18B20 */
#define TEMPERATURE_PRECISION 12

float tempC[NUMBER_SENSOR];          /* Tableau des aquis */
OneWire oneWire(ONE_WIRE_BUS);       /* Definition de la ligne de communication 1 wire */
DallasTemperature sensors(&oneWire); /* Definition des capteurs Dallas Temperature sur la ligne oneWire */

byte TempDevicesFound = 0;                /* Nb de capteurs d�tect�s � l'init */
byte firstAqui=0;                     /* Indique si la premiere aquisition a �t� faite */

DeviceAddress insideThermometer[NUMBER_SENSOR]; /* Tableau des adresses de chaque capteur */

#ifdef DEBUG 
  /* Fonction pour afficher les adresses de chaque capteur */
  /* Permet de les identifier individuellement             */
  void printAddress(DeviceAddress deviceAddress)
  { 
    byte i;
    for (i = 0; i < 8; i++)
    {
      // zero pad the address if necessary
      if (deviceAddress[i] < 16) Serial.print("0");
      Serial.print(deviceAddress[i], HEX);
    }
  }
#endif

/*-------------------------------------------------------------------------------------------------*/
/* Initialisation en fonction du nombre r�el de capteurs (limit� � NUMBER_SENSOR)                  */
/*-------------------------------------------------------------------------------------------------*/
void temperature_init(void)
{
  byte i;
  
  sensors.begin();
  
  #ifdef DEBUG
    Serial.println("Temperature initialisation");
    // locate devices on the bus
    Serial.print("Locating devices...");
    Serial.print("Found ");
    Serial.print(sensors.getDeviceCount(), DEC);
    Serial.println(" devices.");
  #endif

  TempDevicesFound = sensors.getDeviceCount(); 
  
  /*On va limiter le nb le capteur au nombre max configur� car on ne fait pas d'allocation memoire dynamique pour insideThermometer[] */
  if (TempDevicesFound>NUMBER_SENSOR) {TempDevicesFound=NUMBER_SENSOR;}
  
  // Search for devices by index
  for (i = 0; i < TempDevicesFound; i++)
    if (!sensors.getAddress(insideThermometer[i], i)) {
      #ifdef DEBUG
        Serial.println("Unable to find address for Device" + i); 
      #endif
    }
      
  #ifdef DEBUG    
    for (i = 0; i < TempDevicesFound; i++)
    {    
      Serial.print("Device " + (String)i + " Address: ");
      printAddress(insideThermometer[i]);
      Serial.println();
    }
  #endif
  /* Changement de la r�solution par capteur 9 ou 12 bits */
  for (i = 0; i < TempDevicesFound; i++) {
    sensors.setResolution(insideThermometer[i], TEMPERATURE_PRECISION);
    #ifdef DEBUG 
      Serial.print("Device "+(String)i + " Resolution: ");
      Serial.print(sensors.getResolution(insideThermometer[i]), DEC); 
      Serial.println();
    #endif
  }
}

/*-------------------------------------------------------------------------------------------------*/
/* Production p�riodique des diff�rentes temp�ratures                                              */
/*-------------------------------------------------------------------------------------------------*/
void temperature_bgd(void)
{ 
  byte i;
  if (firstAqui==1){ 
  	/* Memorisation des r�sultats */
  	for (i=0 ; i < TempDevicesFound ; i++){
	    tempC[i] = sensors.getTempC(insideThermometer[i]);
    	#ifdef DEBUG 
	      Serial.println(tempC[i]);
    	#endif
  	}
	}
  #ifdef DEBUG 
    Serial.print("Requesting temperatures...");
  #endif
  sensors.setWaitForConversion(false);  /* On lance la conversion mais on n'attend pas la r�ponse */
  sensors.requestTemperatures();        /* On aura la r�ponse au coup d'apr�s */
  firstAqui=1;                          /* On indique que la premiere acquisition a bien �t� lanc�e */
  #ifdef DEBUG 
    Serial.println("DONE");
  #endif
  
}

