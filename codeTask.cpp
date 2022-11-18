/******************************************************************************

                            Online C Compiler.
                Code, Compile, Run and Debug C program online.
Write your code in this editor and press "Run" button to compile and execute it.

*******************************************************************************/

#include <iostream>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <string>
#include <cstring>
#include <cmath>
#include <unistd.h>

using namespace std;
void printingTable();
int changeAvailibility();
std::string task3();
double radians(double degrees);

int main() {

	std::string result = "";
	int myFile;
	if ((myFile = creat("./lab8_Output.txt", S_IRWXU)) < 0)
	{
		cout << "File was not created\n";
	}


	while (true) {
		printingTable();

		char actionNumber;

		cin >> actionNumber;
		bool finish = false;

		switch (actionNumber)
		{
		case '1':
		{
			result = "1\n";
			system("cls");

			write(myFile, result.c_str(), strlen(result.c_str()));
			break;
		}
		case '2':
		{
			system("cls");
			int res = changeAvailibility();
			system("cls");
			if (res == 1) {
				
				int switchMode;
				if ((switchMode = chmod("./lab8_result.txt", S_IREAD)))
				{
					cout << "Read mode enabled" << endl;
				}
				
			}
			else if (res == 2) {
				

				int switchMode;
				if ((switchMode = chmod("./lab8_result.txt", S_IREAD)))
				{
					cout << "Read mode enabled" << endl;
				}

			}
			else {}
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
		if (finish) break;

	}
	close(myFile);

	getchar();
	return 0;
}
void printingTable() {

	cout << "\n---------------------------------------------" << endl;

	cout << "\n(1) - to write 3laba result to file\n(2) - to change file availability\n\n(0) - to EXIT";

	cout << "\n---------------------------------------------" << endl;
}
int changeAvailibility() {
	int actionNumber;

	cout << "\n(1) - to ONLY_READ mode\n(2) - to WRITE mode\n\n(0) - to EXIT";
	cin >> actionNumber;
	return actionNumber;
}
std::string task3() {
	std::string result = "";
	double A = 1;
	double B = 1000;
	double h = 0.01;

	double value = 0;
	for (double i = A; i <= B; i += h) {

		value = sin(radians(i));
		result += "Sin( ";
		result += (std::to_string(i));
		result += " ) = ";
		result += (std::to_string(value));
		result += "\n";

	}
	return result;
}
double radians(double degrees)
{
	double radians;
	double const pi = 3.14159265358979323846;
	radians = (pi / 180) * degrees;
	return radians;
}
