
#ifndef DAWNAPI_H
#define DAWNAPI_H

/* Dawn event types */
#define DAWN_EVENT_NONE     0
#define DAWN_EVENT_KEY      1
#define DAWN_EVENT_MOUSE    2
#define DAWN_EVENT_CLOSE    3

/* Keyboard and mouse event */
struct dawn_event {
  int type;
  int key;
  int x;
  int y;
  int buttons;
};

/* Window and drawing API */
int dawn_open(char *title, int width, int height);
int dawn_close(void);

int dawn_draw_pixel(int x, int y, int color);
int dawn_fill_rect(int x, int y, int width, int height, int color);
int dawn_draw_text(int x, int y, char *text, int color);

int dawn_poll_event(struct dawn_event *event);
int dawn_present(void);

#endif
