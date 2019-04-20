//***************************************************************************************************
// TFLIPSPRITES.H
// Task : laat meerdere sprites wisselen tussen aan en uit
//***************************************************************************************************
// AUTEUR: KENNETH
//***************************************************************************************************

#ifndef FLIPSPRITES_H
#define FLIPSPRITES_H 1
#include "task.h"
#include <sstream> 
using namespace std; 
#include <iostream>
#include <fstream>

//flipt meerdere sprites aan/uit in een rechthoek
class FlipSprites: public Task 
{
public:
	FlipSprites(int xl, int xr,int yu,int yd):xleft(xl),xright(xr),yup(yu),ydown(yd){}

	void execute(struct TriggerData a){
		//controle en uitvoeren
		int xr=min(a.world->WorldHeight(),xright+1)-1;
		int yd=min(a.world->WorldWidth(),ydown+1)-1;
		for(int x=max(0,xleft);x <= xr ;x++)
			for(int y=max(0, yup);y <= yd;y++)
				(*(a.world))[x][y].isSprite= !(*(a.world))[x][y].isSprite;
		
	}
	//wegschrijven
	string toString(){
		stringstream s;
		s<<"FlipSprites"<< " coordLU( " <<xleft<< " , "<<yup<< " ) ,coordRU( "<<xright<<" , "<<ydown<<" )\n";  
		return s.str();	
	}

	//initialisator om in te lezen via een file
	FlipSprites(fstream *s){
		char *d=new char[222];
		(*s)>>d>>xleft>>d>>yup>>d>>d>>xright>>d>>ydown>>d;
		delete[] d;
	}
private:
	int xright;
	int xleft;
	int yup;
	int ydown;
};

#endif