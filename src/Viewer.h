//***************************************************************************************************
// VIEWER.H
// Deze klasse stelt een camera voor in de wereld
//***************************************************************************************************
// AUTEUR: NICK
//***************************************************************************************************

#ifndef VIEWER_H
#define VIEWER_H 1

#include <sstream> 
using namespace std; 
#include <iostream>
#include <fstream>

class Viewer
{
	public:
		Viewer(int x=96, int y=96, double FOV=66, double viewAngle=315,  int looking=240, int DPP=476) : x(x), y(y), FOV(FOV), viewAngle(viewAngle), looking(looking),DPP(DPP){}
		~Viewer(){}
		int x;
		int y;
		double FOV;
		double viewAngle;
		int looking;
		int DPP;//distanceProjectionPlane

		void Reset();

		string toString(){
			stringstream s;
			s<<"Viewer:( "<<x<<" , "<<y<< " ) FOV: "<< FOV <<" ,viewAngle: "<<viewAngle<<" looking: "<<looking<<" DPP: "<<DPP<<" \n";
			return s.str();
		}
		void fromString(fstream* s){
			char* d=new char[222];
			(*s)>> d>>x >>d >>y >>d >>d >>FOV >>d>>viewAngle  >>d >>looking>>d>>DPP ;		
			//return s;
		}
};


#endif