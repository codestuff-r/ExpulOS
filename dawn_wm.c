
#include "types.h"
#include "dawn.h"

static struct dawn_window windows[DAWN_MAX_WINDOWS];
static int window_count;
static int active_window = -1;

int
dawn_wm_create(int x, int y, int width, int height,
               char *title, int background)
{
  int i, j;

  if(window_count >= DAWN_MAX_WINDOWS ||
     width < 32 || height < 24)
    return -1;

  if(width > DAWN_SCREEN_WIDTH)
    width = DAWN_SCREEN_WIDTH;
  if(height > DAWN_SCREEN_HEIGHT)
    height = DAWN_SCREEN_HEIGHT;

  if(x < 0)
    x = 0;
  if(y < 0)
    y = 0;
  if(x + width > DAWN_SCREEN_WIDTH)
    x = DAWN_SCREEN_WIDTH - width;
  if(y + height > DAWN_SCREEN_HEIGHT)
    y = DAWN_SCREEN_HEIGHT - height;

  i = window_count++;

  windows[i].x = x;
  windows[i].y = y;
  windows[i].width = width;
  windows[i].height = height;
  windows[i].background = background;
  windows[i].visible = 1;

  for(j = 0; j < (int)sizeof(windows[i].title) - 1 && title[j]; j++)
    windows[i].title[j] = title[j];
  windows[i].title[j] = '\0';

  active_window = i;
  return i;
}

int
dawn_wm_get_active(void)
{
  return active_window;
}

struct dawn_window *
dawn_wm_get_window(int id)
{
  if(id < 0 || id >= window_count)
    return 0;

  return &windows[id];
}

int
dawn_wm_set_active(int id)
{
  if(id < 0 || id >= window_count || !windows[id].visible)
    return -1;

  active_window = id;
  return 0;
}

int
dawn_wm_move(int id, int x, int y)
{
  struct dawn_window *w;

  w = dawn_wm_get_window(id);
  if(w == 0)
    return -1;

  if(x < 0)
    x = 0;
  if(y < 0)
    y = 0;

  if(x + w->width > DAWN_SCREEN_WIDTH)
    x = DAWN_SCREEN_WIDTH - w->width;
  if(y + w->height > DAWN_SCREEN_HEIGHT)
    y = DAWN_SCREEN_HEIGHT - w->height;

  w->x = x;
  w->y = y;
  return 0;
}

int
dawn_wm_show(int id, int visible)
{
  if(id < 0 || id >= window_count)
    return -1;

  windows[id].visible = visible != 0;

  if(!windows[id].visible && active_window == id)
    active_window = -1;

  return 0;
}

int
dawn_wm_count(void)
{
  return window_count;
}

// Return the topmost visible window at a point.
// Windows created later are treated as being on top.
int
dawn_wm_hit_test(int x, int y)
{
  int i;
  struct dawn_window *w;

  for(i = window_count - 1; i >= 0; i--){
    w = &windows[i];

    if(!w->visible)
      continue;

    if(x >= w->x && x < w->x + w->width &&
       y >= w->y && y < w->y + w->height)
      return i;
  }

  return -1;
}

// Return 1 if the point is inside a window's title bar.
int
dawn_wm_title_hit(int id, int x, int y)
{
  struct dawn_window *w;

  w = dawn_wm_get_window(id);
  if(w == 0 || !w->visible)
    return 0;

  return x >= w->x + DAWN_BORDER_SIZE &&
         x < w->x + w->width - DAWN_BORDER_SIZE &&
         y >= w->y + DAWN_BORDER_SIZE &&
         y < w->y + DAWN_BORDER_SIZE + DAWN_TITLE_HEIGHT;
}
