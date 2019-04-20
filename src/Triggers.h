//***************************************************************************************************
// TRIGGERS.H
// Deze klasse houdt alle triggers bij
//***************************************************************************************************
// AUTEUR: KENNETH
//***************************************************************************************************

#ifndef TRIGGERS_H
#define TRIGGERS_H 1

#include <map>
using namespace std;
#include <qstring>
#include "Trigger.h"
#include <sstream> 
using namespace std; 
#include <iostream>
#include <fstream>
#include <stdlib.h>

//volgorde van triggers
#ifndef SMALLER
#define SMALLER 1
struct smaller
{
  bool operator()( int i1, int i2) const
  {
    return i1<i2;
  }
};
#endif

class Triggers{
public:
	Triggers();
	~Triggers();
	//een trigger toevoegen
	void add(int nr,Trigger *a);
	//een trigger opvragen
	Trigger* get(int i);
	//triggers opslaan en inlezen
	void fromString(fstream *s);
	string toString();
private:
	//map om triggers bij te houden volgens nr
	map<int,Trigger*,smaller> tricky;
};

#endif	