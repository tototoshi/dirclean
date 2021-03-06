/*
  Unixバックアップファイル(*~)を削除する
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <dirent.h>
#include <errno.h>

#define BUFSIZE 1024

#define NORMAL 0
#define RECURSIVELY 1
#define VERBOSE 2

int mode = NORMAL;

extern int optind;
extern int errno;

int main(int argc, char *argv[])
{
  int c;

  while ((c = getopt (argc, argv, "rhv")) != -1) {
    switch (c) {
    case 'r':
      mode = mode | RECURSIVELY;
      break;
    case 'h':
      printf("Usage: dirclean [-r] [directory]\n");
      printf("\t-r:\tcleaning recursively\n");
      printf("\t-v:\tverbose mode\n");
      return 0;
      break;
    case 'v':
      mode = mode | VERBOSE;
      break;
    default:
      break;
    }
  }

  if (mode & VERBOSE & RECURSIVELY) {
    printf("option: recursively cleaning\n");
  }

  if (argc == optind) {
    char cd[BUFSIZE] = "";
    getcwd(cd, sizeof(cd));
    return dirclean(cd);
  } else {
    char dir[BUFSIZE] = "";
    strcpy(dir, argv[optind]);
    if (*(dir + strlen(dir) - 1) == '/') {
      *(dir + strlen(dir) - 1) = 0;
    }
    dirclean(dir);
  }
}

int dirclean(char *dir)
{
  DIR *d_ptr;
  struct dirent *ent;

  d_ptr = opendir(dir);
  if (d_ptr == NULL) {
    if (errno == EACCES) {
      perror("Skip");
      return(1);
    }
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
      char delfile[1024];
      sprintf(delfile, "%s/%s", dir, ent->d_name);
      remove(delfile);
      if (mode & VERBOSE) {
        printf("Delete: %s/%s\n", dir, ent->d_name);
      }
    }
    if ((mode & RECURSIVELY) && ent->d_type == DT_DIR) {
      char child[BUFSIZE];
      sprintf(child, "%s/%s", dir, ent->d_name);
      dirclean(child);
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
