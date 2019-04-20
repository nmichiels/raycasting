//***************************************************************************************************
// TGENERALS.H
// Task : kan meerdere parameters tegelijk in meerdere vakjes zetten
//***************************************************************************************************
// AUTEUR: KENNETH
//***************************************************************************************************

#ifndef GENERALS_H
#define GENERALS_H 1
#include "task.h"
#include <sstream> 
using namespace std; 
#include <iostream>
#include <fstream>

//een algemene task om op meerdere vakjes meerdere parameters te wijzigen
class Generals:public Task{
public:
	Generals(int xl, int xr,int yd,int yu, int n=-1, int e=-1, int s=-1,int w=-1,int floor=-1,int ceil=-1,int trigger=-1,int sprite=-1)
		:xleft(xl),xright(xr),ydown(yd),yup(yu),n(n),e(e),s(s),w(w),floor(floor),ceil(ceil),trigger(trigger),sprite(sprite){}

	void execute(struct TriggerData a){
		//controle en uitvoeren
		int xr=min(a.world->WorldHeight(),xright+1)-1;
		int yd=min(a.world->WorldWidth(),ydown+1)-1;
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
				if(floor!=-1)
					(*(a.world))[x][y].floor=floor;
				if(ceil!=-1)
					(*(a.world))[x][y].ceil=ceil;
				if(trigger!=-1)
					(*(a.world))[x][y].trigger=trigger;
				if(sprite!=-1)
					(*(a.world))[x][y].sprite=sprite;
		
			}
	}
	
	//wegschrijven
	string toString(){
		stringstream st;
		st<<"Generals"<< " coordLU( " <<xleft<< " , "<<yup<< " ) ,coordRU( "<<xright<<" , "<<ydown
			<<" ), north: "<<n<< " east: "<<e<<" south "<<s
			<<" west: "<< w<<" ,floor: "<<floor<<" ,ceil: "<<ceil<<" ,trigger: "<<trigger<< " ,sprite: "<<sprite<< " \n" ; 
		return st.str();
	}

	//initialisator om in te lezen via een file
	Generals(fstream *st){
		char *d=new char[222];
		(*st)>>d>>xleft>>d>>yup>>d>>d>>xright>>d>>ydown>>d>>d>>n>>d>>e>>d>>s>>d>>w>>d>>floor>>d>>ceil>>d>>trigger>>d>>sprite;
		delete[] d;
	}

private:
	int xright;
	int xleft;
	int yup;
	int ydown;
	int sprite;
	int floor;
	int ceil;
	int n;
	int e;
	int s;
	int w;
	int trigger;
};
#endif