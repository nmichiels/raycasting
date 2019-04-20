//***************************************************************************************************
// TSETSPRITES.H
// Task : zet meerdere sprites aan en uit
//***************************************************************************************************
// AUTEUR: KENNETH
//***************************************************************************************************

#ifndef SETSPRITES_H
#define SETSPRITES_H 1
#include "task.h"
#include <sstream> 
using namespace std; 
#include <iostream>
#include <fstream>


//zet meerdere sprites tegelijk aan of uit (in een rechthoekvorm)
class SetSprites: public Task 
{
public:
	SetSprites(int xl, int xr,int yu,int yd,bool isSprite):xleft(xl),xright(xr),yup(yu),ydown(yd),isSprite(isSprite){}

	void execute(struct TriggerData a){
		//controle en uitvoeren
		int xr=min(a.world->WorldHeight(),xright+1)-1;
		int yd=min(a.world->WorldWidth(),ydown+1)-1;
		for(int x=max(0,xleft);x <= xr ;x++)
			for(int y=max(0, yup);y <= yd;y++)
				(*(a.world))[x][y].isSprite= isSprite;
	}
	
	//wegschrijvem
	string toString(){
		stringstream s;
		s<<"SetSprites"<<" to "<<isSprite<< " coordLU( " <<xleft<< " , "<<yup<< " ) ,coordRU( "<<xright<<" , "<<ydown<<" )\n";  
		return s.str();	
	}

	//initialisator om in te lezen via een file
	SetSprites(fstream *s){
		char *d=new char[222];
		(*s)>>d>>isSprite>>d>>xleft>>d>>yup>>d>>d>>xright>>d>>ydown>>d;
		delete[] d;
	}
private:
	bool isSprite;
	int xright;
	int xleft;
	int yup;
	int ydown;
};

#endif