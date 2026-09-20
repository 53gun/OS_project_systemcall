#include "types.h"
#include "stat.h"
#include "user.h"
#include "date.h"

static int
valid_date(struct rtcdate *r)
{
  if(r->second > 59)
    return 0;
  if(r->minute > 59)
    return 0;
  if(r->hour > 23)
    return 0;
  if(r->day < 1 || r->day > 31)
    return 0;
  if(r->month < 1 || r->month > 12)
    return 0;
  if(r->year < 2000)
    return 0;
  return 1;
}

int
main(int argc, char *argv[])
{
  struct rtcdate r;
  struct rtcdate *bad;
  int ok;

  (void)argc;
  (void)argv;
  ok = 1;
  memset(&r, 0, sizeof(r));

  if(date(&r) < 0){
    printf(1, "date: valid call returned -1\n");
    ok = 0;
  } else if(!valid_date(&r)){
    printf(1, "date: invalid value %d-%d-%d %d:%d:%d\n",
           r.year, r.month, r.day, r.hour, r.minute, r.second);
    ok = 0;
  } else {
    printf(1, "date: %d-%d-%d %d:%d:%d UTC\n",
           r.year, r.month, r.day, r.hour, r.minute, r.second);
  }

  // sbrk(0) is the first address outside the current user address space.
  // A correct implementation must reject it without writing through it.
  bad = (struct rtcdate*)sbrk(0);
  if(date(bad) != -1){
    printf(1, "date: invalid pointer was not rejected\n");
    ok = 0;
  }

  if(ok)
    printf(1, "TEST_1_PASS\n");
  else
    printf(1, "TEST_1_FAIL\n");

  exit();
}
