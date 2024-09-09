
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

int create_dir(char *dir) {
  // TODO: if necessary, dispatch the following process due to the OS
  return mkdir(dir, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH) == 0;
}

int remove_dir(char *dir) {
  // TODO: if necessary, dispatch the following process due to the OS
  char *cmd_head = "rm -rf ";
  char *cmd = malloc(strlen(cmd_head) + strlen(dir) + 1);
  sprintf(cmd, "%s%s", cmd_head, dir);
  return system(cmd) == 0;
}