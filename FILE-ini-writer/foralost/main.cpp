#include <iostream>
#include <fstream>

#include <map>
#define String string

/*
	This program uses C++ map which has a dictionary skeleton which used to represent a INI section and then
	converts that section into proper text / .ini format
*/
//Foralost (M. SZ.)

using namespace std;
typedef std::map<string, string> _SECTION;
typedef std::map<string, _SECTION> _INI_FILE;


bool SectionToIni(_INI_FILE& Sections, string& szFileName, bool append)
{
	fstream FILE;
	
	if(append)
		FILE.open(szFileName, ios::out | ios::app);
	else
		FILE.open(szFileName, ios::out | ios::trunc);
	
	if(!FILE.good()){
		FILE.close();
		return false;
	}
	for(auto& x : Sections ) {
		if(!x.first.compare("GLOBAL"))
			FILE << '\n';
		else	
			FILE << '[' << x.first << ']' << '\n';
	
		for(auto& y : x.second){
			
			if(!y.first.compare("COMMENT"))
				FILE << ';' << y.second << '\n';
			else
				FILE << y.first << " = " << y.second << '\n';
		
		}
	
	FILE << endl;
	}
	FILE.close();
	
	return true;
}	

int main()
{
	string szFileName("tempfile.ini");
	
	_INI_FILE INISections;
	
	_SECTION GLOBAL;
	GLOBAL["max_enemy_count"] = "20";
	GLOBAL["COMMENT"] = "Przykładowy plik w formacie INI.";
	
	_SECTION Hero;
	Hero["start_hp"] = "100";
	Hero["start_mp"] = "100";
	Hero["start_inv"] = "Item.Dagger,Item.Cap";
	
	_SECTION Item_Dagger;
	Item_Dagger["dmg"] = "2";
	Item_Dagger["max_durability"] = "15";
	Item_Dagger["value"] = "4";
	
	_SECTION Item_Cap;
	Item_Cap["ar"] = "1";
	Item_Cap["max_durability"] = "9";
	Item_Cap["value"] = "2";
	
	INISections["GLOBAL"] = GLOBAL;
	INISections["Hero"] = Hero;
	INISections["Item.Dagger"] = Item_Dagger;
	INISections["Item.Cap"] = Item_Cap;
	
	SectionToIni(INISections, szFileName, true);
	return 0;
		
}