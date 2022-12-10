#include "number.h"
#include <assert.h>

int main()
{
  assert(osl_parseInt("102", 10) == 102);
  assert(osl_parseLong("102", 10) == 102);
  assert(osl_parseFloat("102.21") == 102.21f);
  assert(osl_parseDouble("102.21") == 102.21);
    
  return 0;
}
