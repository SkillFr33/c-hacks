#ifndef _PERSON_MSGBUF_H
#define _PERSON_MSGBUF_H

#include <stdlib.h>
#include <stdio.h>

void panic(const char* msg) {
  perror(msg);
  exit(EXIT_FAILURE);
}

struct person_msgbuf {
  long mtype;
  struct person_info {
    char   name[32];
    char   gender;
    int    age;
    double weight;
  } info;
};

#endif