#include "hash.h"


// http://stackoverflow.com/a/7666577
unsigned long osl_hash(const char * str) {
  unsigned long hash = 0;
  const char* ptr = str;
  while (*ptr) {
    int c = *ptr;
    hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
    ptr++;
  }
  return hash;
}
