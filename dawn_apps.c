
#include "types.h"
#include "param.h"
#include "spinlock.h"
#include "sleeplock.h"
#include "fs.h"
#include "file.h"
#include "stat.h"
#include "defs.h"
#include "dawn.h"

#define DAWN_MAX_APPS 16

static char app_names[DAWN_MAX_APPS][DIRSIZ + 1];
static int app_count;

static int
dawn_app_name_matches(char *name)
{
  int len = 0;

  while(len < DIRSIZ && name[len])
    len++;

  return len >= 3 &&
         name[0] == 'D' &&
         name[1] == 'F' &&
         name[len - 1] == 'm';
}

int
dawn_apps_scan(void)
{
  struct inode *root;
  struct dirent de;
  char name[DIRSIZ + 1];
  uint off;
  int i, n, len;

  app_count = 0;

  root = namei("/");
  if(root == 0)
    return 0;

  ilock(root);

  if(root->type != T_DIR){
    iunlockput(root);
    return 0;
  }

  for(off = 0; off < root->size &&
      app_count < DAWN_MAX_APPS;
      off += sizeof(de)){
    n = readi(root, (char *)&de, off, sizeof(de));

    if(n != sizeof(de))
      break;

    if(de.inum == 0)
      continue;

    for(i = 0; i < DIRSIZ; i++)
      name[i] = de.name[i];

    name[DIRSIZ] = 0;

    if(!dawn_app_name_matches(name))
      continue;

    /* Confirm the entry is a regular file. */
    {
      struct inode *ip = dirlookup(root, name, 0);

      if(ip == 0)
        continue;

      ilock(ip);
      i = ip->type == T_FILE;
      iunlockput(ip);

      if(!i)
        continue;
    }

    len = 0;
    while(len < DIRSIZ && name[len])
      len++;

    for(i = 0; i <= len; i++)
      app_names[app_count][i] = name[i];

    app_count++;
  }

  iunlockput(root);
  return app_count;
}

int
dawn_apps_count(void)
{
  return app_count;
}

char *
dawn_apps_get(int index)
{
  if(index < 0 || index >= app_count)
    return 0;

  return app_names[index];
}
