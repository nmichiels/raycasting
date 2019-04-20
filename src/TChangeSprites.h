//***************************************************************************************************
// TCHANGESPRITES.H
// Task : meerdere sprites laten veranderen
//***************************************************************************************************
// AUTEUR: KENNETH
//***************************************************************************************************

#ifndef CHANGESPRITES_H
#define CHANGESPRITES_H 1
#include <sstream> 
using namespace std; 
#include <iostream>
#include <fstream>
#include "task.h"

//past meerdere sprites aan in een rechthoek
class ChangeSprites: public Task 
{
public:
	ChangeSprites(int xr, int xl,int yu,int yd,int t):xright(xr),xleft(xl),yup(yu),ydown(yd),texture(t){}

	void execute(TriggerData a){
		//controle en uitvoeren
		int xr=min(a.world->WorldHeight(),xright)-1;
		int yd=min(a.world->WorldWidth(),ydown)-1;
		for(int x=max(0,xleft);x <= xr ;x++)
			for(int y=max(0, yup);y <= yd;y++)
				(*(a.world))[x][y].sprite=texture;
	}
	
	//wegschrijven
	string toString(){
		stringstream s;
		s<<"ChangeSprites"<< " to_texture "<< texture << " ,coordLU( " <<xleft<< " , "<<yup<< " ) ,coordRU( "<<xright<<" , "<<ydown<<")\n"; 
		return s.str();
	}
	//initialisator om in te lezen via een file
	ChangeSprites(fstream *s){
		char *d=new char[222];
		(*s)>>d>>texture>>d>>xleft>>d>>yup>>d>>d>>xright>>d>>ydown>>d;
		delete[] d;
	}
private:
	int xright;
	int xleft;
	int yup;
	int ydown;
	int texture;
};

#endif