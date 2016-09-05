// Date and time functions using a DS1307 RTC connected via I2C and Wire lib
#include "Arduino.h"
#include <Wire.h>
#include "RTClib.h"

RTC_DS1307 rtc;

String DateString;
String TimeString;
String ChronoString;
DateTime start_time; /* Memorisation du debut du chrono */

byte chrono_started=0;

#define INIT_NOTOK 0
#define INIT_OK    1
byte time_status;

void time_init() {
  Wire.begin();
  time_status=INIT_OK;
  
  if (! rtc.begin()) {
    Serial.println("Couldn't find RTC");
    time_status=INIT_NOTOK;
    //while (1);
  }
  /* D�commenter la ligne ci dessous pour mettre � jour l'heure : heure du build */
  //rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  if ((time_status==INIT_OK)&&(! rtc.isrunning())) {
    Serial.println("RTC is NOT running!");
    // following line sets the RTC to the date & time this sketch was compiled
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    // This line sets the RTC with an explicit date & time, for example to set
    // January 21, 2014 at 3am you would call:
    // rtc.adjust(DateTime(2014, 1, 21, 3, 0, 0));
  }
}

/*-------------------------------------------------------------------------------------------------*/
/* Production p�riodique de la date, de l'heure et du chrono, directement format� pour l'affichage */
/*-------------------------------------------------------------------------------------------------*/
void time_bgd (void) {
  if (time_status==INIT_OK) {
    DateTime now = rtc.now(); /* Lecture de l'heure courante */
    DateString= String(now.year(),DEC) + "/"+String(now.month(),DEC) + "/"+String(now.day(),DEC);
    TimeString= String(now.hour(),DEC) + ":"+String(now.minute(),DEC) + ":"+String(now.second(),DEC);
    
    ChronoString = "Error"; /* Si le chrono n'est pas d�marr�, on peut afficher un erreur */
    if (chrono_started) {
      TimeSpan delta_now=rtc.now()- start_time;
      ChronoString = String(delta_now.days(),DEC) + "|"+String(delta_now.hours(),DEC) + ":"+String(delta_now.minutes(),DEC) + ":"+String(delta_now.seconds(),DEC);
      //Serial.println(ChronoString);
    }
  }
  else{
  	DateString= "Error";
    TimeString= "Error";
    ChronoString = "Error";
  }
}

void time_StartChrono(void) {
  if (time_status==INIT_OK) {
  	start_time = rtc.now();
  	chrono_started=1;
  }
}

void time_StopChrono(void) {
  chrono_started=0;
}
