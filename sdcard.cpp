//#define DEBUG
#include "Arduino.h"
#include <SD.h>      /* Attention : Librairie SD specifique pour MEGA */
#include "sdcard.h"
#include "time.h"
#include "temperature.h"
#include "magneto.h"

#define DATALOG_CS   10 /* Shield data-logger : CS = pin 10*/

byte sdcard_status = 0; 

// Le fichier d'enregistrement des mesures
File logfile;

// === gestion des erreurs
void error(char *str)
{
  Serial.print("erreur: ");
  Serial.println(str);

  // La LED rouge indique les erreurs
  //digitalWrite(redLEDpin, HIGH);

  //while (1);
}

/*-------------------------------------------------------------------------------------------------*/
/* Initialisation avec detection d'erreur                                                          */
/*-------------------------------------------------------------------------------------------------*/
void sdcard_init(void)
{
  Serial.println("Mon datalogger ");
  Serial.println();

  // Initialiser la SD
  Serial.print("Initialiser la carte SD...");
  pinMode(DATALOG_CS, OUTPUT);
  
  /* regarder si la carte SD est présente et initialisable */
  /* Attention, pour la MEGA, il faut initialiser le SPI virtuel pour fonctionner avec notre sheild */
  /* car le SPI n'est pas cablé sur 11, 12 et 13 mais sur 52.... */
  if (!SD.begin(DATALOG_CS,11,12,13)) {
    error("Carte SD echec ou absente");
    sdcard_status = SDCARD_MISSING;
  }
  else {
    Serial.println("carte initialisee");
    sdcard_status = SDCARD_AVAILABLE;
  }
}

/*-------------------------------------------------------------------------------------------------*/
/* Démarrage de l'enregistrement:                                                                  */
/*     - Determine automatiqueemnt le nouveau nom de fichier                                       */
/*     - sauve la ligne d'entete passée en param.                                                  */
/*     - detection d'erreur                                                                        */
/*-------------------------------------------------------------------------------------------------*/
void sdcard_start_rec(char * tab_header[], byte nb_params) {
   byte i;
  // Créer un nouveau fichier, on les numérote successivement
  char filename[] = "LOGGER00.CSV";
  for (uint8_t i = 0; i < 100; i++) {
    filename[6] = i / 10 + '0';
    filename[7] = i % 10 + '0';
    if (! SD.exists(filename)) {
      // N'ouvrir un nouveau fichier que s il n'existe pas
      logfile = SD.open(filename, FILE_WRITE);
      break;  // quiter la boucle
    }
  }

  if (! logfile) {
    error("Ne peut pas creer le fichier");
    sdcard_status = SDCARD_NEWFILE_ERROR;
  }
    Serial.print("Enregistrer dans : ");
    Serial.println(filename);
    //Première ligne du fichier CSV pour excel
    for (i = 0; i < nb_params; i++) {
      logfile.print(tab_header[i]);
      logfile.print("; ");
    }
    logfile.println();
  
  //return(filename);
}

/*-------------------------------------------------------------------------------------------------*/
/* Arrêt de l'enregistrement:                                                                      */
/*-------------------------------------------------------------------------------------------------*/
void sdcard_stop_rec(void) {
  logfile.close();
}

void sdcard_save_data(void){ //char * data_tab[]) {
  /* Save : Date, Time, T1, T2, T3, T4, Circulateur*/
  byte i;
  logfile.print(DateString);
  logfile.print("; ");
  logfile.print(TimeString);
  logfile.print("; ");
  for (i = 0; i < NUMBER_SENSOR; i++) {
    logfile.print(tempC[i]);
    logfile.print("; ");
  }
  logfile.print(magneX);
  logfile.print("; ");
  logfile.println();

  /*Serial.print(DateString);
  Serial.print(", ");
  Serial.print(TimeString);
  Serial.print(", ");
  for (i = 0; i < NUMBER_SENSOR; i++) {
    Serial.print(tempC[i]);
    Serial.print(", ");
  }
  Serial.println();*/

}

// ======= Boucle de mesures
void sdcard_bgd(void)
{
  // Enregistrer les millisecondes écoulées depuis le départ
  uint32_t m = millis();
  logfile.print(m);           // millisecondes depuis le départ
  logfile.print(", ");
#if ECHO_TO_SERIAL
  Serial.print(m);
  Serial.print(", ");
#endif

  // Mesurer le time RTC
/*  now = RTC.now();
  logfile.print(now.unixtime()); // secondes depuis 01/01/1970
  logfile.print(", ");
  logfile.print('"');
  logfile.print(now.year(), DEC);  //année
  logfile.print("/");
  logfile.print(now.month(), DEC);  //mois
  logfile.print("/");
  logfile.print(now.day(), DEC);   //jour
  logfile.print(" ");
  logfile.print(now.hour(), DEC);  //heure
  logfile.print(":");
  logfile.print(now.minute(), DEC);  //minutes
  logfile.print(":");
  logfile.print(now.second(), DEC);  //secondes
  logfile.print('"');
#if ECHO_TO_SERIAL
  Serial.print(now.unixtime()); // secondes depuis 01/01/1970
  Serial.print(", ");
  Serial.print('"');
  Serial.print(now.year(), DEC);
  Serial.print("/");
  Serial.print(now.month(), DEC);
  Serial.print("/");
  Serial.print(now.day(), DEC);
  Serial.print(" ");
  Serial.print(now.hour(), DEC);
  Serial.print(":");
  Serial.print(now.minute(), DEC);
  Serial.print(":");
  Serial.print(now.second(), DEC);
  Serial.print('"');
#endif //ECHO_TO_SERIAL


  // === Stocker ces mesures en fichier de log
  logfile.print(", ");
  logfile.print((int) temperature);
  logfile.print(", ");
  logfile.print((int) humidity);
  logfile.print(", ");
  logfile.print(lumiere);


  logfile.print(", ");   //Fichier
  logfile.print(supplyvoltage);
#if ECHO_TO_SERIAL
  Serial.print(", ");     //Ecran
  Serial.print(supplyvoltage);
#endif // ECHO_TO_SERIAL

  logfile.println();
#if ECHO_TO_SERIAL
  Serial.println();
#endif // ECHO_TO_SERIAL

  //Puis on écrit sur le disque, ne pas synchroniser trop souvent, requiert 2048 bytes of I/O to SD card
  //cequi consomme du temps et de la puissance
  if ((millis() - syncTime) < SYNC_INTERVAL) return;
  syncTime = millis();
  logfile.flush();
*/

}

