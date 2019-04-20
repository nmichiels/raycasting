//***************************************************************************************************
// TFLIPSPRITE.H
// Task : laat een sprite wisselen tussen aan en uit
//***************************************************************************************************
// AUTEUR: KENNETH
//***************************************************************************************************

#ifndef FLIPSPRITE_H
#define FLIPSPRITE_H 1
#include "task.h"
#include <sstream> 
using namespace std; 
#include <iostream>
#include <fstream>

//wisselt een sprite tussen aan en uit
class FlipSprite: public Task 
{
public:
	FlipSprite(int x, int y):x(x),y(y){}

	void execute(TriggerData a){
		//controle en uitvoeren
		if(x>=0 && y>=0 && x<a.world->WorldHeight() && y < a.world->WorldWidth())
			(*(a.world))[x][y].isSprite = !(*(a.world))[x][y].isSprite;
	}
	//wegschrijven
	string toString(){
		stringstream s;
		s<<"FlipSprite"<< " coord( " <<x<< " , "<<y<<" )\n"; 
		return s.str();
	}

	//initialisator om in te lezen via een file
	FlipSprite(fstream *s){
		char *d=new char[222];
		(*s)>>d>>x>>d>>y>>d;
		delete[] d;
	}
private:
	int x;
	int y;
};

#endif