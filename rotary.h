#ifndef ROTARY_h
#define ROTARY_h

#define NONE        0x00
#define INCREMENT   0x01
#define DECREMENT   0x02
#define UNPUSHED    0x10
#define PUSHED      0x20
#define SELECT_MASK 0xF0
#define ROT_MASK    0x0F

extern void rotary_init(void);
extern void rotary_bgd(void);
extern byte rotary_state;
extern byte rotary_select_changed;
#endif
