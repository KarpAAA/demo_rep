#include <unistd.h>
#include <iostream>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/resource.h>
#include <string>
#include <csignal>
#include <chrono>
#define MAX_PROCESSES 10

using namespace std;
int main()
{
int processesAmount = 0;
double a = 0;
double b = 0;
double n = 0;

void printTable(int processesAmount, pid_t PID[]);
cout << "Enter processes amount:";
cin>>processesAmount;
cout << "Enter a = ";
cin>>a;
cout << "Enter b (a<b) = ";
cin>>b;
cout << "Enter n = ";
cin>>n;

pid_t PID[MAX_PROCESSES];
clock_t* time = new clock_t[MAX_PROCESSES];
int status;

for (int i=0; i<processesAmount; ++i)
{
PID[i] = fork();
time[i] = clock();

switch (PID[i])
{
case 0:
{
string path = "/home/veronika/individual.out ";
string args = to_string(a) + " " + to_string(b) + " " + to_string(n);
string way = path + args;
execl("/usr/bin/xterm", "xterm", "-e", way.c_str(), (char*)NULL);
break;
}
case -1:
{
cout << "Error" << endl;
break;
}
default: {}
}
}

while (true)
{

printTable(processesAmount, PID);

char actionNumber;
cin >> actionNumber;
bool finish = false;

switch(actionNumber)
{
case '1': //change priority
{
int priority = 2;
int PIDtoChange;
cout <<"Enter PID of process to change priority: ";
cin >> PIDtoChange;

for (int j=0; j<processesAmount; ++j)
{
if (PIDtoChange == PID[j])
{
cout << "Enter new priority: ";
cin >> priority;
setpriority(PRIO_PROCESS, PID[j], priority);
cout << "You changed priority of " << PID[j] << " to" << getpriority(PRIO_PROCESS, PID[j]) << endl;
}
}
break;
}
case '2': //suspend process
{
int PIDtoSuspend;
cout <<"Enter PID of process to suspend: ";
cin >> PIDtoSuspend;
kill (PIDtoSuspend, SIGSTOP);
break;
}
case '3': //continue process
{
int PIDtoContinue;
cout <<"Enter PID of process to continue: ";
cin >> PIDtoContinue;
kill (PIDtoContinue, SIGCONT);
break;
}
case '4': //kill process
{
int PIDtoKill;
cout <<"Enter PID of process to kill: ";
cin >> PIDtoKill;
for (int j=0; j<processesAmount; ++j)
{
if (PIDtoKill == PID[j])
{
kill (PIDtoKill, SIGKILL);
waitpid (PIDtoKill, &status, 0);
time[j] = clock() - time[j]; 
cout << "Process " << PID[j] << " running time: " << to_string((((double)time[j]))/CLOCKS_PER_SEC)<< " s" << endl << endl;
cout << "You killed a process " << PID[j] << endl;
}
}

break;
}
case '0':
{
finish = true;
break;
}
default:
{
break;
}
}
if (finish) break;
}

cout << "\n\n|  Process Number  |     PID     |  PRIORITY  |      TIME      |" << endl;
cout << "----------------------------------------------------------------" << endl;

for (int j=0; j< processesAmount; ++j)
{
kill (PID[j], SIGKILL);
waitpid (PID[j], &status, 0);

cout << "|   process #" + to_string(j+1) << "     |   " << PID[j] << "\t |\t" << getpriority(PRIO_PROCESS, PID[j]) << "     |   " << to_string(((double)time[j])/CLOCKS_PER_SEC)<< " s"<< "   |   "<< endl;    
} 
cout << "----------------------------------------------------------------" << endl;
delete[] time;

getchar();
return 0;
}

void printTable(int processAmount, pid_t PID[])
{
cout << "\n\n|  Process Number  |     PID     |  PRIORITY  |" << endl;
cout << "-----------------------------------------------" << endl;

for (int i=0; i< processAmount; ++i)
{
cout << "|   process #" + to_string(i+1) << "     |   " << PID[i] << "\t |\t" << getpriority(PRIO_PROCESS, PID[i]) << "     |   " << endl;    
} 
cout << "-----------------------------------------------" << endl;
cout << "PRESS TO:\n1 - change priority\n2 - suspend\n3 - continue\n4 - kill\n0 - FINISH\n\nYour choice: ";
}
