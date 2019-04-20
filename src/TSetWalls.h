//***************************************************************************************************
// TSETWALLS.H
// Task : zet meerdere muren aan of uit
//***************************************************************************************************
// AUTEUR: KENNETH
//***************************************************************************************************

#ifndef SETWALLS_H
#define SETWALLS_H 1
#include "task.h"
#include <sstream> 
using namespace std; 
#include <iostream>
#include <fstream>


//zet meerdere muren aan/uit
class SetWalls: public Task 
{
public:
	SetWalls(int xl, int xr,int yu,int yd,bool wall):xleft(xl),xright(xr),yup(yu),ydown(yd),wall(wall){}

	void execute(struct TriggerData a){
		//controle en uitvoeren
		int xr=min(a.world->WorldHeight(),xright+1)-1;
		int yd=min(a.world->WorldWidth(),ydown+1)-1;
		for(int x=max(0,xleft);x <= xr ;x++)
			for(int y=max(0, yup);y <= yd;y++)
				(*(a.world))[x][y].wall= wall;
	}
	
	//wegschrijven
	string toString(){
		stringstream s;
		s<<"SetWalls"<<" to "<<wall<< " coordLU( " <<xleft<< " , "<<yup<< " ) ,coordRU( "<<xright<<" , "<<ydown<<" )\n";  
		return s.str();	
	}

	//initialisator om in te lezen via een file
	SetWalls(fstream *s){
		char *d=new char[222];
		(*s)>>d>>wall>>d>>xleft>>d>>yup>>d>>d>>xright>>d>>ydown>>d;
		delete[] d;
	}
private:
	bool wall;
	int xright;
	int xleft;
	int yup;
	int ydown;
};

#endif