
#include "types.h"
#include "x86.h"

static int mouse_x = 160;
static int mouse_y = 100;
static int mouse_buttons;
static uchar mouse_packet[3];
static int mouse_cycle;

static void
wait_input(void)
{
  int i;

  for(i = 0; i < 100000; i++)
    if((inb(0x64) & 2) == 0)
      return;
}

static void
mouse_command(uchar command)
{
  wait_input();
  outb(0x64, 0xD4);
  wait_input();
  outb(0x60, command);
}

void
dawn_input_init(void)
{
  mouse_x = 160;
  mouse_y = 100;
  mouse_buttons = 0;
  mouse_cycle = 0;

  wait_input();
  outb(0x64, 0xA8);

  mouse_command(0xF6);
  mouse_command(0xF4);
}

static char
key_to_char(uchar sc)
{
  switch(sc){
  case 0x1E: return 'A';
  case 0x30: return 'B';
  case 0x2E: return 'C';
  case 0x20: return 'D';
  case 0x12: return 'E';
  case 0x21: return 'F';
  case 0x22: return 'G';
  case 0x23: return 'H';
  case 0x17: return 'I';
  case 0x24: return 'J';
  case 0x25: return 'K';
  case 0x26: return 'L';
  case 0x32: return 'M';
  case 0x31: return 'N';
  case 0x18: return 'O';
  case 0x19: return 'P';
  case 0x10: return 'Q';
  case 0x13: return 'R';
  case 0x1F: return 'S';
  case 0x14: return 'T';
  case 0x16: return 'U';
  case 0x2F: return 'V';
  case 0x11: return 'W';
  case 0x2D: return 'X';
  case 0x15: return 'Y';
  case 0x2C: return 'Z';
  case 0x39: return ' ';
  case 0x1C: return '\n';
  case 0x0E: return '\b';
  default: return 0;
  }
}

void
dawn_input_poll(int *key, int *mx, int *my, int *buttons)
{
  uchar status;
  uchar data;
  char c;
  int dx, dy;

  *key = 0;

  for(;;){
    status = inb(0x64);
    if((status & 1) == 0)
      break;

    data = inb(0x60);

    if(status & 0x20){
      if(mouse_cycle == 0){
        if(data & 0x08){
          mouse_packet[0] = data;
          mouse_cycle = 1;
        }
      } else if(mouse_cycle == 1){
        mouse_packet[1] = data;
        mouse_cycle = 2;
      } else {
        mouse_packet[2] = data;
        mouse_cycle = 0;

        if((mouse_packet[0] & 0xC0) == 0){
          dx = (int)(char)mouse_packet[1];
          dy = (int)(char)mouse_packet[2];

          mouse_x += dx;
          mouse_y -= dy;

          if(mouse_x < 0) mouse_x = 0;
          if(mouse_x > 311) mouse_x = 311;
          if(mouse_y < 0) mouse_y = 0;
          if(mouse_y > 188) mouse_y = 188;

          mouse_buttons = mouse_packet[0] & 7;
        }
      }
    } else if((data & 0x80) == 0 && *key == 0){
      c = key_to_char(data);
      if(c != 0)
        *key = c;
    }

    if(*key != 0)
      break;
  }

  *mx = mouse_x;
  *my = mouse_y;
  *buttons = mouse_buttons;
}
