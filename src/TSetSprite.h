//***************************************************************************************************
// TSETSPRITE.H
// Task : zet een sprite aan en uit
//***************************************************************************************************
// AUTEUR: KENNETH
//***************************************************************************************************

#ifndef SETSPRITE_H
#define SETSPRITE_H 1
#include "task.h"
#include <sstream> 
using namespace std; 
#include <iostream>
#include <fstream>

//zet de sprites aan en uit
class SetSprite: public Task 
{
public:
	SetSprite(int x, int y,bool isSprite):x(x),y(y),isSprite(isSprite){}

	void execute(TriggerData a){
		//controle en uitvoeren
		if(x>=0 && y>=0 && x<a.world->WorldHeight() && y < a.world->WorldWidth())
			(*(a.world))[x][y].isSprite= isSprite;
	}

	//wegschrijven
	string toString(){
		stringstream s;
		s<<"SetSprite"<< " to " << isSprite <<" coord( " <<x<< " , "<<y<<" )\n"; 
		return s.str();
	}

	//initialisator om in te lezen via een file
	SetSprite(fstream *s){
		char *d=new char[222];
		(*s)>>d>>isSprite>>d>>x>>d>>y>>d;
		delete[] d;
	}
private:
	bool isSprite;
	int x;
	int y;
};

#endif