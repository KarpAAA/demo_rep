#include <ctime>
#include <iostream>
#include <vector>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <algorithm>

using namespace std;

typedef struct
{
  int threadNumber;
  int startIndex;
  int endIndex;
} th_param;

void printingTable (int threadAmount, pthread_t * threads);
void fillArray ();
void *individualTask (void *args);
void testPrintage ();


bool canAcsess = false;
bool ifReady = true;
pthread_cond_t condition = PTHREAD_COND_INITIALIZER;
char synhMethod = ' ';
int n = 1000;
int *arr;
int threadsAmount;
int globalMinimum;
pthread_t *threads;
pthread_mutex_t MUTEX;
pthread_barrier_t BARRIER;
string conclusionString = "";
int* minArray;
int main ()
{

  if (pthread_mutex_init (&MUTEX, NULL) != 0)
    {
      printf ("\n mutex init has failed\n");
      return 1;
    }

 
  srand (time (NULL));
  fillArray ();
  globalMinimum = arr[0];

  bool ifChoiseMade = false;
  pthread_t *threads = NULL;

  //user entering threads amount
  while (!ifChoiseMade)
    {
      cout << "Enter amount of threads";
      cin >> threadsAmount;
      cout << "Type of synchronization:\n(b) - bariers\n(m) - for mutex: ";
      cin >> synhMethod;

      if (threadsAmount == 2 || threadsAmount == 4 || threadsAmount == 8
	  || threadsAmount == 16|| threadsAmount == 100)
	{
	  threads = new pthread_t[threadsAmount];
	  minArray = new int[threadsAmount];
	  ifChoiseMade = true;
	   pthread_barrier_init(&BARRIER, NULL, threadsAmount);
	}
      else
	{

	  cout << "Invalid data typed try again" << endl;
	}
    }





  //creating threads and measuring time thread 
  clock_t *time = new clock_t[threadsAmount];
  th_param params[threadsAmount];
  int ostacha = 0;
  int amountPerThread = n / threadsAmount;
  for (int i = 0; i < threadsAmount; ++i)
    {
      
      if (i == threadsAmount - 1)
	{
	  ostacha = n % threadsAmount;
	}
	
      params[i].threadNumber = i;
      params[i].startIndex = (i * amountPerThread);
      params[i].endIndex = (i * amountPerThread) + amountPerThread + ostacha;

      pthread_create (&threads[i], NULL, &individualTask, &params[i]);

      time[i] = clock ();
    }

  sleep (1);
  cout << endl;
  while (true)
    {
      printingTable (threadsAmount, threads);

      char actionNumber;
      cin >> actionNumber;
      bool finish = false;

      switch (actionNumber)
	{

	case '1':
	  {
	    int numb;
	    cout << "Enter thread to be detached: ";
	    cin >> numb;


	    pthread_detach (threads[numb]);
	    break;
	  }

	case '2':
	  {
	    int numb;
	    cout << "Enter number to be canceled: ";
	    cin >> numb;


	    pthread_cancel (threads[numb]);
	    break;
	  }

	case '0':
	  {
	    finish = true;
	    break;
	  }


	default:
	  break;
	}
      if (finish)
	break;
    }


  for (int i = 0; i < threadsAmount; ++i)
    {

      pthread_join (threads[i], NULL);
    }

  cout << "Thread #\tTID\tTime spent" << endl;
  cout << "-------------------------------------------" << endl;
  for (int j = 0; j < threadsAmount; j++)
    {
      time[j] = clock () - time[j];
      cout << "Thread #" + to_string (j) << "\t" << threads[j];
      cout << "\t\t" << to_string (((double) time[j]) /
				   CLOCKS_PER_SEC) << " s" << endl;
    }
  cout << "-------------------------------------------" << endl;






  cout << "Minimum array element " << *std::min_element (arr, arr + n);
  cout << " Found min element is : " << globalMinimum << "\n\n";
  pthread_barrier_destroy(&BARRIER);
  testPrintage ();
  return 0;
}

void
printingTable (int threadAmount, pthread_t * threads)
{

  cout << "\n\nThread#\t\t\tTid" << endl;
  cout << "----------------------------------" << endl;


  for (int i = 0; i < threadAmount; i++)
    {
      cout << "thread #" + to_string (i) << "\t" << threads[i] << endl;
    }

  cout << "---------------------------------------------" << endl;

  cout <<
    "\n(1) - to detech a thread\n(2) - to cancel a thread\n\n\n(0) - to EXIT";
  cout << "\n\nMake a choice : ";
}

void *
individualTask (void *args)
{
  int threadNumber = (int) ((th_param *) args)->threadNumber;
  int min = globalMinimum;
  int startIndex = (int) ((th_param *) args)->startIndex;
  int endIndex = (int) ((th_param *) args)->endIndex;


  for (int i = startIndex; i < endIndex; ++i)
    {
      if (min > arr[i])
	{
	  min = arr[i];
	}
    }

  if (synhMethod == 'm')
    {

      pthread_mutex_lock (&MUTEX);

      if (min < globalMinimum)
	{
	     sleep(1);
	  globalMinimum = min;
	}
      conclusionString +=
	("Min found by thread #" + std::to_string (threadNumber) + " : " +
	 std::to_string (min) + " My range is [" +
	 std::to_string (startIndex) + " " + std::to_string (endIndex - 1) +
	 "]" + "\n");

      
      pthread_mutex_unlock (&MUTEX);


    }

  else if (synhMethod == 'b')
    {

  int threadNumber = (int) ((th_param *) args)->threadNumber;
  int min = globalMinimum;
  int startIndex = (int) ((th_param *) args)->startIndex;
  int endIndex = (int) ((th_param *) args)->endIndex;
  minArray[threadNumber] = arr[startIndex];

  for (int i = startIndex; i < endIndex; ++i)
    {
      if (minArray[threadNumber] > arr[i])
	{
	  minArray[threadNumber] = arr[i];
	}
    }
    
    conclusionString +=
	("Min found by thread #" + std::to_string (threadNumber) + " : " +
	 std::to_string (minArray[threadNumber]) + " My range is [" +
	 std::to_string (startIndex) + " " + std::to_string (endIndex - 1) +
	 "]" + "\n");
	 sleep(1);
     pthread_barrier_wait (&BARRIER);
     globalMinimum = *std::min_element(minArray,minArray+threadNumber);
     
    }
  else
    {
      if (min < globalMinimum)
	{
	   sleep(1);
	  globalMinimum = min;
	}
    }
    pthread_exit(0);
  return NULL;
}

void
fillArray ()
{
  arr = new int[n];
  for (int i = 0; i < n; ++i)
    {
      arr[i] = rand () % 10000000;
    }
}

void
showArray ()
{
  cout << endl << endl;
  for (int i = 0; i < n; ++i)
    {
      cout << arr[i] << "\t";
    }
  cout << endl << endl;
}

void
testPrintage ()
{
  cout << endl << endl << conclusionString << endl;
}
