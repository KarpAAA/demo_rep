#include <iostream>
#include <string>
#include <Windows.h>



using namespace std;
const double DEFAULT_A = 0;
const double DEFAULT_B = 1000;

void infoTable(int count, PROCESS_INFORMATION* prInfo, string* stateString, string* nameString) {
	cout << endl << endl;
	cout << "Name  \t         Pid\tPriority\tWorkingTime\t" << endl;
	cout << "----------------------------------------------------" << endl;
	for (int i = 0; i < count; i++) {

		cout << nameString[i] << "\t" << prInfo[i].dwProcessId << "\t   ";

		string prior = "";
		DWORD pr = GetPriorityClass(prInfo[i].hProcess);
		switch (pr) {
		case HIGH_PRIORITY_CLASS: {
			 prior = "HIGH";
			break;
		}
		case ABOVE_NORMAL_PRIORITY_CLASS: {
			 prior = "ABOVE_NORMAL";
			break;
		}
		case NORMAL_PRIORITY_CLASS: {
			 prior = "NORMAL";
			break;
		}
		case BELOW_NORMAL_PRIORITY_CLASS: {
			 prior = "BELOW_NORMAL";
			break;
		}
		}

		cout << prior << "    \t" << stateString[i] << endl;
	}
	//printf("%14s|%13d|%21s\n", nameString[i], prInfo[i].dwProcessId, /*GetPriorityClass(prInfo[i].hProcess)*/stateString[i]);

	cout << "----------------------------------------------------" << endl;
	cout << "To Add - a\nTo suspend - s\nTo continue - c\nTo end - e\nTo kill - k\nTo set priority - p\n\nTo EXIT - 0" << endl;
}
void suspending(int count, PROCESS_INFORMATION* prInfo, string* stateString) {

	int PID;

	cout << "Enter PID of process: ";

	cin >> PID;

	cout << endl;

	for (int j = 0; j < count; j++) {

		if (PID == prInfo[j].dwProcessId) {
			SuspendThread(prInfo[j].hThread);
			stateString[j] = "Suspended";
		}
	}
}
void continuing(int count, PROCESS_INFORMATION* prInfo, string* stateString) {

	int PID;

	cout << "Enter PID of process: ";

	cin >> PID;

	cout << endl;

	for (int j1 = 0; j1 < count; j1++) {

		if (PID == prInfo[j1].dwProcessId) {

			ResumeThread(prInfo[j1].hThread);
			stateString[j1] = "Running";
		}
	}
}
void changingPriority(int count, PROCESS_INFORMATION* prInfo) {

	int pr = 0;
	int PID;

	cout << "Enter Pid of the process: ";
	cin >> PID;
	cout << endl;

	for (int j = 0; j < count; j++) {
		if (PID == prInfo[j].dwProcessId) {

			cout << "New priority: " << endl;

			cout << "HIGH_PRIORITY: 0" << endl;

			cout << "ABOVE_NORMAL_PRIORITY: 1" << endl;

			cout << "NORMAL_PRIORITY: 2" << endl;

			cout << "BELOW_NORMAL_PRIORITY: 3" << endl << endl;

			cin >> pr;

			if (pr > 3 || pr < 0) break;

			DWORD priorities[4] = {
				HIGH_PRIORITY_CLASS,
				ABOVE_NORMAL_PRIORITY_CLASS,
				NORMAL_PRIORITY_CLASS,
				BELOW_NORMAL_PRIORITY_CLASS
			};

			SetPriorityClass(prInfo[j].hProcess, priorities[pr]);


		}
	}
}
void addingNewProcess(int& count, PROCESS_INFORMATION* prInfo, STARTUPINFO* stInfo, HANDLE* handles, string* timeString, string* nameString, wstring formedPath) {

	if (count < 10) {

		ZeroMemory(&stInfo[count], sizeof(STARTUPINFO));
		stInfo[count].cb = sizeof(STARTUPINFO);

		if (CreateProcess(NULL, (LPWSTR)formedPath.c_str(), NULL, NULL, true, CREATE_NEW_CONSOLE | NORMAL_PRIORITY_CLASS, NULL, NULL, &stInfo[count], &prInfo[count])) {

			
			nameString[count] = "Process " + to_string(count + 1);
			timeString[count] = "Running";

			handles[count] = prInfo[count].hProcess;
			count++;
		}
	}
	else {

		cout << "Process overflow..." << endl;
	}
}
void finishing(int& count, PROCESS_INFORMATION* prInfo, STARTUPINFO* stInfo, HANDLE* handles, string* timeString, string* nameString, UINT sig) {

	int PID;
	cout << "Enter PID of process: ";
	cin >> PID;
	cout << endl;
	int index;
	double timeSpent;

	for (int j = 0; j < count; j++) {

		if (PID == prInfo[j].dwProcessId) {

			TerminateProcess(prInfo[j].hProcess, sig);
			WaitForSingleObject(prInfo[j].hProcess, INFINITE);

			cout << endl;
			FILETIME F, M, C, D;
			if (GetProcessTimes(prInfo[j].hProcess, (FILETIME*)&F, (FILETIME*)&M, (FILETIME*)&C, (FILETIME*)&D)) {


				timeSpent = (long)&D * pow(10., -7.);
				cout << nameString[j] << "Process has ran: " << timeSpent << " sec" << endl << endl;
				getchar();
				getchar();
			}

			CloseHandle(prInfo[j].hThread);
			CloseHandle(prInfo[j].hProcess);

			index = j;
			break;
		}

	}
	
	if (index < count) {

		count--;

		for (int j = index; j < count; j++) {

			stInfo[j] = stInfo[j + 1];
			prInfo[j] = prInfo[j + 1];
			handles[j] = handles[j + 1];
			timeString[j] = timeString[j + 1];
			nameString[j] = nameString[j + 1];
		}
	}
}
boolean chooseAction(char action, int& count, string* timeString
	, string* nameString, HANDLE* handles, STARTUPINFO* stInfo, PROCESS_INFORMATION* prInfo, wstring cmdline, string* stateString) {


	if (action == 'a') addingNewProcess(count, prInfo, stInfo, handles, timeString, nameString, cmdline);
	else if (action == 's') suspending(count, prInfo, stateString);
	else if (action == 'c') continuing(count, prInfo, stateString);
	else if (action == 'e') finishing(count, prInfo, stInfo, handles, timeString, nameString, 0);
	else if (action == 'k') finishing(count, prInfo, stInfo, handles, timeString, nameString, 1);
	else if (action == 'p') changingPriority(count, prInfo);
	else if (action == '0') return true;


	return false;
}


