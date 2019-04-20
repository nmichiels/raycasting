//***************************************************************************************************
// TWAIT.H
// Task : laat een pauze tussen twee tasks
//***************************************************************************************************
// AUTEUR: KENNETH
//***************************************************************************************************

#ifndef WAIT_H
#define WAIT_H 1
#include "task.h"
#include <windows.h>
#include <sstream> 
using namespace std; 
#include <iostream>
#include <fstream>

//las een puze in tussen 2 tasks
class Wait: public Task 
{
public:
	Wait(int sleeptime):time(sleeptime){}

	void execute(struct TriggerData a){
		Sleep(time);
	}
	
	//wegschrijven
	string toString(){
		stringstream s;
		s<<"Wait"<< " time: "<<time<<"\n"; 
		return s.str();
	}

	//initialisator om in te lezen via een file
	Wait(fstream *s){
		char *d=new char[222];
		(*s)>>d>>time;
		delete[] d;
	}
private:
	int time;
};

#endif