#include <stdio.h>
#include <stdlib.h>

#include "../include/parse.h"

struct dbheader_t *create_db_header(int fd, struct dbheader_t **header_out) {
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
