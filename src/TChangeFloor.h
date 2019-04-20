//***************************************************************************************************
// TCHANGEFLOOR.H
// Task : een vloer texture laten veranderen
//***************************************************************************************************
// AUTEUR: KENNETH
//***************************************************************************************************

#ifndef CHANGEFLOOR_H
#define CHANGEFLOOR_H 1
#include "task.h"
#include <sstream> 
using namespace std; 
#include <iostream>
#include <fstream>
//past de vloer aan in 1 vakje
class ChangeFloor: public Task 
{
public:
	ChangeFloor(int x, int y, int t):x(x),y(y),texture(t){}

	void execute(TriggerData a){
		//controleren en uitvoeren
		if(x>=0 && y>=0 && x<a.world->WorldHeight() && y < a.world->WorldWidth())
				(*(a.world))[x][y].floor=texture;
	}
	//wegschrijven
	string toString(){
		stringstream s;
		s<<"ChangeFloor"<< " to_texture "<< texture << " ,coord( " <<x<< " , "<<y<< " )\n"; 
		return s.str();
	}
		//initialisator om in te lezen via een file
	ChangeFloor(fstream *s){
		char *d=new char[222];
		(*s)>>d>>texture>>d>>x>>d>>y>>d;
		delete[] d;
	}
private:
	int x;
	int y;
	int texture;
};

#endif