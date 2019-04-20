//***************************************************************************************************
// TSETWALL.H
// Task : zet een muur aan of uit
//***************************************************************************************************
// AUTEUR: KENNETH
//***************************************************************************************************

#ifndef SETWALL_H
#define SETWALL_H 1
#include "task.h"
#include <sstream> 
using namespace std; 
#include <iostream>
#include <fstream>

//zet een muur aan/uit
class SetWall: public Task 
{
public:
	SetWall(int x, int y,bool wall):x(x),y(y),wall(wall){}

	void execute(TriggerData a){
		//controle en uitvoeren
			(*(a.world))[x][y].wall= wall;
	}

	//wegschrijven
	string toString(){
		stringstream s;
		s<<"SetWall"<< " to " << wall <<" coord( " <<x<< " , "<<y<<" )\n"; 
		return s.str();
	}

	//initialisator om in te lezen via een file
	SetWall(fstream *s){
		char *d=new char[222];
		(*s)>>d>>wall>>d>>x>>d>>y>>d;
		delete[] d;
	}
private:
	bool wall;
	int x;
	int y;
};

#endif