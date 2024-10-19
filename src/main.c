#include <getopt.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include "../include/common.h"
#include "../include/file.h"
#include "../include/parse.h"

void print_usage(char *argv[]) {
  printf("Usage: %s -n -f DbFile\n", argv[0]);
  printf("-n specify if the file has to be created");
  printf("-f (required) specify the Db path");
}
int main(int argc, char *argv[]) {
  if (argc < 3) {
    print_usage(argv);
    return STATUS_ERROR;
  }

  int currentArg;
  char *filePath = NULL;
  bool newfile = false;
  int dbfd;
  struct dbheader_t *header = NULL;

  while ((currentArg = getopt(argc, argv, "nf:")) != -1) {
    switch (currentArg) {
    case 'n':
      newfile = true;
      break;
    case 'f':
      filePath = optarg;
      break;
    case '?':
      printf("unknown argument -%c\n", currentArg);
      break;
    default:
      return STATUS_ERROR;
    }
  }
  if (filePath == NULL) {
    printf("filepath is required");
    print_usage(argv);
  }
  if (newfile) {
    dbfd = createDb(filePath);
    if (dbfd == -1) {
      return STATUS_ERROR;
    }
    header = create_db_header(dbfd);
    if (header == NULL) {
      return STATUS_ERROR;
    }
  } else {
    dbfd = openDb(filePath);
    if (dbfd == -1) {
      return STATUS_ERROR;
    }
    if (validate_db_header(dbfd, &header) == STATUS_ERROR) {
      printf("file validation failed");
      return STATUS_ERROR;
    }
  }
  output_file(dbfd, header);
  return 0;
}
