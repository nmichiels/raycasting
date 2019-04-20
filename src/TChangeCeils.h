//***************************************************************************************************
// TCHANGECEILS.H
// Task : meerdere plafond textures laten veranderen
//***************************************************************************************************
// AUTEUR: KENNETH
//***************************************************************************************************

#ifndef CHANGECEILS_H
#define CHANGECEILS_H 1
#include "task.h"
#include <sstream> 
using namespace std; 
#include <iostream>
#include <fstream>
//past het plafond aan in meerdere vakjes ( rechthoek)
class ChangeCeils: public Task 
{
public:
	ChangeCeils(int xl, int xr,int yu,int yd,int t):xleft(xl),xright(xr),yup(yu),ydown(yd),texture(t){}

	void execute(struct TriggerData a){
		//controle op out of bounds
		int xr=min(a.world->WorldHeight(),xright+1)-1;
		int yd=min(a.world->WorldWidth(),ydown+1)-1;
		for(int x=max(0,xleft);x <= xr ;x++)
			for(int y=max(0, yup);y <= yd;y++)
				(*(a.world))[x][y].ceil=texture;
	}

	//wegschrijvem
	string toString(){
		stringstream s;
		s<<"ChangeCeils"<< " to_texture "<< texture << " ,coordLU( " <<xleft<< " , "<<yup<< " ) ,coordRU( "<<xright<<" , "<<ydown<<" )\n"; 
			return s.str();
	}
	//initialisator om in te lezen via een file
	ChangeCeils(fstream *s){
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