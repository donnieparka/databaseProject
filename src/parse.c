#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>

#include "../include/common.h"
#include "../include/parse.h"

void output_file(int fd, struct dbheader_t *dbhdr) {
  dbhdr->magic = htonl(dbhdr->magic);
  dbhdr->filesize = htonl(sizeof(struct dbheader_t));
  dbhdr->count = htons(dbhdr->count);
  dbhdr->version = htons(dbhdr->version);

  lseek(fd, 0, SEEK_SET);
  if (write(fd, dbhdr, sizeof(struct dbheader_t)) == -1) {
    perror("write");
    return;
  }
  printf("db written");
  return;
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
    printf("filesize mismatch");
    free(header);
    return STATUS_ERROR;
  }
  if (header->version != 1) {
    printf("database version wrong");
    return STATUS_ERROR;
  }
  printf("validation ok");
  return STATUS_SUCCESS;
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
