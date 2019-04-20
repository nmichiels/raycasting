//***************************************************************************************************
// TMOVEVIEWER.H
// Task : een viewer verplaatsen
//***************************************************************************************************
// AUTEUR: KENNETH
//***************************************************************************************************

#ifndef MOVEVIEWER_H
#define MOVEVIEWER_H 1
#include "task.h"
#include <sstream> 
using namespace std; 
#include <iostream>
#include <fstream>


//een task om de viewer te verplaatsen
//-1 staat voor niets doen als relative uitstaat, anders staat 0 voor niets doen
//bij relative worden waardes bepaald ten opzichte van de huidige positie
//anders absoluut
class MoveViewer: public Task 
{
public:
	MoveViewer(bool relative,int x, int y, double viewAngle,int looking, int DPP, int FOV):relative(relative),x(x),y(y), viewAngle(viewAngle), looking(looking), FOV(FOV), DPP(DPP){}

	void execute(TriggerData a){
		//controles+uitvoeren
		if(relative){
				a.viewer->x=abs(a.viewer->x+x%(a.world->WorldHeight()*64));
				a.viewer->y=abs(a.viewer->y+y%(a.world->WorldWidth()*64));
				a.viewer->looking=abs((int)(a.viewer->looking+looking)%480);
				a.viewer->viewAngle=abs((int)(a.viewer->viewAngle+viewAngle)%360);
				a.viewer->DPP=abs((int)(a.viewer->DPP+DPP)%10000);
				a.viewer->FOV=abs((int)(a.viewer->FOV+FOV)%180);
		}
		else if(x>=64 && y>=64 && x<a.world->WorldWidth()*64 && y < a.world->WorldHeight()*64)
		{
			if(x!=-1)
				a.viewer->x=x%(a.world->WorldHeight()*64);
			if(y!= -1)
				a.viewer->y=y%(a.world->WorldWidth()*64);
			if(looking!= -1)
				a.viewer->looking=(int)looking%480;
			if(viewAngle != -1)
				a.viewer->viewAngle=(int)viewAngle%360;
			if(DPP != -1)
				a.viewer->DPP=abs(DPP);
			if(FOV != -1)
				a.viewer->FOV=abs(FOV);
			
		}
	}
	
	//wegschrijven
	string toString(){
		stringstream s;
		s<<"MoveViewer"<< " relative? "<<relative <<" to_pos( "<< x << " , "<< y<< " ) ,viewAngle: "<< viewAngle << " ,looking: " <<looking<<" ,DistanceProjectionPlane: "<<DPP<<" ,FieldOfView: "<<FOV<<"\n"; 
		return s.str();
	}

	//initialisator om in te lezen via een file
	MoveViewer(fstream *s){
		char *d=new char[222];
		(*s)>>d>>relative>>d>>x>>d>>y>>d>>d>>viewAngle>>d>>looking>>d>>DPP>>d>>FOV;
		delete[] d;
	}
private:
	bool relative;
	int x;
	int y;
	double viewAngle;
	int looking;
	int DPP;//distanceprojectionplane
	int FOV;//field of view
};

#endif