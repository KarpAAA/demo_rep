#include <iostream>
#include <fstream>
#include <sstream>
#include <ctime>
#include <cstdlib>
#include <vector>
#include <stdio.h>
#include <pthread.h>
#include <signal.h>
#include <algorithm>

using namespace std;

typedef struct {
	int threadNumber;
	int startIndex;
	int endIndex;
} th_param;

void printingTable(int threadAmount, pthread_t* threads);
void fillArray();
void* individualTask(void* args);
void testPrintage();


bool canAcsess = false;
bool ifReady=true;
pthread_cond_t condition = PTHREAD_COND_INITIALIZER;
char synhMethod = ' ';
int n = 10000;
int* arr;
int threadsAmount;
int globalMinimum;
pthread_t* threads;
pthread_mutex_t MUTEX = PTHREAD_MUTEX_INITIALIZER;
pthread_barrier_t BARRIER;
string conclusionString ="";
int main() {
	pthread_barrier_init(&BARRIER, NULL, threadsAmount);
	srand(time(NULL));
	fillArray();
	globalMinimum = arr[0];
	
	bool ifChoiseMade = false;
	pthread_t* threads = NULL;

	//user entering threads amount
	while (!ifChoiseMade) {
		cout << "Enter amount of threads";
		cin >> threadsAmount;
		cout << "Type of synchronization:\n(b) - bariers\n(m) - for mutex: ";
		cin >> synhMethod;

		if (threadsAmount == 2 || threadsAmount == 4 || threadsAmount == 8 || threadsAmount == 16)
		{
			threads = new pthread_t[threadsAmount];
			ifChoiseMade = true;
		}
		else {
			system("cls");
			cout << "Invalid data typed try again" << endl;
		}
	}


	


	//creating threads and measuring time thread 
	clock_t* time = new clock_t[threadsAmount];
	int ostacha = 0;
	int amountPerThread = n / threadsAmount;
	for (int i = 0; i < threadsAmount; ++i) {
	    th_param param ={0,0,0};
	    if (i == threadsAmount - 1) { ostacha = n % threadsAmount; }
		param.threadNumber = i;
		param.startIndex = (i * amountPerThread);
		param.endIndex = (i * amountPerThread) + amountPerThread + ostacha;
		
		pthread_create(&threads[i], NULL, &individualTask, &param);
		
		time[i] = clock();
	}

	
	while (true) {
		printingTable(threadsAmount, threads);

		char actionNumber;
		cin >> actionNumber;
		bool finish = false;

		switch (actionNumber){

		case '1':{
			int numb;
			cout << "Enter thread to be detached: ";
			cin >> numb;

			
			pthread_detach(threads[numb]);
			break;
		}

		case '2':{
			int numb;
			cout << "Enter number to be canceled: ";
			cin >> numb;

			
			pthread_cancel(threads[numb]);
			break;
		}
		
		case '0':{
			finish = true;
			break;
		}


		default:
			break;
		}
		if (finish) break;
	}


	for (int i = 0; i < threadsAmount; ++i) {
	
		pthread_join(threads[i],NULL);
	}

	cout << "Thread #\tTID\tTime spent" << endl;
	cout << "-------------------------------------------" << endl;
	for (int j = 0; j < threadsAmount; j++)
	{
		time[j] = clock() - time[j];
		cout << "Thread #" + to_string(j) << "\t" << threads[j];
		cout << "\t\t" << to_string(((double)time[j]) / CLOCKS_PER_SEC) << " s" << endl;
	}
	cout << "-------------------------------------------" << endl;



	

    
	cout << "Minimum array element " << *std::min_element(arr, arr + n);
	cout << " Found min element is : " << globalMinimum << "\n\n";
	
	testPrintage();
	return 0;
}

void printingTable(int threadAmount, pthread_t* threads) {

	cout << "\n\nThread#\t\t\tTid" << endl;
	cout << "----------------------------------" << endl;

	
	for (int i = 0; i < threadAmount; i++){
		cout << "thread #" + to_string(i) << "\t" << threads[i] << endl;
	}

	cout << "---------------------------------------------" << endl;

	cout << "\n(1) - to detech a thread\n(2) - to cancel a thread\n\n\n(0) - to EXIT";
	cout << "\n\nMake a choice : ";
}
void* individualTask(void* args) {
	
	int threadNumber = (int)((th_param*)args)->threadNumber;
	int min = globalMinimum;
	int startIndex = (int)((th_param*)args)->startIndex;
	int endIndex = (int)((th_param*)args)->endIndex;


	for (int i = startIndex; i < endIndex; ++i) {
		if (min > arr[i]) {
			min = arr[i];
		}
	}
   

	if (synhMethod == 'm') {
	    
		pthread_mutex_lock(&MUTEX);
		while(!ifReady){
		    pthread_cond_wait(&condition,&MUTEX);
		}
		ifReady = false;
		
		
		if (min < globalMinimum) {

			globalMinimum = min;
		}
		conclusionString += ("Min found by thread #" + std::to_string(threadNumber)
+ " : " + std::to_string(min) + " My range is [" +
std::to_string(startIndex) + " " + std::to_string(endIndex - 1) + "]" + "\n");

        ifReady = true;
        pthread_cond_signal(&condition);
		pthread_mutex_unlock(&MUTEX);
		
		
	}

	else if (synhMethod == 'b')
	{
		
		if (min < globalMinimum) {
			globalMinimum = min;
		}
		conclusionString += ("Min found by thread #" + std::to_string(threadNumber)
+ " : " + std::to_string(min) + " My range is [" +
std::to_string(startIndex) + " " + std::to_string(endIndex - 1) + "]" + "\n");

		pthread_barrier_wait(&BARRIER);
	}
	else {
		if (min < globalMinimum) {
			globalMinimum = min;
		}
	}
	
	return NULL;
}

void fillArray() {
	arr = new int[n];
	for (int i = 0; i < n; ++i) {
		arr[i] = rand() % 100000;
	}
}
void showArray() {
	cout << endl << endl;
	for (int i = 0; i < n; ++i) {
		cout << arr[i] << "\t";
	}
	cout << endl << endl;
}
void testPrintage() {
cout << endl << endl << conclusionString << endl;
}
