#include "Arduino.h"
#include "U8glib.h"
#include "SPI.h"
#include "temperature.h"
#include "magneto.h"
#include "affiche.h"
#include "time.h"

/* Digital pin number for the displays chip select pin */
#define OLED_CS 9
/* Digital pin number for the displays data/command pin */
#define OLED_DC 8
/* Digital pin number for the displays reset pin */
#define OLED_RST 7
//U8GLIB_SH1106_128X64_2X u8g(13, 11, OLED_CS, OLED_DC , OLED_RST);
U8GLIB_SH1106_128X64_2X u8g(OLED_CS, OLED_DC , OLED_RST);

byte menu_state = MENU_INIT;
byte recblink = 0;
byte recblinkcnt = 0;
long durDraw;
byte picture_finished;

void affiche_init(void) {
  /* Reset the display */
  Serial.print("test afficheur");
  /* Display some text using a small 8x8 fixed width font */
}

void draw(void) {
  char tmp_string[7];
  byte i;
  byte x;
  byte y;
  byte strSize;

  // graphic commands to redraw the complete screen should be placed here
  u8g.setFont(u8g_font_6x10);
  u8g.setFontRefHeightText();
  u8g.setFontPosTop();

  /* Partie Titre : On inverse : txt sur fond blanc */
  u8g.setDefaultForegroundColor();
  u8g.drawBox(0, 0, 123, 9);
  u8g.setDefaultBackgroundColor();
  u8g.drawStr( 16, 0, "SECTION RDO");

  /* On reinverse : txt sur fond noir */
  u8g.setDefaultForegroundColor();
 /* Partie centrale */
 /* Balayage des températures */
  for (i = 0; i < TempDevicesFound; i++) {
    x = 0;
    y = i;
    if (i > 2) { /* On passa à la seconde colonne*/
      x = 62;
      y = i - 3;
    }
    strSize = 33; /*u8g.getStrWidth(tempC[i]);*/
    u8g.drawCircle(x + strSize + 3, (y * 10) + 13, 2); /* Creation du caractere � */
    u8g.drawStr( x + strSize + 3, (y * 10) + 10, " C");
    u8g.setPrintPos(x, (y * 10) + 10);
    u8g.print(tempC[i]);
  }
/* Affichage du Magnetometre */
    y++;
    u8g.setPrintPos(x, ((y) * 10) + 10);
    //u8g.setPrintPos(0, 20);
    u8g.print(magneX);

/* Affichage de la partie basse */
  u8g.drawLine(0, 52, 123, 52); /* Separation partie menu en bas */
  u8g.drawLine(60, 52, 60, 63); /* Split menu en 2 */

  /* Gestion du pseudo menu */
  if (menu_state == MENU_INIT) {
    u8g.setPrintPos(0, 54);
    u8g.print("START");
    u8g.drawLine(0, 63, 27, 63);

    u8g.setPrintPos(72, 54);
    u8g.print(TimeString);
  }

  if (menu_state == MENU_RECORD) {
    u8g.setPrintPos(0, 54);
    u8g.print("STOP");
    u8g.drawLine(0, 63, 22, 63);

    if (recblink == 1) {
      u8g.setPrintPos(30, 54);
      u8g.print("REC.");
    }
    u8g.setPrintPos(65, 54);
    u8g.print(ChronoString);
  }
}

void affiche_bgd(void) {
  //Serial.println(TimeString);

  recblinkcnt++;                           /* Gestion du clignotement 100xperiod */
  recblink = 0;
  if (recblinkcnt < 9) {
    recblink = 1;
  }
  if (recblinkcnt > 18) {
    recblinkcnt = 0;
  }

  // picture loop
  if (picture_finished == 0) {
    /* We can start to write next page */
    u8g.firstPage();
    picture_finished = 1;
  }
  if (picture_finished == 1) {
    durDraw = millis();
    draw();
    Serial.print("TdurDraw=");
    Serial.println(millis() - durDraw);
    if (!u8g.nextPage()) {
      picture_finished = 0;
    }
  }


/*
  do {
    durDraw = millis();
    draw();
    Serial.print("TdurDraw=");
    Serial.println(millis() - durDraw);
  } while ( u8g.nextPage() );
  */
}

