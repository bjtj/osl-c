#include "date.h"
#include "osl.h"
#include <assert.h>

void test_date(void)
{
  printf("== test date ==\n");
  int i = 0;
  for (; i < 3; i++)
  {
    osl_date_t now = osl_date_now();
    printf("%04d-%02d-%02d %02d:%02d:%02d.%03d\n",
           now.year, now.month+1, now.day, now.hour, now.minute, now.second, now.millisecond);
    osl_idle(200);
  }
}

int main()
{
  osl_init_once();

  test_date();
    
  osl_finish();
    
  return 0;
}
