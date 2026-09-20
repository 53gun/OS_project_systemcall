#include "types.h"
#include "stat.h"
#include "user.h"
#include "syscall.h"

int
main(int argc, char *argv[])
{
  int mask;
  int parent_pid;
  int child_pid;
  int rc;

  (void)argc;
  (void)argv;
  mask = 1 << SYS_getpid;

  printf(1, "TRACE_TEST_BEGIN\n");

  if(trace(mask) < 0){
    printf(1, "TRACE_TEST_SETUP_FAIL\n");
    exit();
  }

  // Trace line 1: the parent before fork.
  parent_pid = getpid();

  // uptime is deliberately not selected and must not produce a trace line.
  (void)uptime();

  rc = fork();
  if(rc < 0){
    trace(0);
    printf(1, "TRACE_TEST_FORK_FAIL\n");
    exit();
  }

  if(rc == 0){
    // Trace line 2: proves that the child inherited the parent's mask.
    (void)getpid();

    // This getpid must not be traced after the child disables tracing.
    (void)trace(0);
    (void)getpid();
    exit();
  }

  child_pid = rc;
  (void)wait();

  // Trace line 3: proves that the child's trace(0) did not change the parent.
  (void)getpid();

  // This final getpid must not be traced.
  (void)trace(0);
  (void)getpid();

  printf(1, "TRACE_TEST_END parent=%d child=%d\n", parent_pid, child_pid);
  printf(1, "EXPECTED: exactly 3 syscall getpid lines between the markers\n");
  exit();
}
