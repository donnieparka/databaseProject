#include <getopt.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

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
  bool list = false;
  bool update = false;
  bool remove = false;
  bool addOne = false;
  int count;
  int dbfd = 0;
  struct dbheader_t *header = NULL;
  struct employee_t *employee = NULL;
  struct employee_t *employees = NULL;

  while ((currentArg = getopt(argc, argv, "nf:alhr")) != -1) {
    switch (currentArg) {
    case 'n':
      newfile = true;
      break;
    case 'f':
      filePath = optarg;
      break;
    case 'a':
      addOne = true;
      break;
    case 'l':
      list = true;
      break;
    case 'h':
      list = true;
      update = true;
      break;
    case 'r':
      remove = true;
      list = true;
      break;
    case '?':
      printf("unknown argument -%c\n", currentArg);
      break;
    default:
      return STATUS_ERROR;
    }
  }
  if (filePath == NULL) {
    print_usage(argv);
  }
  if (newfile) {
    dbfd = createDb(filePath);
    if (dbfd == -1) {
      return STATUS_ERROR;
    }
    header = create_db_header();
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

    employees = read_employees(dbfd, header);
    if (employees == NULL && header->count > 0) {
      printf("failed to read employees");
      perror("read");
      return STATUS_ERROR;
    }
    if (addOne) {
      header->count++;
      employees = realloc_employees(employees, header->count);
      if (employees == NULL) {
        printf("realloc failed in realloc_employees\n");
        return STATUS_ERROR;
      }
      employees = add_employee(employees, header->count);
    }
    if (header->count == 0) {
      printf("non ci sono impiegati da listare o modificare");
      return STATUS_ERROR;
    }
    if (list) {
      for (int i = 0; i < header->count; i++) {
        printf("-----------------------\n");
        printf("impiegato %d: %s\n", i, employees[i].name);
        printf("vive a: %s\n", employees[i].address);
        printf("ha lavorato : %d ore\n", employees[i].hours);
        printf("-----------------------\n");
      }
      if (update) {
        int select;
        int updated_hours;
        select = choose_employee(header->count);
        printf("enter updated hours: \n");
        scanf("%d", &updated_hours);
        employees[select].hours = updated_hours;
      } else if (remove) {
        int select;
        select = choose_employee(header->count);
        bool is_last = select == header->count - 1;

        if (!is_last) {
          for (int i = select; i < header->count - 1; i++) {
            employees[i] = employees[i + 1];
          }
          is_last = false;
        }
        header->count--;
        if (header->count > 0 && !is_last) {
          struct employee_t *new_employees = realloc(employees, header->count * sizeof(struct employee_t));
          if (new_employees == NULL) {
            printf("realloc smaller failed\n");
            perror("realloc");
            free(employees);
            return STATUS_ERROR;
          } else {
            employees = new_employees;
          }
        } else {
          free(employees);
          employees = NULL;
        }
      }
    }
  }
  if (remove) {
    if (ftruncate(dbfd, sizeof(struct dbheader_t) + sizeof(struct employee_t) * header->count) != 0) {
      perror("ftruncate");
    }
  }
  output_file(dbfd, header, employees);
  return 0;
}
