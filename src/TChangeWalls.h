//***************************************************************************************************
// TCHANGEWALLS.H
// Task : meerdere muren laten veranderen
//***************************************************************************************************
// AUTEUR: KENNETH
//***************************************************************************************************

#ifndef CHANGEWALLS_H
#define CHANGEWALLS_H 1
#include "task.h"
#include <sstream> 
using namespace std; 
#include <iostream>
#include <fstream>

#include <sstream> 
using namespace std; 
#include <iostream>
#include <fstream>

//past meerdere vakjes aan (de muur)
//-1 staat voor niet aanpassen
class ChangeWalls: public Task 
{
public:
	ChangeWalls(int xr, int xl,int yu,int yd, int n, int e,int s,int w):xright(xr),xleft(xl),yup(yu),ydown(yd),n(n),e(e),s(s),w(w){}

	void execute(TriggerData a){
		//controle en uitvoeren
		int xr=min(a.world->WorldHeight(),xright)-1;
		int yd=min(a.world->WorldWidth(),ydown)-1;
		for(int x=max(0,xleft);x <= xr ;x++)
			for(int y=max(0, yup);y <= yd;y++)
			{
			if(n!=-1)
				(*(a.world))[x][y].north=n;
			if(e!=-1)	
				(*(a.world))[x][y].east=e;
			if(s!=-1)
				(*(a.world))[x][y].south=s;
			if(w!=-1)
				(*(a.world))[x][y].west=w;
			}
	}
	
	//wegschrijvem
	string toString(){
		stringstream se;
		se<<"ChangeWalls"<< " tex_n: "<< n << " tex_e: "<< e<< " tex_s: "<< s<< " tex_w: "<< w << " ,coordLU( " <<xleft<< " , "<<yup<< " ) ,coordRU( "<<xright<<" , "<<ydown<<" )\n"; 
		return se.str();
	}

	//initialisator om in te lezen via een file
	ChangeWalls(fstream *st){
		char *d=new char[222];
		(*st)>>d>>n>>d>>e>>d>>s>>d>>w>>d>>xleft>>d>>yup>>d>>d>>xright>>d>>ydown>>d;
		delete[] d;
	}
private:
	int xright;
	int xleft;
	int yup;
	int ydown;
	//north east south and west wall
	int n;
	int e;
	int s;
	int w;
};

#endif