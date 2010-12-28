#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <string.h>

#define TESTDIR  "./testdir"
#define TESTDIR2  "./testdir/testdir2"
#define TESTDIR3  "./testdir/testdir2/testdir3"

#define TESTFILE1 "./testdir/aaa"
#define TESTFILE2 "./testdir/bbb~"

int main(int argc, char *argv[])
{
  char d[1024];
  FILE *a, *b;

  if (mkdir(TESTDIR, 0755) != 0) {
    perror("Error");
    exit(1);
  }

  a = fopen(TESTFILE1, "w");
  if (a == NULL) {
    perror("Error");
    exit(1);
  }
  fclose(a);

  b = fopen(TESTFILE2, "w");
  if (b == NULL) {
    perror("Error");
    exit(1);
  }
  fclose(b);



  return 0;
}

