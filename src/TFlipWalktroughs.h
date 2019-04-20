//***************************************************************************************************
// TFLIPWALKTROUGHS.H
// Task : maakt meerdere vakjes doorloopbaar of niet
//***************************************************************************************************
// AUTEUR: KENNETH
//***************************************************************************************************

#ifndef FLIPWALKTROUGHS_H
#define FLIPWALKTROUGHS_H 1
#include "task.h"
#include <sstream> 
using namespace std; 
#include <iostream>
#include <fstream>

//flipt meerdere vakjes wat betreft 'doorloopbaarheid'
class FlipWalktroughs: public Task 
{
public:
	FlipWalktroughs(int xl, int xr,int yu,int yd):xleft(xl),xright(xr),yup(yu),ydown(yd){}

	void execute(struct TriggerData a){
		//controle en uitvoeren
		int xr=min(a.world->WorldHeight(),xright+1)-1;
		int yd=min(a.world->WorldWidth(),ydown+1)-1;
		for(int x=max(0,xleft);x <= xr ;x++)
			for(int y=max(0, yup);y <= yd;y++)
				(*(a.world))[x][y].walktrough= !(*(a.world))[x][y].walktrough;
	}
	
	//wegschrijven
	string toString(){
		stringstream s;
		s<<"FlipWalktroughs"<< " coordLU( " <<xleft<< " , "<<yup<< " ) ,coordRU( "<<xright<<" , "<<ydown<<" )\n";  
		return s.str();	
	}

	//initialisator om in te lezen via een file
	FlipWalktroughs(fstream *s){
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