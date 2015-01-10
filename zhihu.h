#include <pthread.h>
#include <stdio.h>

#define barTotalNumber 20 // the number of bars provided to the users
#define materialTotalNumber 5 // the number of material the user need to take from bars
#define grillTotalNumber 8  // the number of food material the grill can process at one time
#define MAXTHREADS 100

// the time spent in bar and grill
#define barProcessTime 10
#define grillProcessTime 180

typedef struct User
{
  int materialTaken[barTotalNumber]; //materialNo[i] == 1, means the No.i food has been taken and the user don't need any more
  int lackNo; // the number of food material the customer need
} Customer;

struct {
	pthread_mutex_t mutex;
	int bars[barTotalNumber]; // bars[i] == 1 means the No.i bar has been taken
  pthread_cond_t cond;
	int available; // the number of bars are still available for customers
} barInfo;

struct {
  pthread_mutex_t mutex;
  int grills[grillTotalNumber]; // bars[i] == 1 means the No.i grill has been taken
  pthread_cond_t cond;
  int available;  // the number of grills are still available for customers
} grillInfo;

int initInfos(); // init the value for barInfo and grillInfo
Customer getCustomer();  // get a user struct for a thread
void *process(void*);  // process in bar and grill

void doBarStuff(Customer user, int userId); // process in bar
int getBarNumber(Customer user); // get the number of bar for the user to get food material
void getMaterial(); // get food material in bar

void doGrillStuff(int userId); // process in grill
int getGrillNumber(); // get the number of grill number for the user
void grillFood(); // grill food
void* processGrill(void* args);