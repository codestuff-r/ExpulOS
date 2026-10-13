
#ifndef DAWN_H
#define DAWN_H

/* Display */
#define DAWN_SCREEN_WIDTH  320
#define DAWN_SCREEN_HEIGHT 200

/* Window manager */
#define DAWN_MAX_WINDOWS 8
#define DAWN_TITLE_HEIGHT 16
#define DAWN_BORDER_SIZE 2

struct dawn_window {
  int x;
  int y;
  int width;
  int height;
  int background;
  int visible;
  char title[32];
};

/* Dawn API events */
#define DAWN_EVENT_NONE  0
#define DAWN_EVENT_KEY   1
#define DAWN_EVENT_MOUSE 2
#define DAWN_EVENT_CLOSE 3

struct dawn_event {
  int type;
  int key;
  int x;
  int y;
  int buttons;
};

/* VGA palette indices */
#define DAWN_BLACK          0
#define DAWN_BLUE           1
#define DAWN_GREEN          2
#define DAWN_CYAN           3
#define DAWN_RED            4
#define DAWN_MAGENTA        5
#define DAWN_BROWN          6
#define DAWN_LIGHT_GREY     7
#define DAWN_DARK_GREY      8
#define DAWN_LIGHT_BLUE     9
#define DAWN_LIGHT_GREEN   10
#define DAWN_LIGHT_CYAN    11
#define DAWN_LIGHT_RED     12
#define DAWN_LIGHT_MAGENTA 13
#define DAWN_YELLOW        14
#define DAWN_WHITE         15

/* VGA driver */
int vga_set_mode(int mode);
int vga_putpixel(int x, int y, int color);
int vga_fillrect(int x, int y, int width, int height, int color);

/* Text rendering */
void dawn_draw_char(int x, int y, char c, int fg, int bg);
void dawn_draw_string(int x, int y, char *s, int fg, int bg);

/* Desktop */
void dawn_main(void);
void dawn_render_desktop(void);
void dawn_render_scene(int menu_open, int terminal_open,
                       int filemanager_open, char *line,
                       char *status, char output[6][16],
                       int mx, int my);

/* Window manager */
int dawn_wm_create(int x, int y, int width, int height,
                   char *title, int background);
int dawn_wm_get_active(void);
struct dawn_window *dawn_wm_get_window(int id);
int dawn_wm_set_active(int id);
int dawn_wm_move(int id, int x, int y);
int dawn_wm_show(int id, int visible);
int dawn_wm_count(void);
int dawn_wm_hit_test(int x, int y);
int dawn_wm_title_hit(int id, int x, int y);

/* Application discovery */
int dawn_apps_scan(void);
int dawn_apps_count(void);
char *dawn_apps_get(int index);

#endif