int main(int argc, char** argv) {

	double A;
	double B;
	double h;
	int count = 0;

	cout << "A B and h: ";
	cin >> A >> B >> h;
	
	HANDLE* handles = new HANDLE[10];
	string* stateString = new string[10];
	string* nameString = new string[10];


	wstring pathToFile = L"C:\\Users\\ivank\\source\\repos\\lab03_oc\\Debug\\lab03_oc.exe ";
	wstring comandLineARGS = { to_wstring(A) + L" " + to_wstring(B) + L" " + to_wstring(h) };
	wstring formedPath = pathToFile + comandLineARGS;


	STARTUPINFO* stInfo = new STARTUPINFO[10];
	PROCESS_INFORMATION* prInfo = new PROCESS_INFORMATION[10];
	
	
	
		while (1) {

			cout << "Enter number of processes from 1 to 10: ";
			cin >> count;

			if (count > 10 || count < 1) {
				cout << endl << "Bad format try again" << endl;
				continue;
			}
		
			std::cout << "Press any key to continue";
			getchar();

			break;
		}

	
		

		for (int i = 0; i < count; i++) {

			ZeroMemory(&stInfo[i], sizeof(STARTUPINFO));
			stInfo[i].cb = sizeof(STARTUPINFO);
		}

		for (int i = 0; i < count; ++i) {
			if (CreateProcess(NULL, (LPWSTR)formedPath.c_str(), NULL, NULL
				, true, CREATE_NEW_CONSOLE | NORMAL_PRIORITY_CLASS, NULL, NULL, &stInfo[i], &prInfo[i])) {
				handles[i] = prInfo[i].hProcess;
				stateString[i] = "Running";
				nameString[i] = "Process " + to_string(i + 1);
			}
		}

		while (1) {
			infoTable(count, prInfo, stateString, nameString);
			if(chooseAction(getchar(),count,stateString,nameString,handles,stInfo,prInfo,formedPath,stateString)) break;
			system("CLS");
		}

		system("CLS");



	return 0;
}
