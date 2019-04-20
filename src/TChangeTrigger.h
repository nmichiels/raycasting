//***************************************************************************************************
// TCHANGETRIGGER.H
// Task : een trigger laten veranderen
//***************************************************************************************************
// AUTEUR: KENNETH
//***************************************************************************************************

#ifndef CHANGETRIGGER_H
#define CHANGETRIGGER_H 1
#include "task.h"
#include <sstream> 
using namespace std; 
#include <iostream>
#include <fstream>

//geeft de mogelijkheid om een trigger toe te kennen aan een vakje
class ChangeTrigger: public Task 
{
public:
	ChangeTrigger(int x, int y, int t):x(x),y(y),trigger(t){}

	void execute(TriggerData a){
		//controle en uitvoeren
		if(x>=0 && y>=0 && x<a.world->WorldHeight() && y < a.world->WorldWidth())
				(*(a.world))[x][y].trigger=trigger;
	}
	
	//wegschrijvem
	string toString(){
		stringstream s;
		s<<"ChangeTrigger"<< " trigger: "<< trigger << " ,coord( " <<x<< " , "<<y<< " )\n"; 
		return s.str();
	}

	//initialisator om in te lezen via een file
	ChangeTrigger(fstream *s){
		char *d=new char[222];
		(*s)>>d>>trigger>>d>>x>>d>>y>>d;
		delete[] d;
	}
private:
	int x;
	int y;
	int trigger;
};

#endif