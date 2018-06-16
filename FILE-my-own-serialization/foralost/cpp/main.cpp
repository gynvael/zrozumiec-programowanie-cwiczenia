#include <iostream>
using namespace std;

#include "Person.h"

//This program demonstrates a class Person, that has a built in Serialization method.
// Foralost (M. SZ)

int main()
{
	Person me("FooName", "BarSurname", "secretpassword", 0xF00D); // Creating a pesron
	string szFileName("tempfile.ini"); 
	me.printInfo();
	me.dumptoFile(&szFileName); //Serialization

	Person otherme("tempfile.ini"); //Deserialization
	otherme.printInfo();

	return 0;
}