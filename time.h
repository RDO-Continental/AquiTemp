#ifndef TIME_h
#define TIME_h

extern void time_init(void);
extern void time_bgd(void);
extern void time_StartChrono(void);
extern void time_StopChrono(void);
extern String DateString;   /* Date courante */
extern String TimeString;   /* Heure courante */
extern String ChronoString; /* Temps depuis le début de l'enregistrement */
#endif
