#include "number.h"


int osl_parseInt(const char * str, int radix)
{
  return (int)strtol(str, NULL, radix);
}

long osl_parseLong(const char * str, int radix)
{
  return (long)strtol(str, NULL, radix);
}

float osl_parseFloat(const char * str)
{
  return strtof(str, NULL);
}

double osl_parseDouble(const char * str)
{
  return strtod(str, NULL);
}

