//***************************************************************************************************
// TCHANGEWALL.H
// Task : een muur laten veranderen
//***************************************************************************************************
// AUTEUR: KENNETH
//***************************************************************************************************

#ifndef CHANGEWALL_H
#define CHANGEWALL_H 1
#include "task.h"

#include <sstream> 
using namespace std; 
#include <iostream>
#include <fstream>

//flipt een wall om (aan/uit zetten)
//als ergens -1 wordt meegegeven wordt de waarde niet gewijzigd
class ChangeWall: public Task 
{
public:
	ChangeWall(int x, int y, int n, int e,int s,int w):x(x),y(y),n(n),e(e),s(s),w(w){}

	void execute(TriggerData a){
		//controle en uitvoeren
		if(x>=0 && y>=0 && x<a.world->WorldHeight() && y < a.world->WorldWidth())
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
	
	//wegschrijven
	string toString(){
		stringstream se;
		se<<"ChangeWall"<< " tex_n: "<< n << " tex_e: "<< e<< " tex_s: "<< s<< " tex_w: "<< w << " ,coord( " <<x<< " , "<<y<< " )\n"; 
		return se.str();
	}

	//initialisator om in te lezen via een file
	ChangeWall(fstream *st){
		char *d=new char[222];
		(*st)>>d>>n>>d>>e>>d>>s>>d>>w>>d>>x>>d>>y>>d;
		delete[] d;
	}
private:
	int x;
	int y;
	//north east south and west wall
	int n;
	int e;
	int s;
	int w;
	
};

#endif