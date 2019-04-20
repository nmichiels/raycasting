//***************************************************************************************************
// TGENERAL.H
// Task : kan meerdere parameters tegelijk in een vakje zetten
//***************************************************************************************************
// AUTEUR: KENNETH
//***************************************************************************************************

#ifndef GENERAL_H
#define GENERAL_H 1
#include "task.h"
#include <sstream> 
using namespace std; 
#include <iostream>
#include <fstream>

//een algemene task die meerdere parameters tegelijk in een vakje kan zetten
//-1 staat voor niets wijzigen
class General:public Task{
public:
	General(int x, int y, int n=-1, int e=-1, int s=-1,int w=-1,int floor=-1,int ceil=-1,int trigger=-1,int sprite=-1)
		:x(x),y(y),n(n),e(e),s(s),w(w),floor(floor),ceil(ceil),trigger(trigger),sprite(sprite){}

	void execute(struct TriggerData a){
		if(x<a.world->WorldHeight() && y<a.world->WorldWidth() && x>=0 && y>=0)
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
		st<<"General"<< " coord( "<< x << " , " <<y<< " ), north: "<<n<< " east: "<<e<<" south "<<s	<<" west: "<< w<<" ,floor: "<<floor<<" ,ceil: "<<ceil<<" ,trigger: "<<trigger<< " ,sprite: "<<sprite<< " \n" ; 
		return st.str();
	}

	//initialisator om in te lezen via een file
	General(fstream *so){
		char *d=new char[222];
		(*so)>>d>>x>>d>>y>>d>>d>>n>>d>>e>>d>>s>>d>>w>>d>>floor>>d>>ceil>>d>>trigger>>d>>sprite;
		delete[] d;
	}

private:
	int x;
	int y;
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