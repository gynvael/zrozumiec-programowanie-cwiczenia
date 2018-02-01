/*
	[FPU: Inf, NaN, -0]
	Wymień jak najwięcej różnych operacji, które w wyniku dadzą Inf, NaN lub ujemne zero.
	
	NaN : 
	0 / 0  
	inf - inf   
	...*  
	
	Inf:  
	1 / 0  
	...*  
	
	-Inf:  
	-1 / 0  
	...*
	
	-0:
	0 * -1
	...*
	
		* mozna uzupelnic liste programem i eksperymentowac ;) 
		
		Foralost (M. SZ.)
*/

#include <iostream>
#define null 0

struct chardouble { 
	unsigned char* buff = null;
	
	operator double() { 
	
		return *((double*)(buff));
	}
};

using namespace std;
int main(int argc, char** argv)
{
	//IEEE 754 - 2008
	chardouble nzero, inf, ninf, nan, zero;
	
	zero.buff = new unsigned char[8]();
								// 0 000000000000 000000000000000000000...
	nzero.buff = new unsigned char[8]{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x80};
								// 1 000000000000 000000000000000000000...
	inf.buff = new unsigned char[8]{0x00,0x00,0x00,0x00,0x00,0x00,0xF0,0x7F};
								// 0 111111111111 000000000000000000000...
	ninf.buff = new unsigned char[8]{0x00,0x00,0x00,0x00,0x00,0x00,0xF0,0xFF};
								// 1 111111111111 000000000000000000000...
	nan.buff = new unsigned char[8]{0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF}; 
								// 1 111111111111 111111111111111111111...
								
	cout << "Generowanie liczb typu +/- Inf, 0, -0, NaN na zawolanie" << endl;
	cout << "INF: " << (double)inf << endl;
	cout << "-INF: " << (double)ninf << endl;
	cout << "NaN: " << (double)nan << endl;
	cout << "Zero: " << (double)zero << endl;
	cout << "NZero: " << (double)nzero << endl;
	
	cout << endl;
	
	cout << "nan mul nan: " << (double)((double)nan * (double)nan) << endl;
	cout << "zero mul -1: " << (double)((double)zero * (-1)) << endl;
	cout << "inf - inf: " << (double)( (double)inf - (double)inf) << endl;
	cout << "zero div zero: " << (double)( (double)zero / (double)zero) << endl;
	cout << "-1 div zero: " << (double)(-1 / (double)zero) << endl;
	cout << "1 div zero: " << (double)(1 / (double)zero) << endl;
	
	return 0;
}