//***************************************************************************************************
// TCHANGESPRITE.H
// Task : een sprite laten veranderen
//***************************************************************************************************
// AUTEUR: KENNETH
//***************************************************************************************************

#ifndef CHANGESPRITE_H
#define CHANGESPRITE_H 1
#include "task.h"
#include <sstream> 
using namespace std; 
#include <iostream>
#include <fstream>
//past een sprite aan in een vakje
class ChangeSprite: public Task 
{
public:
	ChangeSprite(int x, int y, int t):x(x),y(y),texture(t){}

	void execute(TriggerData a){
		//controle en uitvoeren
		if(x>=0 && y>=0 && x<a.world->WorldHeight() && y < a.world->WorldWidth())
			(*(a.world))[x][y].sprite=texture;
	}

	//wegschrijvem	
	string toString(){
		stringstream s;
		s<<"ChangeSprite"<< " to_texture "<< texture << " ,coord( " <<x<< " , "<<y<< " )\n"; 
		return s.str();
	}
	//initialisator om in te lezen via een file
	ChangeSprite(fstream *s){
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