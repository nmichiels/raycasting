//***************************************************************************************************
// TSETWALKTHROUGHS.H
// Task : zet de doorloopbaarheid van meerdere vakjes
//***************************************************************************************************
// AUTEUR: KENNETH
//***************************************************************************************************

#ifndef SETWALKTROUGHS_H
#define SETWALKTROUGHS_H 1
#include "task.h"
#include <sstream> 
using namespace std; 
#include <iostream>
#include <fstream>

//zet de 'doorloopbaarheid' van meerdere vakjes
class SetWalktroughs: public Task 
{
public:
	SetWalktroughs(int xl, int xr,int yu,int yd,bool walk):xleft(xl),xright(xr),yup(yu),ydown(yd),walk(walk){}

	void execute(struct TriggerData a){
		//controle en uitvoeren
		int xr=min(a.world->WorldHeight(),xright+1)-1;
		int yd=min(a.world->WorldWidth(),ydown+1)-1;
		for(int x=max(0,xleft);x <= xr ;x++)
			for(int y=max(0, yup);y <= yd;y++)
				(*(a.world))[x][y].walktrough= walk;
	}
	
	//wegschrijven
	string toString(){
		stringstream s;
		s<<"SetWalktroughs"<<" to "<<walk<< " coordLU( " <<xleft<< " , "<<yup<< " ) ,coordRU( "<<xright<<" , "<<ydown<<" )\n";  
		return s.str();	
	}

	//initialisator om in te lezen via een file
	SetWalktroughs(fstream *s){
		char *d=new char[222];
		(*s)>>d>>walk>>d>>xleft>>d>>yup>>d>>d>>xright>>d>>ydown>>d;
		delete[] d;
	}
private:
	bool walk;
	int xright;
	int xleft;
	int yup;
	int ydown;
};

#endif