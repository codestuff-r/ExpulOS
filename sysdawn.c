
#include "types.h"
#include "defs.h"
#include "param.h"
#include "memlayout.h"
#include "mmu.h"
#include "proc.h"
#include "x86.h"
#include "dawn.h"

/*
 * Initial Dawn syscall backend.
 *
 * Drawing calls use the existing VGA primitives.
 * Window/event state is not yet implemented here.
 */

static int
dawn_valid_color(int color)
{
  return color >= DAWN_BLACK && color <= DAWN_WHITE;
}

int
sys_dawn_open(void)
{
  char *title;
  int width, height;

  if(argstr(0, &title) < 0 ||
     argint(1, &width) < 0 ||
     argint(2, &height) < 0)
    return -1;

  if(width < 32 || height < 24 ||
     width > DAWN_SCREEN_WIDTH ||
     height > DAWN_SCREEN_HEIGHT)
    return -1;

  /*
   * The window manager exists, but its drawing path is not
   * yet connected to the user API. Avoid creating a window
   * that applications cannot reliably draw into.
   */
  return -1;
}

int
sys_dawn_close(void)
{
  /* Requires per-process window ownership, not implemented yet. */
  return -1;
}

int
sys_dawn_draw_pixel(void)
{
  int x, y, color;

  if(argint(0, &x) < 0 ||
     argint(1, &y) < 0 ||
     argint(2, &color) < 0)
    return -1;

  if(x < 0 || x >= DAWN_SCREEN_WIDTH ||
     y < 0 || y >= DAWN_SCREEN_HEIGHT ||
     !dawn_valid_color(color))
    return -1;

  return vga_putpixel(x, y, color);
}

int
sys_dawn_fill_rect(void)
{
  int x, y, width, height, color;

  if(argint(0, &x) < 0 ||
     argint(1, &y) < 0 ||
     argint(2, &width) < 0 ||
     argint(3, &height) < 0 ||
     argint(4, &color) < 0)
    return -1;

  if(width <= 0 || height <= 0 ||
     x < 0 || y < 0 ||
     x >= DAWN_SCREEN_WIDTH ||
     y >= DAWN_SCREEN_HEIGHT ||
     width > DAWN_SCREEN_WIDTH - x ||
     height > DAWN_SCREEN_HEIGHT - y ||
     !dawn_valid_color(color))
    return -1;

  return vga_fillrect(x, y, width, height, color);
}

int
sys_dawn_draw_text(void)
{
  int x, y, color;
  char *text;

  if(argint(0, &x) < 0 ||
     argint(1, &y) < 0 ||
     argstr(2, &text) < 0 ||
     argint(3, &color) < 0)
    return -1;

  if(x < 0 || y < 0 ||
     x >= DAWN_SCREEN_WIDTH ||
     y >= DAWN_SCREEN_HEIGHT ||
     !dawn_valid_color(color))
    return -1;

  /*
   * The existing text renderer takes a foreground AND
   * background color. Until the API defines clipping and
   * background behavior, do not draw directly to the screen.
   */
  return -1;
}

int
sys_dawn_poll_event(void)
{
  char *user_event;

  if(argptr(0, &user_event, sizeof(struct dawn_event)) < 0)
    return -1;

  /* Input event queue is not implemented yet. */
  return -1;
}

int
sys_dawn_present(void)
{
  /*
   * Presentation requires a per-window back buffer.
   * Direct-to-VGA drawing has no buffer to present.
   */
  return -1;
}
