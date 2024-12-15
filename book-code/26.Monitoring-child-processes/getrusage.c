#include <sys/resource.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define ARR_SIZE 100000000

static int arr[ARR_SIZE];
static int * ptr;

struct rusage usage;

/*
struct rusage {
               struct timeval ru_utime; // user CPU time used /
               struct timeval ru_stime; // system CPU time used /
               long   ru_maxrss;        // maximum resident set size /
               long   ru_ixrss;         // integral shared memory size /
               long   ru_idrss;         // integral unshared data size /
               long   ru_isrss;         // integral unshared stack size /
               long   ru_minflt;        // page reclaims (soft page faults) /
               long   ru_majflt;        // page faults (hard page faults) /
               long   ru_nswap;         // swaps /
               long   ru_inblock;       // block input operations /
               long   ru_oublock;       // block output operations /
               long   ru_msgsnd;        // IPC messages sent /
               long   ru_msgrcv;        // IPC messages received /
               long   ru_nsignals;      // signals received /
               long   ru_nvcsw;         // voluntary context switches /
               long   ru_nivcsw;        // involuntary context switches / 
    };
*/

int main(){
  ptr = malloc(sizeof(int)*ARR_SIZE);
  for(int i=0;i<ARR_SIZE;++i){
    ptr[i] = i * 10 + i * 10 + i * 10;
  }
  for(int i=0;i<ARR_SIZE;++i){
    int j = i;
    while( j ){
      j /= 2;
    }
  }

  getrusage(RUSAGE_SELF, &usage);
  printf("PID: %ld, \n \
      user CPU time used : %ld (sec) \n \
      system CPU time used : %ld (sec) \n \
      maximum resident set size : %ld (bytes) \n \
      integral shared memory size : %ld (bytes) \n \
      integral unshared data size : %ld (bytes) \n \
      integral unshared stack size : %ld (bytes) \n \
      page reclaims (soft page faults) : %ld \n \
      page faults (hard page faults) : %ld \n \
      swaps : %ld \n \ 
      block input operations : %ld \n \
      block output operations : %ld \n \
      IPC messages sent : %ld \n \
      IPC messages received : %ld \n \
      signals received : %ld \n \
      voluntary context switches : %ld \n \
      involuntary context switches : %ld \n", \
      ( long int)getpid(), \
      ( long int)usage.ru_utime.tv_sec + ( usage.ru_utime.tv_usec ) / 1000000L, \
      ( long int)usage.ru_stime.tv_sec + ( usage.ru_stime.tv_usec ) / 1000000L, \
      usage.ru_maxrss, \
      usage.ru_ixrss, \
                        usage.ru_idrss, \
                                        usage.ru_isrss, \
                                        usage.ru_minflt, \
                                        usage.ru_majflt, \
                                        usage.ru_nswap, \
                                        usage.ru_inblock, \
                                        usage.ru_oublock, \
                                        usage.ru_msgsnd, \
                                        usage.ru_msgrcv, \
                                        usage.ru_nsignals, \
                                        usage.ru_nvcsw, \
                                        usage.ru_nivcsw);
}



























