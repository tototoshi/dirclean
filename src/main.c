/*
  カレントディレクトリのUnixバックアップファイル(*~)を削除する
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <dirent.h>

int main(int argc, char *argv[])
{
  char cd[1024] = "";

  if (argc == 1) {
    getcwd(cd, sizeof(cd));
    return dirclean(cd);
  } else {
    dirclean(argv[1]);
  }
}

int dirclean(char *dir)
{
  DIR *d_ptr;
  struct dirent *ent;

  d_ptr = opendir(dir);
  if (d_ptr == NULL) {
    perror("Error");
    exit(1);
  }

  while ((ent = readdir(d_ptr)) != NULL) {
    if (strcmp(ent->d_name, ".") == 0 ||
        strcmp(ent->d_name, "..") == 0) {
      continue;
    }
    if (ent->d_type == DT_REG &&
        is_unix_backup(ent->d_name) == 0) {
      remove(ent->d_name);
      printf("Delete: %s/%s\n", dir, ent->d_name);
    }
    if (ent->d_type == DT_DIR) {
      {
        char child[1024];
        strcpy(child, dir);
        strcat(child, "/");
        strcat(child, ent->d_name);
        dirclean(child);
      }
    }
  }

  closedir(d_ptr);
  return(0);
}

int is_unix_backup(char *f)
{
  if (*(f + (int) strlen(f) - 1) == '~') {
    return 0;
  }
  return 1;
}









