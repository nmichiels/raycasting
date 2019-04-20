//***************************************************************************************************
// TCHANGECEIL.H
// Task : een plafond texture laten veranderen
//***************************************************************************************************
// AUTEUR: KENNETH
//***************************************************************************************************

#ifndef CHANGECEIL_H
#define CHANGECEIL_H 1
#include "task.h"
#include <sstream> 
using namespace std; 
#include <iostream>
#include <fstream>
//Past het plafond aan in 1 vakje
class ChangeCeil: public Task 
{
public:
	ChangeCeil(int x, int y, int t):x(x),y(y),texture(t){}

	void execute(TriggerData a){
		//controle op out of bounds
		if(x>=0 && y>=0 && x<a.world->WorldHeight() && y < a.world->WorldWidth())
				(*(a.world))[x][y].ceil=texture;
	}
	//wegschrijven
	string toString(){
		stringstream s;
		s<<"ChangeCeil"<< " to_texture "<< texture << " ,coord( " <<x<< " , "<<y<< " )\n"; 
		return s.str();
	}
		//initialisator om in te lezen via een file
	ChangeCeil(fstream *s){
		char *d=new char[222];
		(*s)>>d>>texture>>d>>x>>d>>x>>d;
		delete[] d;
	}
private:
	int x;
	int y;
	int texture;
};

#endif