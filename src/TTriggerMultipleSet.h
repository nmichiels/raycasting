//***************************************************************************************************
// TTRIGGERMULTIPLSET.H
// Task : creëert meerdere triggers tegelijk
//***************************************************************************************************
// AUTEUR: KENNETH
//***************************************************************************************************

#ifndef TRIGGERMULTIPLESET_H
#define TRIGGERMULTIPLESET_H 1
#include "task.h"
#include <sstream> 
using namespace std; 
#include <iostream>
#include <fstream>

//zet een muur aan/uit
class TriggerMultipleSet: public Task 
{
public:
	TriggerMultipleSet(bool trick):trick(trick){}

	void execute(TriggerData a){
			a.world->trigger= trick;
	}

	//wegschrijven
	string toString(){
		stringstream s;
		s<<"TriggerMultipleSet"<< " to " << trick <<"\n"; 
		return s.str();
	}

	//initialisator om in te lezen via een file
	TriggerMultipleSet(fstream *s){
		char *d=new char[222];
		(*s)>>d>>trick;
		delete[] d;
	}
private:
	bool trick;
};

#endif