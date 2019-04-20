//***************************************************************************************************
// TFLIPWALL.H
// Task : laat een muur afwisselen tussen aan en uit
//***************************************************************************************************
// AUTEUR: KENNETH
//***************************************************************************************************

#ifndef FLIPWALL_H
#define FLIPWALL_H 1
#include "task.h"
#include <sstream> 
using namespace std; 
#include <iostream>
#include <fstream>

//switcht een muur tussen aan/uit
class FlipWall: public Task 
{
public:
	FlipWall(int x, int y):x(x),y(y){}

	void execute(TriggerData a){
		//controle en uitvoeren
		if(x>=0 && y>=0 && x<a.world->WorldHeight() && y < a.world->WorldWidth())
			(*(a.world))[x][y].wall= !(*(a.world))[x][y].wall;
	}

	//wegschrijven
	string toString(){
		stringstream s;
		s<<"FlipWall"<< " coord( " <<x<< " , "<<y<<" )\n"; 
		return s.str();
	}
	//initialisator om in te lezen via een file
	FlipWall(fstream *s){
		char *d=new char[222];
		(*s)>>d>>x>>d>>y>>d;
		delete[] d;
	}
private:
	int x;
	int y;
};

#endif