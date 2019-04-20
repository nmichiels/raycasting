//***************************************************************************************************
// TFLIPWALKTROUGH.H
// Task : maakt een vakje doorloopbaar of niet
//***************************************************************************************************
// AUTEUR: KENNETH
//***************************************************************************************************

#ifndef FLIPWALKTROUGH_H
#define FLIPWALKTROUGH_H 1
#include "task.h"
#include <sstream> 
using namespace std; 
#include <iostream>
#include <fstream>

//wissel tussen wel en niet 'doorloopbaar'
class FlipWalktrough: public Task 
{
public:
	FlipWalktrough(int x, int y):x(x),y(y){}

	void execute(TriggerData a){
		//controle en uitvoeren
		if(x>=0 && y>=0 && x<a.world->WorldHeight() && y < a.world->WorldWidth())
			(*(a.world))[x][y].walktrough= !(*(a.world))[x][y].walktrough;
	}

	//wegschrijven
	string toString(){
		stringstream s;
		s<<"FlipWalktrough"<< " coord( " <<x<< " , "<<y<<" )\n"; 
		return s.str();
	}

	//initialisator om in te lezen via een file
	FlipWalktrough(fstream *s){
		char *d=new char[222];
		(*s)>>d>>x>>d>>y>>d;
		delete[] d;
	}
private:
	int x;
	int y;
};

#endif