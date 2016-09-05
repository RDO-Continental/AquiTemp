//#define DEBUG
#include "Arduino.h"
#include "Encoder.h"
#include "rotary.h"

#define ENC_BUTTONINTERVAL    10  // check button every 10 milliseconds, also debouce time
#define ENC_HOLDTIME          20  // click hold 20ms
#define ROTARY_PIN             4

Encoder myEnc(2, 3); /* Pins D2 and D3 */
long oldPosition  = -999;
byte pinBTN = 4;
byte oldButton = 0;
byte newButton = 0;
byte rotary_state = 0;
uint16_t keyDownTicks = 0;
byte rotary_select_changed=0;

void rotary_init(void){
  pinMode(ROTARY_PIN, INPUT);
  digitalWrite(ROTARY_PIN, HIGH);
}

void rotary_bgd(void){
    /* Lecture de la rotation du bouton */
    long newPosition = myEnc.read();
    if (newPosition/2 != oldPosition/2) {
      rotary_state=DECREMENT;
      if (newPosition > oldPosition){rotary_state=INCREMENT;}
      oldPosition = newPosition;
      #ifdef DEBUG
        Serial.print("New:");
        Serial.println(newPosition/2);
      #endif
    }

    /* Lecture du bouton : Appuis*/
    /* On va filtrer les parasites pendant 50ms */
    if (!newButton && (digitalRead(pinBTN) == LOW) ) { // key is down
      keyDownTicks++;
      if (keyDownTicks > (ENC_HOLDTIME / ENC_BUTTONINTERVAL)) {
        newButton = 1;
        keyDownTicks=0;
      }
    }
    else {
      if (digitalRead(pinBTN) == HIGH){
        if (keyDownTicks !=0) {
          keyDownTicks--;
        }
        else {
           newButton = 0;
        }
      }/*EndIf*/
    }/*EndElse*/
    /* On peut détecter la transition */
    if (newButton != oldButton) {
      rotary_state=UNPUSHED;
      if (newButton == 1) {rotary_state=PUSHED;} /* Si on trourne alors qu'il y a appuis, on n'en tinet pas compte */
      oldButton = newButton;
      rotary_select_changed=1;
      #ifdef DEBUG
        Serial.print("newButton:");
        Serial.println(newButton);
      #endif
    }
    else{
      rotary_select_changed=0;
    }
  }
