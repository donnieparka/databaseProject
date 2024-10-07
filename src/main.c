#include <getopt.h>
#include <stdbool.h>
#include <stdio.h>

#include "../include/common.h"
#include "../include/file.h"
#include "../include/parse.h"

int main(int argc, char *argv[]) {
  if (argc < 3) {
    printf("Usage: -f filepath\n");
    return -1;
  }
  int currentArg;
  char *filePath;
  bool newfile;
  int dbfd;
  while ((currentArg = getopt(argc, argv, "nf:")) != -1) {
    switch (currentArg) {
    case 'n':
      newfile = true;
      break;
    case 'f':
      filePath = optarg;
      break;
    case '?':
      printf("unknown argument %c\n", currentArg);
      break;
    default:
      return -1;
    }
  }
  if (newfile) {
    dbfd = createDb(filePath);
    if (dbfd == -1) {
      printf("can't create database\n");
      return -1;
    } else {
      printf("database created\n");
    }
  } else {
    dbfd = openDb(filePath);
    if (dbfd == -1) {
      printf("can't open database\n");
      return -1;
    } else {
      printf("database opened\n");
    }
  }

  return 0;
}
