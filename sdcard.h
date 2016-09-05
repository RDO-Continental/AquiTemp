#ifndef SDCARD_h
#define SDCARD_h

enum sdcards_status {
  SDCARD_MISSING,
  SDCARD_AVAILABLE,
  SDCARD_NEWFILE_ERROR
};

extern void sdcard_init(void);
extern void sdcard_start_rec(char * tab_header[], byte nb_params);
extern void sdcard_stop_rec(void);
extern void sdcard_save_data(void);
extern void sdcard_bgd(void);
extern byte sdcard_status;
#endif

