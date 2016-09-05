#define _TASK_SLEEP_ON_IDLE_RUN
#include <TaskScheduler.h>
#include "affiche.h"
#include "rotary.h"
#include "temperature.h"
#include "magneto.h"
#include "time.h"
#include "sdcard.h"

Scheduler runner;
// Callback methods prototypes
void t10msCallback();
void t100msCallback();
void t1000msCallback();
void txReccordCallback();
byte action_state;
long syncTimeRecms;
long syncTime10ms;
long syncTime100ms;

long durTime10ms;
long durTime100ms;
enum action_states {
  INIT,
  VIEW_ACQUI,
  START_RECORD,
  RECORD_RUNNING,
  STOP_RECORD,
  RECORD_STOPPED
};
// Tasks
Task t10ms(10, TASK_FOREVER, &t10msCallback, &runner, true);  //adding task to the chain on creation
Task t100ms(100, TASK_FOREVER, &t100msCallback, &runner, true);  //adding task to the chain on creation
Task t1000ms(1000, TASK_FOREVER, &t1000msCallback, &runner, true);  //adding task to the chain on creation
Task txReccord(5000, TASK_FOREVER, &txReccordCallback);

char *csv_header[] = { "Date", "Heure", "T1", "T2", "T3", "T4", "Circulateur"};

void t10msCallback() {
    Serial.print("T10ms=");
    Serial.println(millis()-syncTime10ms);
    syncTime10ms=millis();
    durTime10ms=millis();
  rotary_bgd(); /* Produit l'état du bouton : Incrément/Decrement et Appuis */
  
  /* Gestion du menu */
  /* En focntion de l'état du bouton change d'état ou demande des actions */

  /* gestion des actions */
  //Serial.print("action_state=");
  //Serial.println(action_state);
  
  switch (action_state) {
    case INIT:
      /* Ici on affiche le menu principal */
      if (   (rotary_select_changed == 1) /* on ne change d'état que sur transition du bouton */
             && ((rotary_state & SELECT_MASK) == PUSHED)
         )
      {
        action_state = START_RECORD;
        time_StartChrono();
      }
      break;
    case VIEW_ACQUI:
      /* On visualise les données en temsp réel (resol 100ms) */
      break;
    case START_RECORD:
      /* On reccupere l'intervale d'aquisition (et on le sauve) */
      /* On prépare la sauvegarde sur la flash */
      sdcard_start_rec(csv_header,7);
      /* Si tout va bien :*/
      menu_state = MENU_RECORD;
      /* On lance une tache sur cet intervale */
      runner.addTask(txReccord);
      txReccord.setInterval(5000);
      txReccord.enable();

      /* On passe en etat d'attente */
      action_state = RECORD_RUNNING;
      break;
    case RECORD_RUNNING:
      /* Ici on attend simplement une action pour arreter l'enregistrement*/
      if (   (rotary_select_changed == 1) /* on ne change d'état que sur transition du bouton */
             && ((rotary_state & SELECT_MASK) == PUSHED)
         )
      {
        menu_state = MENU_INIT;
        action_state = STOP_RECORD;
      }
      break;
    case STOP_RECORD:
      /* Ici on stoppe la tache d'enregistrement (on attends que la tache soit terminée) */
      txReccord.disable();
      runner.deleteTask(txReccord);
      action_state = RECORD_STOPPED;
      time_StopChrono();
      break;
    case RECORD_STOPPED:
      /* Ici on arrete proprement l'écriture en flash */
      sdcard_stop_rec();
      action_state = INIT;
      break;
    default:
      // if nothing else matches, do the default
      // default is optional
      break;
  }
Serial.print("Tdur=");
  Serial.println(millis()-durTime10ms);
}/*Fct_end*/

void txReccordCallback() {
   Serial.print("Trecms=");
   Serial.println(millis()-syncTimeRecms);
   syncTimeRecms=millis();
  /* On reccupere la date et l'heure */
  /* On reccupere les températures */
  /* On sauve sur la flash */
  sdcard_save_data();
}

void t100msCallback() {
    Serial.print("T100ms=");
    Serial.println(millis()-syncTime100ms);
    syncTime100ms=millis();
    magneto_bgd();
    time_bgd();
    durTime100ms=millis();
    affiche_bgd();
    Serial.print("Tdur100ms=");
  Serial.println(millis()-durTime100ms);
}

void t1000msCallback() {
  temperature_bgd();
}
void setup () {
  Serial.begin(115200);
  Serial.println("Scheduler TEST");
  rotary_init();
  temperature_init();
  magneto_init();
  time_init();
  affiche_init();
  sdcard_init();
  delay(1000);

  runner.startNow();  // set point-in-time for scheduling start
}


void loop () {

  runner.execute();
}
