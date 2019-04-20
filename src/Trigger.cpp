//***************************************************************************************************
// TRIGGER.CPP
// Deze klasse houdt de data van 1 trigger bij
//***************************************************************************************************
// AUTEUR: KENNETH
//***************************************************************************************************

#include "Trigger.h"
#include <windows.h>
#include <fstream>
#include <sstream> 
using namespace std; 
#include <iostream>


struct info{
	struct TriggerData data;
	Trigger *trigger;
};


//**************************************************************************************************
// Constructor
//**************************************************************************************************
Trigger::Trigger(){
	//standaardopties
		triggered=true;
		allowTrigger=false;
		leftAngle=360;
		key=true;
		rightAngle=360;
		tasks=new Tasks();
	}


//**************************************************************************************************
// De trigger opstarten
//**************************************************************************************************
void Trigger::startTasks(bool key,Viewer *viewer,World *world,Ui::RaycastingQTClass *ui)//,RaycastingQT *ray)
{
	//indien de omstandighen goed zijn wordt de data opgenaakt en de triggerthread gestart
	if(this->key==key && viewer->viewAngle>=this->leftAngle && viewer->viewAngle<=this->rightAngle && !(triggered && !allowTrigger))
	{
		triggered=true;
		data.ui=ui;
		data.viewer=viewer;
		data.world=world;
		start();
	}
}
//**************************************************************************************************


//**************************************************************************************************
// De eigenlijke thread waarin triggers gerunt worden
//**************************************************************************************************
void Trigger::run(){
	if (tasks->empty())
		return;
	//de tasks overlopen
	for(map<int, Task*,smaller>::iterator it = tasks->begin(); it != tasks->end(); ++it)
    {
		it->second->execute(data);
    }
}

//**************************************************************************************************


//**************************************************************************************************
// Voeg 1 task toe
//**************************************************************************************************
void Trigger::addTask(int i, Task* tsk){
	tasks->add(i, tsk);
}
//**************************************************************************************************


//**************************************************************************************************
// Verwijder 1 task
//**************************************************************************************************
void Trigger::delTask(int i){
	tasks->del(i);
}
//**************************************************************************************************


//**************************************************************************************************
// Leest een trigger in vanuit een file
//**************************************************************************************************
Trigger::Trigger(fstream *s){
	tasks=new Tasks();
	char *b=new char[222];
	//opties
	(*s)>>b>>key>>b>>allowTrigger>>b>>triggered>>b;
	(*s)>>leftAngle>>b;
	(*s)>>rightAngle;
	//tasks inlezen
	tasks->fromString(s);
	delete[] b;
}
//**************************************************************************************************


//**************************************************************************************************
// Maak een string op die de trigger voorstelt
//**************************************************************************************************
string Trigger::toString()
{
	stringstream s;
	s<<"Key_needed? "<<key<<" allow_trigger_more_then_once? "<<allowTrigger<<" triggered_once_already? "<<triggered
				<<"\nrequired_leftAngle? "<<leftAngle<<" required_rightAngle? "<<rightAngle<<"\n";
	//de rest inlezen langs de tasks
	s<<tasks->toString();
	return s.str();
}
//**************************************************************************************************
