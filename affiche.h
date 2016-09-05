#ifndef AFFICHE_h
#define AFFICHE_h

enum menu_status {
  MENU_INIT,
  MENU_RECORD
};

extern void affiche_init(void);
extern void affiche_bgd(void);
extern byte menu_state;
#endif
