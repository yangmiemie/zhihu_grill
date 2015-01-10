#include "timer.h"

/* Return 1 if the difference is negative, otherwise 0.  */
int timeval_subtract(struct timeval *result, struct timeval *t2, struct timeval *t1)
{
    long int diff = (t2->tv_usec + 1000000 * t2->tv_sec) - (t1->tv_usec + 1000000 * t1->tv_sec);
    result->tv_sec = diff / 1000000;
    result->tv_usec = diff % 1000000;

    return (diff<0);
}

void timeval_print(struct timeval *tv)
{
    char buffer[30];
    time_t curtime;

    printf("%ld.%06ld", tv->tv_sec, tv->tv_usec);
    printf("\n");
    // curtime = tv->tv_sec;
    // strftime(buffer, 30, "%m-%d-%Y  %T", localtime(&curtime));
    // printf(" = %s.%06ld\n", buffer, tv->tv_usec);
}

struct timeval time_print(int userId, int type, char* place)
{
  struct timeval tvBegin, tvEnd, tvDiff;
  gettimeofday(&tvBegin, NULL);
  if (type == 0){
     printf("Customer %d in %s start at:", userId, place);    
  }
  else {
     printf("Customer %d in %s end at:", userId, place);    
  }
  timeval_print(&tvBegin);
  return tvBegin;
}

void time_diff_print(int userId, char* place, struct timeval *tvDiff, struct timeval *tvEnd, struct timeval *tvBegin)
{
  timeval_subtract(tvDiff, tvEnd, tvBegin);      
  printf("Customer %d in %s time spend:", userId, place);
  printf("%ld.%06ld\n", (*tvDiff).tv_sec, (*tvDiff).tv_usec);
}
// int main()
// {
//     struct timeval tvBegin, tvEnd, tvDiff;

//     // begin
//     gettimeofday(&tvBegin, NULL);
//     timeval_print(&tvBegin);

//     // lengthy operation
//     // int i,j;
//     // for(i=0;i<999999L;++i) {
//     // 	j=sqrt(i);
//     // }
//     sleep(5);
//     //end
//     gettimeofday(&tvEnd, NULL);
//     timeval_print(&tvEnd);

//     // diff
//     timeval_subtract(&tvDiff, &tvEnd, &tvBegin);
//     printf("%ld.%06ld\n", tvDiff.tv_sec, tvDiff.tv_usec);

//     return 0;
// }