#pragma once 
#include <string>
#include <fstream>
#include <chrono> // for creating random data
#include <random> // same
/*
	Person
	
	Its a class that represents in a small perspective a human.
	It has a structure which is a package of data which consists of (sur)name, 
	secret password and a also a packet of 64byte private data.

	The structure of serialized data: (byte per byte)
	
	HEADER OF NEW PERSON     AGE      BYTE OF NAME LENGTH    BYTE OF SURNAME LENGTH  BYTE OF PASS LENGTH NAME, SURNAME, PASS, DATA
	--------------------  ---------   ---------------------- ----------------------  ------------------- --------------
	     FF AA FF       XX XX XX XX   XX                     XX                             XX           XXXXXXXXXXXXXXXX...
*/ 

class Person {
	struct Credentials { 
		string szName, szSurName;
		string szSecretPasw;
		unsigned iAge;
		
		char* pPrivateData = NULL; //and we know its 64  bytes long or it should be

	} Data;
	
	public:
		Person() = delete;
		Person(string szFileName);
		Person(string gName, string gSurName, string gSecretPassword, unsigned gAge);

		void printInfo();
		bool isValid();
		bool dumptoFile(string* szFileName = NULL);
		bool readfromFile(string szFileName);
};

bool Person::isValid()
{
	return Data.pPrivateData;
}
Person::Person(string szFileName)
{
	if(!readfromFile(szFileName))
		cout << "Failed to create a person using constructor from file: " << szFileName << endl;
}

Person::Person(string gName, string gSurName, string gSecretPassword, unsigned gAge)
{
	Data.szSecretPasw = gSecretPassword; 
	Data.szName = gName; 
	Data.szSurName = gSurName; 
	Data.iAge = gAge;

	Data.pPrivateData = new char[64](); // creating a 64 zeroed byte array of some random string which consists of letters from A-Z
	default_random_engine engine(chrono::steady_clock::now().time_since_epoch().count());
	uniform_int_distribution<int> magicnmb(65,90);
	for(int i = 0 ; i < 63 ; i++)
		Data.pPrivateData[i] = (unsigned char)(magicnmb(engine));
}
/*
	Prints info about current person
*/
void Person::printInfo()
{
	if (!isValid())
		return;

	cout << "Printing some info about person named: " << Data.szName <<
	" " << Data.szSurName << '\n';
	cout << "His or Hers secret password: " << Data.szSecretPasw << '\n';
	cout << "Aged: " << Data.iAge << '\n';
	
	cout << "Private data " << Data.pPrivateData << endl; 
	cout << endl;
	return;
}
/*
	Serializes (I suppose its correct terminology) the Person object into a file using C++ fstream.
	It uses the same scheme as showed at the beginning of the class.
*/
bool Person::dumptoFile(string* szFileName)
{
	fstream FILE;
	
	if (!szFileName) {
		string szNewFileName = Data.szName;
		szNewFileName.append(Data.szSurName).append(to_string(Data.iAge)).append(".per");
		//NAME+SURNAME+iAGE.per
		FILE.open(szNewFileName, ios::out | ios::trunc);
		cout << "Preparing to dump a person into a file..." << szNewFileName << endl;
	}
	else {
		FILE.open(*szFileName, ios::out | ios::trunc);
		cout << "Preparing to dump a person into a file..." << *szFileName << endl;
	}
	 char bHeader[] = {(char)0xFF, (char)0xAA, (char)0xFF}; // Persons "format" header
	if(!FILE.good())
		return false;

	FILE.write(bHeader, 3);

	char *p = (char*)(&Data.iAge);
	FILE.write(p, 4);
	FILE << (char)(Data.szName.size()) << (char)(Data.szSurName.size()) << 
		(char)(Data.szSecretPasw.size())<< Data.szName << Data.szSurName << Data.szSecretPasw;
	p = Data.pPrivateData;

	FILE.write(p, 64);

	FILE.close();
	cout << endl;
	return true;
}
/*
	Deserializes a .per format file into an actual Person object class.
	Currently this function can load only 1 person from file.
*/
bool Person::readfromFile(string szFileName)
{
	cout << "Preparing to read a person from file..." << szFileName << endl;
	fstream FILE;
	FILE.open(szFileName, ios::in);
	
	if (!FILE.good())
		return false;

	char* bHeader = new char[3];
	FILE.read(bHeader, 3);
	if (bHeader[0] != (char)0xFF || //Checking for correct Header
		bHeader[1] != (char)0xAA ||
		bHeader[2] != (char)0xFF)
		return false;
	
	FILE.read((char*)(&Data.iAge), 4);

	unsigned char bLenName, bLenSurname, bLenPass;
	FILE.read((char*)&bLenName, 1).read((char*)&bLenSurname, 1).read((char*)&bLenPass, 1);  //Get info about lengths of strings
	
	char* pData[3];
	pData[0] = new char[bLenName + 1](); // Reserve some space for string ( + 1 for null termination)
	pData[1] = new char[bLenSurname + 1]();
	pData[2] = new char[bLenPass + 1]();

	FILE.read(pData[0], bLenName);
	FILE.read(pData[1], bLenSurname);
	FILE.read(pData[2], bLenPass);

	Data.szName = string(pData[0]);
	Data.szSurName = string(pData[1]);
	Data.szSecretPasw = string(pData[2]);

	for (int i = 0; i < 3; i++)
		delete pData[i];

	Data.pPrivateData = new char[64]();
	FILE.read(Data.pPrivateData, 64);
	FILE.close();
	cout << endl;
	return true;
}




