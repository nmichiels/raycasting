//***************************************************************************************************
// TTRIGGERONCESET.H
// Task : definieert dat een trigger maar 1 keer mag worden geset in een vakje
//***************************************************************************************************
// AUTEUR: KENNETH
//***************************************************************************************************

#ifndef TRIGGERONCESET_H
#define TRIGGERONCESET_H 1
#include "task.h"
#include <sstream> 
using namespace std; 
#include <iostream>
#include <fstream>

//zet een muur aan/uit
class TriggerOnceSet: public Task 
{
public:
	TriggerOnceSet(int x, int y,bool trick):x(x),y(y),trick(trick){}

	void execute(TriggerData a){
		//controle en uitvoeren
		if(x>=0 && y>=0 && x<a.world->WorldWidth() && y < a.world->WorldHeight())
			a.world->trigger= trick;
	}

	//wegschrijven
	string toString(){
		stringstream s;
		s<<"TriggerOnceSet"<< " to " << trick <<" coord( " <<x<< " , "<<y<<" )\n"; 
		return s.str();
	}

	//initialisator om in te lezen via een file
	TriggerOnceSet(fstream *s){
		char *d=new char[222];
		(*s)>>d>>trick>>d>>x>>d>>y>>d;
		delete[] d;
	}
private:
	bool trick;
	int x;
	int y;
};

#endif