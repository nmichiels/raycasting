//***************************************************************************************************
// TTRIGGERONCEFLIP.H
// Task : bepaalt dat een trigger maar 1 keer aan of uit mag worden gezet
//***************************************************************************************************
// AUTEUR: KENNETH
//***************************************************************************************************

#ifndef TRIGGERONCEFLIP_H
#define TRIGGERONCEFLIP_H 1
#include "task.h"
#include <sstream> 
using namespace std; 
#include <iostream>
#include <fstream>

//switcht een muur tussen aan/uit
class TriggerOnceFlip: public Task 
{
public:
	TriggerOnceFlip(int x, int y):x(x),y(y){}

	void execute(TriggerData a){
		//controle en uitvoeren
		if(x>=0 && y>=0 && x<a.world->WorldWidth() && y < a.world->WorldHeight())
			a.world->trigger= a.world->trigger;
	}

	//wegschrijven
	string toString(){
		stringstream s;
		s<<"TriggerOnceFlip"<< " coord( " <<x<< " , "<<y<<" )\n"; 
		return s.str();
	}
	//initialisator om in te lezen via een file
	TriggerOnceFlip(fstream *s){
		char *d=new char[222];
		(*s)>>d>>x>>d>>y>>d;
		delete[] d;
	}
private:
	int x;
	int y;
};

#endif