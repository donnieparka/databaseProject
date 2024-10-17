#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "../include/common.h"
#include "../include/parse.h"

int validate_db_header(int fd, struct dbheader_t **header_out) {
  struct dbheader_t *header = calloc(1, sizeof(struct dbheader_t));
  if (header == NULL) {
    printf("calloc failed to create header validation space\n");
    perror("calloc");
    return STATUS_ERROR;
  }
  int read_success = read(fd, header, sizeof(struct dbheader_t));
  if (read_success != sizeof(struct dbheader_t)) {
    printf("can not read for validation");
    perror("read");
    free(header);
    return STATUS_ERROR;
  }
}

struct dbheader_t *create_db_header(int fd) {
  struct dbheader_t *header = calloc(1, sizeof(struct dbheader_t));
  if (header == NULL) {
    perror("calloc");
    printf("calloc failed");
    return NULL;
  }
  header->count = 0;
  header->version = 0x1;
  header->filesize = sizeof(struct dbheader_t);
  header->magic = HEADER_MAGIC;
  return header;
}
