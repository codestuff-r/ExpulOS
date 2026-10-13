#include "types.h"
#include "dawn.h"

static void
bevel(int x, int y, int w, int h)
{
  vga_fillrect(x, y, w, h, DAWN_LIGHT_GREY);
  vga_fillrect(x, y, w, 2, DAWN_WHITE);
  vga_fillrect(x, y, 2, h, DAWN_WHITE);
  vga_fillrect(x, y + h - 2, w, 2, DAWN_DARK_GREY);
  vga_fillrect(x + w - 2, y, 2, h, DAWN_DARK_GREY);
}

void
dawn_render_desktop(void)
{
  /* Deep blue desktop */
  vga_fillrect(0, 0, 320, 200, DAWN_BLUE);

  /* Subtle horizontal highlights */
  vga_fillrect(0, 0, 320, 1, DAWN_LIGHT_BLUE);
  vga_fillrect(0, 1, 320, 1, DAWN_CYAN);

  /* Center plaque shadow */
  vga_fillrect(87, 81, 150, 38, DAWN_BLACK);

  /* Raised Powered by XV6 plaque */
  bevel(83, 77, 150, 38);
  vga_fillrect(86, 80, 144, 32, DAWN_BLUE);
  vga_fillrect(86, 80, 144, 1, DAWN_LIGHT_CYAN);

  dawn_draw_string(119, 92, "POWERED BY XV6",
                   DAWN_WHITE, DAWN_BLUE);

  /* Bottom panel shadow */
  vga_fillrect(0, 180, 320, 20, DAWN_BLACK);

  /* Bottom panel */
  vga_fillrect(0, 182, 320, 18, DAWN_LIGHT_GREY);
  vga_fillrect(0, 182, 320, 2, DAWN_WHITE);
  vga_fillrect(0, 184, 320, 1, DAWN_LIGHT_CYAN);
  vga_fillrect(0, 199, 320, 1, DAWN_DARK_GREY);

  /* Start/menu button */
  bevel(2, 185, 44, 13);
  dawn_draw_string(7, 188, "DAWN",
                   DAWN_BLACK, DAWN_LIGHT_GREY);

  /* Separator */
  vga_fillrect(50, 186, 1, 10, DAWN_DARK_GREY);
  vga_fillrect(51, 186, 1, 10, DAWN_WHITE);

  /* Desktop label */
  dawn_draw_string(58, 188, "DESKTOP",
                   DAWN_BLACK, DAWN_LIGHT_GREY);
}

static void
draw_menu(void)
{
  /* Menu shadow */
  vga_fillrect(5, 104, 116, 81, DAWN_BLACK);

  bevel(2, 101, 116, 81);

  vga_fillrect(5, 104, 110, 12, DAWN_BLUE);
  dawn_draw_string(9, 106, "DAWN MENU",
                   DAWN_WHITE, DAWN_BLUE);

  vga_fillrect(5, 118, 110, 17, DAWN_LIGHT_GREY);
  dawn_draw_string(10, 122, "TERMINAL",
                   DAWN_BLACK, DAWN_LIGHT_GREY);

  vga_fillrect(5, 136, 110, 17, DAWN_LIGHT_GREY);
  dawn_draw_string(10, 140, "THISFILE",
                   DAWN_BLACK, DAWN_LIGHT_GREY);

  vga_fillrect(5, 154, 110, 17, DAWN_LIGHT_GREY);
  dawn_draw_string(10, 158, "EXIT DAWN",
                   DAWN_BLACK, DAWN_LIGHT_GREY);
}

static void
draw_close_button(void)
{
  bevel(269, 32, 13, 14);
  dawn_draw_string(272, 35, "X",
                   DAWN_BLACK, DAWN_LIGHT_GREY);
}

static void
draw_terminal(char *line, char *status,
              char output[6][16])
{
  int i;

  /* Window shadow */
  vga_fillrect(34, 29, 260, 150, DAWN_BLACK);

  bevel(30, 25, 260, 150);

  vga_fillrect(34, 29, 252, 20, DAWN_BLUE);
  dawn_draw_string(40, 35, "TERMINAL",
                   DAWN_WHITE, DAWN_BLUE);
  draw_close_button();

  vga_fillrect(36, 51, 248, 116, DAWN_BLACK);

  dawn_draw_string(43, 55, "EXPULOS DAWN",
                   DAWN_GREEN, DAWN_BLACK);
  dawn_draw_string(43, 65, status,
                   DAWN_WHITE, DAWN_BLACK);

  for(i = 0; i < 5; i++)
    dawn_draw_string(43, 77 + i * 10, output[i],
                     DAWN_WHITE, DAWN_BLACK);

  dawn_draw_string(43, 135, ">",
                   DAWN_GREEN, DAWN_BLACK);
  dawn_draw_string(59, 135, line,
                   DAWN_WHITE, DAWN_BLACK);

  vga_fillrect(36, 151, 248, 16, DAWN_LIGHT_GREY);
  vga_fillrect(36, 151, 248, 1, DAWN_WHITE);
  dawn_draw_string(42, 155, "DAWN TERMINAL",
                   DAWN_BLACK, DAWN_LIGHT_GREY);
}

static void
draw_thisfile(char output[6][16])
{
  int i;

  /* Window shadow */
  vga_fillrect(34, 29, 260, 150, DAWN_BLACK);

  bevel(30, 25, 260, 150);

  vga_fillrect(34, 29, 252, 20, DAWN_BLUE);
  dawn_draw_string(40, 35, "THISFILE",
                   DAWN_WHITE, DAWN_BLUE);
  draw_close_button();

  vga_fillrect(36, 51, 248, 116, DAWN_WHITE);
  dawn_draw_string(43, 55, "FILES IN /",
                   DAWN_BLUE, DAWN_WHITE);

  for(i = 0; i < 6; i++)
    dawn_draw_string(47, 70 + i * 13, output[i],
                     DAWN_BLACK, DAWN_WHITE);

  vga_fillrect(36, 151, 248, 16, DAWN_LIGHT_GREY);
  vga_fillrect(36, 151, 248, 1, DAWN_WHITE);
  dawn_draw_string(42, 155, "XV6 FILE MANAGER",
                   DAWN_BLACK, DAWN_LIGHT_GREY);
}

static void
draw_mouse(int x, int y)
{
  vga_fillrect(x, y, 2, 12, DAWN_BLACK);
  vga_fillrect(x, y, 8, 2, DAWN_BLACK);
  vga_fillrect(x + 2, y + 2, 2, 7, DAWN_WHITE);
  vga_fillrect(x + 2, y + 2, 5, 2, DAWN_WHITE);
}

void
dawn_render_scene(int menu_open, int terminal_open,
                  int filemanager_open, char *line,
                  char *status, char output[6][16],
                  int mx, int my)
{
  dawn_render_desktop();

  if(terminal_open)
    draw_terminal(line, status, output);

  if(filemanager_open)
    draw_thisfile(output);

  if(menu_open)
    draw_menu();

  draw_mouse(mx, my);
}
