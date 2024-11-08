#include <arpa/inet.h>
#include <malloc.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

#include "../include/common.h"
#include "../include/parse.h"

struct dbheader_t *create_db_header() {
  struct dbheader_t *header = calloc(1, sizeof(struct dbheader_t));
  if (header == NULL) {
    perror("calloc");
    printf("calloc failed\n");
    return NULL;
  }
  header->count = 0;
  header->version = 0x1;
  header->filesize = sizeof(struct dbheader_t);
  header->magic = HEADER_MAGIC;
  printf("header created\n");
  return header;
}

int validate_db_header(int fd, struct dbheader_t **header_out) {
  struct dbheader_t *header = calloc(1, sizeof(struct dbheader_t));
  if (header == NULL) {
    printf("calloc failed to create header validation space\n");
    perror("calloc");
    return STATUS_ERROR;
  }

  int read_success = read(fd, header, sizeof(struct dbheader_t));
  if (read_success != sizeof(struct dbheader_t)) {
    perror("read");
    printf("can not read for validation\n");
    free(header);
    return STATUS_ERROR;
  }

  header->version = ntohs(header->version);
  header->count = ntohs(header->count);
  header->magic = ntohl(header->magic);
  header->filesize = ntohl(header->filesize);

  struct stat h_stat = {0};
  fstat(fd, &h_stat);
  if (header->filesize != h_stat.st_size) {
    printf("filesize mismatch \n");
    free(header);
    return STATUS_ERROR;
  }
  if (header->version != 1) {
    printf("database version wrong\n");
    free(header);
    return STATUS_ERROR;
  }
  printf("validation ok\n");
  *header_out = header;
  return STATUS_SUCCESS;
}

struct employee_t *read_employees(int fd, struct dbheader_t *dbhdr) {
  unsigned short count = dbhdr->count;
  if (count == 0) {
    return NULL; // Return empty array for new files
  }

  struct employee_t *employees = calloc(count, sizeof(struct employee_t));
  if (employees == NULL) {
    perror("calloc");
    printf("calloc failed");
    return NULL;
  }
  int read_success = read(fd, employees, count * sizeof(struct employee_t));
  if (read_success == -1) {
    perror("read");
    return NULL;
  }
  for (int i = 0; i < count; i++) {
    employees[i].hours = ntohl(employees[i].hours);
  }
  return employees;
}

struct employee_t *add_employee(struct employee_t *employees, int count) {
  char address[256];
  char name[256];
  unsigned int hours;

  printf("full name\n");
  if (scanf("%255s", name) != 1) {
    fprintf(stderr, "Errore nella lettura del nome\n");
    return NULL;
  }

  printf("address\n");
  if (scanf(" %255s", address) != 1) {
    fprintf(stderr, "Errore nella lettura dell'indirizzo\n");
    return NULL;
  }

  printf("hours worked:\n");
  if (scanf(" %d", &hours) != 1) {
    fprintf(stderr, "Errore nella lettura delle ore\n");
    return NULL;
  }
  struct employee_t *new_employee = &employees[count - 1];
  strcpy(new_employee->name, name);
  strcpy(new_employee->address, address);
  new_employee->hours = hours;
  return employees;
}

struct employee_t *realloc_employees(struct employee_t *employees, int count) {
  struct employee_t *new_employees = realloc(employees, count * sizeof(struct employee_t));
  if (new_employees == NULL) {
    perror("realloc");
    free(employees);
    return NULL;
  }
  return new_employees;
}

int choose_employee(int count) {
  int select;
  printf("quale dipendente vuoi aggiornare?\n");
  scanf("%d", &select);
  while (select > count - 1 || select < 0) {
    printf("seleziona numero impiegato valido: \n");
    scanf("%d", &select);
  }
  return select;
}

void output_file(int fd, struct dbheader_t *dbhdr, struct employee_t *employees) {
  dbhdr->filesize = sizeof(struct dbheader_t) + (dbhdr->count * sizeof(struct employee_t));
  dbhdr->magic = htonl(dbhdr->magic);
  dbhdr->filesize = htonl(dbhdr->filesize);
  dbhdr->count = htons(dbhdr->count);
  dbhdr->version = htons(dbhdr->version);

  lseek(fd, 0, SEEK_SET);
  if (write(fd, dbhdr, sizeof(struct dbheader_t)) == -1) {
    perror("write");
    return;
  }
  if (employees != NULL && dbhdr->count > 0) {
    for (int i = 0; i < ntohs(dbhdr->count); i++) {
      employees[i].hours = htonl(employees[i].hours);
    }

    if (write(fd, employees, ntohs(dbhdr->count) * sizeof(struct employee_t)) == -1) {
      perror("write");
      return;
    }
    printf("db written\n");
  }
}
