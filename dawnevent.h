#ifndef DAWNEVENT_H
#define DAWNEVENT_H

struct dawn_event {
  int type;
  int key;
  int x;
  int y;
  int buttons;
};

#define DAWN_EVENT_NONE  0
#define DAWN_EVENT_KEY   1
#define DAWN_EVENT_MOUSE 2
#define DAWN_EVENT_CLOSE 3

#endif
