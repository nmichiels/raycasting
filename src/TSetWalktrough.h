//***************************************************************************************************
// TSETWALKTHROUGH.H
// Task : zet de doorloopbaarheid van een vakje
//***************************************************************************************************
// AUTEUR: KENNETH
//***************************************************************************************************

#ifndef SETWALKTROUGH_H
#define SETWALKTROUGH_H 1
#include "task.h"
#include <sstream> 
using namespace std; 
#include <iostream>
#include <fstream>


//zet de 'doorloopbaarheid' van een vakje
class SetWalktrough: public Task 
{
public:
	SetWalktrough(int x, int y,bool walk):x(x),y(y),walk(walk){}

	void execute(TriggerData a){
		//controle en uitvoeren
		if(x>=0 && y>=0 && x<a.world->WorldWidth() && y < a.world->WorldHeight())
			(*(a.world))[x][y].walktrough= walk;
	}

	//wegschrijven
	string toString(){
		stringstream s;
		s<<"SetWalktrough"<< " to " << walk <<" coord( " <<x<< " , "<<y<<" )\n"; 
		return s.str();
	}

	//initialisator om in te lezen via een file
	SetWalktrough(fstream *s){
		char *d=new char[222];
		(*s)>>d>>walk>>d>>x>>d>>y>>d;
		delete[] d;
	}
private:
	bool walk;
	int x;
	int y;
};

#endif