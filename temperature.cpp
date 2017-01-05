//#define DEBUG
#include "debug.h"

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
      if (deviceAddress[i] < 16) DEBUG_PRINT("0");
      DEBUG_PRINT(deviceAddress[i], HEX);
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
  
    DEBUG_PRINTLN("Temperature initialisation");
    // locate devices on the bus
    DEBUG_PRINT("Locating devices...");
    DEBUG_PRINT("Found ");
    DEBUG_PRINTDEC(sensors.getDeviceCount(), DEC);
    DEBUG_PRINTLN(" devices.");


  TempDevicesFound = sensors.getDeviceCount(); 
  
  /*On va limiter le nb le capteur au nombre max configur� car on ne fait pas d'allocation memoire dynamique pour insideThermometer[] */
  if (TempDevicesFound>NUMBER_SENSOR) {TempDevicesFound=NUMBER_SENSOR;}
  
  // Search for devices by index
  for (i = 0; i < TempDevicesFound; i++)
    if (!sensors.getAddress(insideThermometer[i], i)) {
        DEBUG_PRINTLN("Unable to find address for Device" + i); 
    }
      
  #ifdef DEBUG    
    for (i = 0; i < TempDevicesFound; i++)
    {    
      DEBUG_PRINT("Device " + (String)i + " Address: ");
      printAddress(insideThermometer[i]);
      DEBUG_PRINTLN();
    }
  #endif
  /* Changement de la r�solution par capteur 9 ou 12 bits */
  for (i = 0; i < TempDevicesFound; i++) {
    sensors.setResolution(insideThermometer[i], TEMPERATURE_PRECISION);
    DEBUG_PRINT("Device "+(String)i + " Resolution: ");
    DEBUG_PRINTDEC(sensors.getResolution(insideThermometer[i]), DEC); 
      DEBUG_PRINTLN();
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
	      DEBUG_PRINTLN(tempC[i]);
  	}
	}
  DEBUG_PRINT("Requesting temperatures...");
  sensors.setWaitForConversion(false);  /* On lance la conversion mais on n'attend pas la r�ponse */
  sensors.requestTemperatures();        /* On aura la r�ponse au coup d'apr�s */
  firstAqui=1;                          /* On indique que la premiere acquisition a bien �t� lanc�e */
  DEBUG_PRINTLN("DONE"); 
}

