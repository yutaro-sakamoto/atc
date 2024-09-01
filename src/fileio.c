
#include <sys/stat.h>
#include <dirent.h>
#include <stdio.h>

int create_dir(char *dir) {
  // TODO: if necessary, dispatch the following process due to the OS
  return mkdir(dir, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH) == 0;
}

int remove_dir(char *dir) {
  // TODO: if necessary, dispatch the following process due to the OS
  return remove(dir) == 0;
}