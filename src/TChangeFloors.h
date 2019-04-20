//***************************************************************************************************
// TCHANGEFLOORS.H
// Task : meerdere vloer textures laten veranderen
//***************************************************************************************************
// AUTEUR: KENNETH
//***************************************************************************************************

#ifndef CHANGEFLOORS_H
#define CHANGEFLOORS_H 1
#include "task.h"
#include <sstream> 
using namespace std; 
#include <iostream>
#include <fstream>
//past de vloer aan in meerdere vakjes (rechthoek)
class ChangeFloors: public Task 
{
public:
	ChangeFloors(int xl, int xr,int yu,int yd,int t):xleft(xl),xright(xr),yup(yu),ydown(yd),texture(t){}

	void execute(struct TriggerData a){
		//controle en uitvoeren
		int xr=min(a.world->WorldHeight(),xright+1)-1;
		int yd=min(a.world->WorldWidth(),ydown+1)-1;
		for(int x=max(0,xleft);x <= xr ;x++)
			for(int y=max(0, yup);y <= yd;y++)
				(*(a.world))[x][y].floor=texture;
		//a.world->read=false;
		
	}
	
	//wegschrijven
	string toString(){
		stringstream s;
		s<<"ChangeFloors"<< " to_texture "<< texture << " ,coordLU( " <<xleft<< " , "<<yup<< " ) ,coordRU( "<<xright<<" , "<<ydown<<" )\n"; 
			return s.str();

	}
	//initialisator om in te lezen via een file
	ChangeFloors(fstream *s){
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