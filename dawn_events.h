
#ifndef DAWN_EVENTS_H
#define DAWN_EVENTS_H

#define DAWN_EVENT_QUEUE_SIZE 32

#define DAWN_EVENT_NONE       0
#define DAWN_EVENT_KEY_DOWN   1
#define DAWN_EVENT_KEY_UP     2
#define DAWN_EVENT_MOUSE_MOVE 3
#define DAWN_EVENT_MOUSE_DOWN 4
#define DAWN_EVENT_MOUSE_UP   5

#define DAWN_MOUSE_LEFT   1
#define DAWN_MOUSE_RIGHT  2
#define DAWN_MOUSE_MIDDLE 3

struct dawn_event {
  int type;
  int keycode;
  int x;
  int y;
  int buttons;
};

struct dawn_event_queue {
  struct dawn_event events[DAWN_EVENT_QUEUE_SIZE];
  int read_pos;
  int write_pos;
  int count;
};

void dawn_events_init(struct dawn_event_queue *queue);
int dawn_events_push(struct dawn_event_queue *queue,
                     struct dawn_event *event);
int dawn_events_pop(struct dawn_event_queue *queue,
                    struct dawn_event *event);

int dawn_input_poll(struct dawn_event_queue *queue);

#endif
