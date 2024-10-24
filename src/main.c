#include <getopt.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
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
  struct employee_t *employee = NULL;
  struct employee_t *employees = NULL;

  while ((currentArg = getopt(argc, argv, "nf:a")) != -1) {
    switch (currentArg) {
    case 'n':
      newfile = true;
      break;
    case 'f':
      filePath = optarg;
      break;
    case 'a':
      char name[256];
      char address[256];
      unsigned int hours;
      printf("full name\n");
      scanf("%s", name);
      printf("address\n");
      scanf("%s", address);
      printf("hours worked:\n");
      scanf("%d", &hours);
      employee = init_employee(name, address, hours);
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
  employees = read_employees(dbfd, header);
  if (employees == NULL) {
    return STATUS_ERROR;
  }
  if (employee) {
    header->count++;
    if (!realloc(employees, header->count * sizeof(struct employee_t))) {
      printf("reallc failed");
      return STATUS_ERROR;
    }
    add_employee(header->count, employee, employees);
  }
  output_file(dbfd, header);
  return 0;
}
