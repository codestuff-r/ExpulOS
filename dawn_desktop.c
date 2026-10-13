
#include "types.h"
#include "param.h"
#include "x86.h"
#include "spinlock.h"
#include "sleeplock.h"
#include "fs.h"
#include "file.h"
#include "stat.h"
#include "defs.h"
#include "dawn.h"

extern void dawn_input_init(void);
extern void dawn_input_poll(int *, int *, int *, int *);

extern void dawn_render_scene(
    int, int, int, char *, char *, char [6][16], int, int
);

static char input_line[29];
static int input_length;
static char status_text[32];
static char output_lines[6][16];

static int
dawn_streq(char *a, char *b)
{
  while(*a && *a == *b){
    a++;
    b++;
  }
  return *a == *b;
}

static void
set_status(char *s)
{
  int i;

  for(i = 0; i < 31 && s[i]; i++)
    status_text[i] = s[i];

  status_text[i] = 0;
}

static void
clear_output(void)
{
  int i, j;

  for(i = 0; i < 6; i++)
    for(j = 0; j < 16; j++)
      output_lines[i][j] = 0;
}

static void
set_output(int row, char *s)
{
  int i;

  if(row < 0 || row >= 6)
    return;

  for(i = 0; i < 15 && s[i]; i++)
    output_lines[row][i] = s[i];

  output_lines[row][i] = 0;
}

static void
list_root(void)
{
  struct inode *ip;
  struct dirent de;
  char name[DIRSIZ + 1];
  uint off;
  int row, i, n;

  clear_output();

  ip = namei("/");
  if(ip == 0){
    set_output(0, "ROOT UNAVAILABLE");
    return;
  }

  ilock(ip);

  if(ip->type != T_DIR){
    iunlockput(ip);
    set_output(0, "NOT A DIRECTORY");
    return;
  }

  row = 0;

  for(off = 0; off < ip->size; off += sizeof(de)){
    n = readi(ip, (char *)&de, off, sizeof(de));

    if(n != sizeof(de))
      break;

    if(de.inum == 0)
      continue;

    for(i = 0; i < DIRSIZ; i++)
      name[i] = de.name[i];

    name[DIRSIZ] = 0;

    if(name[0] == '.' &&
       (name[1] == 0 ||
        (name[1] == '.' && name[2] == 0)))
      continue;

    if(row < 6){
      set_output(row++, name);
    } else {
      set_output(5, "MORE FILES...");
      break;
    }
  }

  iunlockput(ip);

  if(row == 0)
    set_output(0, "(EMPTY)");
}

static int
run_command(void)
{
  clear_output();

  if(dawn_streq(input_line, "HELP")){
    set_output(0, "HELP - COMMANDS");
    set_output(1, "LS");
    set_output(2, "CLEAR");
    set_output(3, "THISFILE");
    set_output(4, "HELP");
    set_output(5, "EXIT");
    set_status("COMMANDS AVAILABLE");
  } else if(dawn_streq(input_line, "LS")){
    list_root();
    set_status("ROOT DIRECTORY");
  } else if(dawn_streq(input_line, "CLEAR")){
    set_status("TERMINAL READY");
  } else if(dawn_streq(input_line, "THISFILE")){
    list_root();
    set_status("THISFILE");
    input_line[0] = 0;
    input_length = 0;
    return 1;
  } else if(input_length != 0){
    set_output(0, "UNKNOWN COMMAND");
    set_output(1, "TYPE HELP");
    set_status("COMMAND NOT FOUND");
  } else {
    set_status("TERMINAL READY");
  }

  input_line[0] = 0;
  input_length = 0;
  return 0;
}

void
dawn_main(void)
{
  int key, mx, my, buttons;
  int oldx = -1, oldy = -1, oldbuttons = 0;
  int menu_open = 0;
  int terminal_open = 0;
  int filemanager_open = 0;
  int changed, clicked;

  input_length = 0;
  input_line[0] = 0;

  clear_output();
  set_status("TERMINAL READY");

  if(vga_set_mode(13) < 0)
    return;

  dawn_input_init();

  /* Draw the initial desktop immediately. */
  dawn_render_scene(0, 0, 0, input_line,
                    status_text, output_lines, 160, 100);

  for(;;){
    dawn_input_poll(&key, &mx, &my, &buttons);
    changed = 0;
    clicked = (buttons & 1) && !(oldbuttons & 1);

    if(terminal_open && key != 0){
      if(key == '\b' && input_length > 0){
        input_line[--input_length] = 0;
        changed = 1;
      } else if(key == '\n'){
        if(dawn_streq(input_line, "EXIT")){
          vga_set_mode(3);
          return;
        }

        if(run_command()){
          terminal_open = 0;
          filemanager_open = 1;
        }

        changed = 1;
      } else if(key >= 'A' && key <= 'Z' &&
                input_length < 28){
        input_line[input_length++] = key;
        input_line[input_length] = 0;
        set_status("TERMINAL READY");
        changed = 1;
      } else if(key == ' ' && input_length < 28){
        input_line[input_length++] = ' ';
        input_line[input_length] = 0;
        changed = 1;
      }
    }

    if(clicked){
      /* Launcher button */
      if(my >= 183 && mx <= 47){
        menu_open = !menu_open;
        changed = 1;
      }
      /* Close whichever application is open */
      else if((terminal_open || filemanager_open) &&
              mx >= 268 && mx <= 283 &&
              my >= 30 && my <= 48){
        terminal_open = 0;
        filemanager_open = 0;
        changed = 1;
      }
      /* Launch terminal */
      else if(menu_open &&
              mx >= 5 && mx <= 115 &&
              my >= 118 && my <= 135){
        terminal_open = 1;
        filemanager_open = 0;
        menu_open = 0;
        set_status("TERMINAL READY");
        changed = 1;
      }
      /* Launch ThisFile */
      else if(menu_open &&
              mx >= 5 && mx <= 115 &&
              my >= 136 && my <= 153){
        list_root();
        terminal_open = 0;
        filemanager_open = 1;
        menu_open = 0;
        changed = 1;
      }
      /* Return to xv6 shell */
      else if(menu_open &&
              mx >= 5 && mx <= 115 &&
              my >= 154 && my <= 172){
        vga_set_mode(3);
        return;
      }
      else if(menu_open){
        menu_open = 0;
        changed = 1;
      }
    }

    if(mx != oldx || my != oldy ||
       buttons != oldbuttons)
      changed = 1;

    if(changed)
      dawn_render_scene(menu_open, terminal_open,
                        filemanager_open, input_line,
                        status_text, output_lines, mx, my);

    oldx = mx;
    oldy = my;
    oldbuttons = buttons;
  }
}
