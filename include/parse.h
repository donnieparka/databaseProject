#ifndef PARSE_H
#define PARSE_H

#define HEADER_MAGIC 0x4c4c4144

struct dbheader_t {
  unsigned int magic;
  unsigned short count;
  unsigned short version;
  unsigned int filesize;
};

struct employee_t {
  char name[256];
  char address[256];
  unsigned int hours;
};

struct dbheader_t *create_db_header();
int validate_db_header(int fd, struct dbheader_t **header_out);
struct employee_t* read_employees(int fd, struct dbheader_t *dhhdr);
struct employee_t* realloc_employees(struct employee_t *employees, int count);
struct employee_t* add_employee(struct employee_t *employees, int count);
int choose_employee(int count);
void output_file(int fd, struct dbheader_t*, struct employee_t* employees);
#endif
