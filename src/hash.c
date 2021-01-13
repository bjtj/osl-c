#include "hash.h"


// http://stackoverflow.com/a/7666577
unsigned long osl_hash(const char * str) {
    unsigned long hash = 0;
    int c;
    while ((c = *str++)) {
	hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
    }
    return hash;
}
