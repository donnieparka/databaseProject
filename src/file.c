#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>

int createDb(char *fileName) {
  int fd = open(fileName, O_RDONLY);
  if (fd != -1) {
    close(fd);
    printf("file already exists\n");
    return -1;
  }
  fd = open(fileName, O_RDWR | O_CREAT, 0644);
  if (fd == -1) {
    printf("file not created\n");
    perror("open");
    return -1;
  }
  return fd;
}
int openDb(char *fileName) {
  int fd = open(fileName, O_RDWR, 0644);
  if (fd == -1) {
    printf("file not opened\n");
    perror("open");
    return -1;
  }
  return fd;
}
