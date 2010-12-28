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
  getcwd(cd, sizeof(cd));
  return dirclean(cd);
}


int dirclean(char *dir)
{
  DIR *d_ptr;
  struct dirent *ent;

  d_ptr = opendir(dir);
  if (d_ptr == NULL) {
    printf("error\n");
    exit(1);
  }

  while ((ent = readdir(d_ptr)) != NULL) {
    if (ent->d_type != DT_REG) continue;
    if (is_unix_backup(ent->d_name) == 0) {
      remove(ent->d_name);
      printf("Delete: %s\n", ent->d_name);
    }
  }

  return(0);
}

int is_unix_backup(char *f)
{
  if (*(f + (int) strlen(f) - 1) == '~') {
    return 0;
  }
  return 1;
}
