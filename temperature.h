#ifndef TEMPERATURE_h
#define TEMPERATURE_h

#define NUMBER_SENSOR 5 /* Nombre de capteurs 18B20 sur la ligne */

extern void temperature_init(void);
extern void temperature_bgd(void);
extern float tempC[NUMBER_SENSOR];
extern byte TempDevicesFound;
#endif
