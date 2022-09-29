#include <iostream>
#include <cmath>


//4. Табулювати функцію sin x, задану розкладом в ряд Тейлора, в області
//її визначення на відрізку від А до В(кількість кроків не менше 100
//000 –задається користувачем).

const double DEFAULT_A = 0;
const double DEFAULT_B = 1000;
const double DEFAULT_STEP = 0.001;

double factorial(double i);
double radians(double degrees);
double power(double x, double n);
double sinus(double radians);


int main(int argc, char** argv) {

	
	double A = std::atof(argv[1]);
	double B = std::atof(argv[2]);
	double h = std::atof(argv[3]);


	if (B < A || B<0 || A<0){
		B = DEFAULT_B;
		A = DEFAULT_A;
		if ((B - A) / h < 100000) h = (B-A)/100000;
	}
	else {
	
		
		if ((B - A) / h < 100000) {
			B = DEFAULT_B;
			A = DEFAULT_A;
			h = DEFAULT_STEP;
		}
		
	}
	
	std::cout << A << " " << B << " " << h << std::endl;
	std::cout << "Press any key to continue";
	getchar();

	double value = 0;
	for (double i = A; i <= 1000; i+=h) {

		value = 0;
		

		value = sinus(radians(i));
		std::cout << "Sin( " << i << " ) = ";
		std::cout << value << std::endl;
	
	}


	return 0;
}
double factorial(double i)
{
	if (i == 0) return 1;
	else return i * factorial(i - 1);
	
}

double radians(double degrees)  
{
	double radians;
	double const pi = 3.14159265358979323846;
	radians = (pi / 180) * degrees;
	return radians;
}

double power(double x, double n) 
{
	double output = 1;
	while (n > 0)
	{
		output = (x * output);
		n--;
	}
	return output;
}

double sinus(double radians){
	double a, b, c;
	float result = 0;
	for (int y = 0; y != 9; y++)
	{
		a = power(-1, y);
		b = power(radians, (2 * y + 1));
		c = factorial((2 * y + 1));
		result = result + (a * b) / c;
	}
	return sin(radians);
}
