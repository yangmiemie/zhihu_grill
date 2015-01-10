#include "zhihu.h"
#include "timer.h"

int main(int argc, char** argv) {
  if (argc != 3 && argc != 2)
  {
    printf("Usage error: zhihu program need 1 or 2 arguments\n");
    return 0;
  }

  int nthreads = atoi(argv[1]);
  int waitTime = 0;
  pthread_t tid[MAXTHREADS];
  int i = 0;
  int userId[MAXTHREADS];
  void* joinRet;

  if (argv[2] != 0)
  {
    waitTime = atoi(argv[2]);
  }

  printf("%d customers is running...\n", nthreads);
  initInfos();

  struct timeval tvBegin, tvEnd, tvDiff;
  gettimeofday(&tvBegin, NULL);
  printf("All start at:");
  timeval_print(&tvBegin);

  // create thread for each customer
  for (i = 0; i < nthreads; ++i)
  {
    userId[i] = i;
    pthread_create(&tid[i], NULL, process, &userId[i]);
    if (waitTime != 0)
    {
      sleep(waitTime);
    }
  }

  for (i = 0; i < nthreads; ++i)
  {
    pthread_join(tid[i], NULL);
    printf("pthread_join %d\n", i);
  }

  gettimeofday(&tvEnd, NULL);
  printf("All end at:");
  timeval_print(&tvEnd);

  // diff
  timeval_subtract(&tvDiff, &tvEnd, &tvBegin);
  printf("All time spent:");
  printf("%ld.%06ld\n", tvDiff.tv_sec, tvDiff.tv_usec);
}

int initInfos()
{
  int i = 0;

  // init barInfo value
  pthread_mutex_init(&barInfo.mutex , NULL);
  barInfo.available = barTotalNumber;
  for(i = 0; i < barTotalNumber; ++i)
  {
    barInfo.bars[i] = 0;
  }

  // init barInfo value
  pthread_mutex_init(&grillInfo.mutex , NULL);
  grillInfo.available = grillTotalNumber;
  for(i = 0; i < grillTotalNumber; ++i) 
  {
    grillInfo.grills[i] = 0;
  }
}

// get a customer
Customer getCustomer() {
	Customer user;
  int i = 0;
	user.lackNo = materialTotalNumber;
	for(i = 0; i < barTotalNumber; ++i) {
		user.materialTaken[i] = 0;
	}
	return user;
}

void* process(void* args) {
	Customer user = getCustomer();
  int userId = *((int*) args);
	doBarStuff(user, userId);
  doGrillStuff(userId);
}

int getGrillNumber() {
  int i = 0;
  for (i = 0; i < grillTotalNumber; ++i)
  {
    // find a grill that is available
    if(grillInfo.grills[i] == 0) {
      return i;
    }
  }
} 

void grillFood() {
  sleep(grillProcessTime);
}

void* processGrill(void* args) {
  pthread_mutex_lock(&grillInfo.mutex);
  while(grillInfo.available == 0) {
    pthread_cond_wait(&grillInfo.cond, &grillInfo.mutex);
  }
  int grillNumber = getGrillNumber();
  grillInfo.available--;
  grillInfo.grills[grillNumber] = 1;
  pthread_mutex_unlock(&grillInfo.mutex);

  grillFood();

  pthread_mutex_lock(&grillInfo.mutex);
  if (grillInfo.available == 0)
  {
    pthread_cond_broadcast(&grillInfo.cond);
  }
  grillInfo.available++;
  grillInfo.grills[grillNumber] = 0;
  pthread_mutex_unlock(&grillInfo.mutex);
}

void doGrillStuff(int userId) {
  int i = 0;
  pthread_t tid[MAXTHREADS];

  struct timeval tvBegin, tvEnd, tvDiff;
  tvBegin = time_print(userId, 0, "grill");

  // create a thread for each food material
  for (i = 0; i < materialTotalNumber; ++i)
  {
      pthread_create(&tid[i], NULL, processGrill, NULL);
  }
  for (i = 0; i < materialTotalNumber; ++i)
  {
    pthread_join(tid[i], NULL);
  }

  tvEnd = time_print(userId, 1, "grill");

  time_diff_print(userId, "grill", &tvDiff, &tvEnd, &tvBegin);  
}


int getBarNumber(Customer user) {
  int i = 0;
  for (i = 0; i < barTotalNumber; ++i)
  {
    // the No.i bar is available and the user has not taken the food from this bar
    if(barInfo.bars[i] == 0 && user.materialTaken[i] == 0) {
      return i;
    }
  }
  // there is no available bar or the food in available bars has been taken by the user
  return -1;
}

void getMaterial() {
  sleep(barProcessTime);
}

void doBarStuff(Customer user, int userId){
  // begin
  struct timeval tvBegin, tvEnd, tvDiff;
  tvBegin = time_print(userId, 0, "bar");

  // if the number of user's material is less than materialTotalNumber
  while(user.lackNo > 0){
    pthread_mutex_lock(&barInfo.mutex);
    while(barInfo.available == 0) {
      // the user has to waiting for no available bars
      pthread_cond_wait(&barInfo.cond, &barInfo.mutex);
    }
    if (getBarNumber(user) != -1) {
      barInfo.available--;
      int barNo = getBarNumber(user);
      barInfo.bars[barNo] = 1;
      user.materialTaken[barNo] = 1;
      user.lackNo--;
      pthread_mutex_unlock(&barInfo.mutex);

      getMaterial();

      pthread_mutex_lock(&barInfo.mutex);
      if (barInfo.available == 0)
      {
        pthread_cond_broadcast(&barInfo.cond);
      }
      barInfo.available++;
      barInfo.bars[barNo] = 0;
      pthread_mutex_unlock(&barInfo.mutex);
    }
    else {
        // the user has to waiting because there are no different bars but have available bars
        pthread_mutex_unlock(&barInfo.mutex);      
    }
  }

  tvEnd = time_print(userId, 1, "bar");

  time_diff_print(userId, "bar", &tvDiff, &tvEnd, &tvBegin);
}