//***************************************************************************************************
// TCHANGETRIGGER.H
// Task : meerdere triggers laten veranderen
//***************************************************************************************************
// AUTEUR: KENNETH
//***************************************************************************************************

#ifndef CHANGETRIGGERS_H
#define CHANGETRIGGERS_H 1
#include "task.h"
#include <sstream> 
using namespace std; 
#include <iostream>
#include <fstream>

//geeft de mogelijkheid om een trigger toe te kennen aan meerdere vakjes
class ChangeTriggers: public Task 
{
public:
	ChangeTriggers(int xl, int xr,int yu,int yd,int t):xleft(xl),xright(xr),yup(yu),ydown(yd),trigger(t){}

	void execute(TriggerData a){
	//controle op out of bounds
		int xr=min(a.world->WorldHeight(),xright+1)-1;
		int yd=min(a.world->WorldWidth(),ydown+1)-1;
		for(int x=max(0,xleft);x <= xr ;x++)
			for(int y=max(0, yup);y <= yd;y++)
				(*(a.world))[x][y].trigger=trigger;
	}
	
	//wegschrijvem
	string toString(){
		stringstream s;
		s<<"ChangeTriggers"<< " trigger: "<< trigger << " ,coordLU( " <<xleft<< " , "<<yup<< " ) ,coordRU( "<<xright<<" , "<<ydown<<" )\n"; 
		return s.str();
	}

	//initialisator om in te lezen via een file
	ChangeTriggers(fstream *s){
		char *d=new char[222];
				(*s)>>d>>trigger>>d>>xleft>>d>>yup>>d>>d>>xright>>d>>ydown>>d;
		delete[] d;
	}
private:
	int xright;
	int xleft;
	int yup;
	int ydown;
	int trigger;
};

#endif