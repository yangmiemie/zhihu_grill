#include <sys/time.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

int timeval_subtract(struct timeval *result, struct timeval *t2, struct timeval *t1);
void timeval_print(struct timeval *tv);


struct timeval time_print(int userId, int type, char* place);
void time_diff_print(int userId, char* place, struct timeval *tvDiff, struct timeval *tvEnd, struct timeval *tvBegin);

