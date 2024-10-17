#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>

int createDb(char *fileName) {
  int fd = open(fileName, O_RDONLY);
  if (fd != -1) {
    close(fd);
    printf("can't create database\n");
    perror("open");
    return -1;
  } else {
    fd = open(fileName, O_RDWR | O_CREAT, 0644);
    if (fd == -1) {
      printf("file not created\n");
      perror("open");
      return -1;
    }
    printf("database created\n");
    return fd;
  }
}
int openDb(char *fileName) {
  int fd = open(fileName, O_RDWR, 0644);
  if (fd == -1) {
    perror("open");
    return -1;
  }
  printf("database opened\n");
  return fd;
}
