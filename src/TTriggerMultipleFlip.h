//***************************************************************************************************
// TTRIGGERMULTIPLEFLIP.H
// Task : zet meerdere triggers aan of uit
//***************************************************************************************************
// AUTEUR: KENNETH
//***************************************************************************************************

#ifndef TRIGGERMULTIPLEFLIP_H
#define TRIGGERMULTIPLEFLIP_H 1
#include "task.h"
#include <sstream> 
using namespace std; 
#include <iostream>
#include <fstream>

//switcht een muur tussen aan/uit
class TriggerMultipleFlip: public Task 
{
public:
	TriggerMultipleFlip(){}

	void execute(TriggerData a){
			a.world->trigger= !a.world->trigger;
	}

	//wegschrijven
	string toString(){
		stringstream s;
		s<<"TriggerMultipleFlip"<<"\n"; 
		return s.str();
	}
	//initialisator om in te lezen via een file
	TriggerMultipleFlip(fstream *s){
	}
private:
};

#endif