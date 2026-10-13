
#include "types.h"
#include "dawn_events.h"

void
dawn_events_init(struct dawn_event_queue *queue)
{
  int i;

  queue->read_pos = 0;
  queue->write_pos = 0;
  queue->count = 0;

  for(i = 0; i < DAWN_EVENT_QUEUE_SIZE; i++){
    queue->events[i].type = DAWN_EVENT_NONE;
    queue->events[i].keycode = 0;
    queue->events[i].x = 0;
    queue->events[i].y = 0;
    queue->events[i].buttons = 0;
  }
}

int
dawn_events_push(struct dawn_event_queue *queue,
                 struct dawn_event *event)
{
  if(queue->count >= DAWN_EVENT_QUEUE_SIZE)
    return -1;

  queue->events[queue->write_pos] = *event;
  queue->write_pos =
    (queue->write_pos + 1) % DAWN_EVENT_QUEUE_SIZE;
  queue->count++;

  return 0;
}

int
dawn_events_pop(struct dawn_event_queue *queue,
                struct dawn_event *event)
{
  if(queue->count == 0)
    return -1;

  *event = queue->events[queue->read_pos];
  queue->read_pos =
    (queue->read_pos + 1) % DAWN_EVENT_QUEUE_SIZE;
  queue->count--;

  return 0;
}
