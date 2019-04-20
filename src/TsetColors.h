//***************************************************************************************************
// TSETCOLORS.H
// Task : zet de overlappende kleur van een wereld (mist)
//***************************************************************************************************
// AUTEUR: KENNETH
//***************************************************************************************************

#ifndef SETCOLORS_H
#define SETCOLORS_H 1
#include "task.h"
#include <sstream> 
using namespace std; 
#include <iostream>
#include <fstream>


//zet de overlappende kleur (bvb mist) van de wereld
class SetColors: public Task 
{
public:
	SetColors(int red,int green, int blue):red(red),blue(blue),green(green){}

	void execute(TriggerData a){
		a.world->r=red;
		a.world->g=green;
		a.world->b=blue;
	}

	//wegschrijven
	string toString(){
		stringstream s;
		s<<"SetColors"<< " Red: " << red <<" Blue: " <<blue<< " Green: "<<green<<" \n"; 
		return s.str();
	}

	//initialisator om in te lezen via een file
	SetColors(fstream *s){
		char *d=new char[222];
		(*s)>>d>>red>>d>>blue>>d>>green;
		delete[] d;
	}
private:
	int blue;
	int red;
	int green;
};

#endif