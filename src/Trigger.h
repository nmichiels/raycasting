//***************************************************************************************************
// TRIGGER.H
// Deze klasse houdt de data van 1 trigger bij
//***************************************************************************************************
// AUTEUR: KENNETH
//***************************************************************************************************

#ifndef TRIGGERTEST_H
#define TRIGGERTEST_H 1

#include <map>
#include <string>
#include "World.h"
#include "Tasks.h"
#include "Viewer.h"
#include "windows.h"
#include <sstream> 
using namespace std; 
#include <iostream>
#include <fstream>
#include <QThread>




class Trigger:public QThread{
public:
	//het aanmaken van een nieuwe lege trigger
	Trigger(double leftAngle, double rightAngle,  bool allowTrigger=false, bool triggered=false )
		:leftAngle(leftAngle),rightAngle(rightAngle), allowTrigger(allowTrigger), triggered(triggered){tasks=new Tasks();}
	//inladen vie een file
	Trigger(fstream *s);
	//de standaardtrigger
	Trigger();
	//de thread die de tasks laat runnen
	void run();
	//de startfunctie voor een trigger
	void startTasks(bool key, Viewer *viewer,World *world ,Ui::RaycastingQTClass *ui);
	//opslaan
	string toString();
	//tasks toevoegen of verwijderen
	void addTask(int i, Task* tsk);
	void delTask(int i);

private:
	double leftAngle;	//geeft de verst toegestane linkerhoek
	double rightAngle;	//geeft de verst toegestane rechterhoek
	Tasks *tasks;		//tasklijst
	bool key;			//key nodig of niet?
	bool triggered;		//reeds getriggered of niet?
	bool allowTrigger;	//trigger toestaan om meerdere keren af te gaan?
	TriggerData data;	//data gebruikt door een thread

};

#endif