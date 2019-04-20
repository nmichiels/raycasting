//***************************************************************************************************
// TASKS.H
// Task: een bundeling van tasks die kunnen worden uitgevoerd 
//***************************************************************************************************
// AUTEUR: KENNETH
//***************************************************************************************************

#ifndef TASKS_H
#define TASKS_H 1
#include "Task.h"
#include "TAddText.h"
#include "TSetImage.h"
#include "TChangeFloors.h"
#include "TChangeCeils.h"
#include "TChangeFloor.h"
#include "TChangeCeil.h"
#include "TChangeWall.h"
#include "TChangeWalls.h"
#include "TChangeSprite.h"
#include "TChangeSprites.h"
#include "TChangeTrigger.h"
#include "TChangeTriggers.h"
#include "TmoveViewer.h"
#include "TWait.h"
#include "TFlipWall.h"
#include "TFlipWalls.h"
#include "TSetWall.h"
#include "TSetWalls.h"
#include "TFlipSprite.h"
#include "TFlipSprites.h"
#include "TSetSprite.h"
#include "TSetSprites.h"
#include "TFlipWalktrough.h"
#include "TFlipWalktroughs.h"
#include "TSetWalktrough.h"
#include "TSetWalktroughs.h"
#include "TSetColors.h"
#include "TGeneral.h"
#include "TGenerals.h"
#include "TRemoveImage.h"
#include "TTriggerMultipleFlip.h"
#include "TTriggerMultipleSet.h"
#include "TIf.h"
#include "TMusic.h"

#include <map>
#include <sstream> 
using namespace std; 
#include <iostream>
#include <fstream>

//comparator om de tasks in volgorde te laten lopen
#ifndef SMALLER
#define SMALLER 1
struct smaller
{
  bool operator()( int i1, int i2) const
  {
    return i1<i2;
  }
};

#endif

class Tasks
{
	public:	
		Tasks(){;}
		~Tasks();
		//iterators om de tasks te doorlopen
		map<int, Task*,smaller>::iterator begin(){return task.begin();}
		map<int, Task*,smaller>::iterator end(){return task.end();}
		//test op empty
		bool empty(){return task.empty();}
		//toeveoegen en verwijderen
		void add(int i, Task *tas){	task[i]=tas;}
		void del(int i){task[i]=task[0];}
		//naar files wegschrijven en inladen
		void fromString(fstream *s);
		string toString();

private:
		//container voor de tasks
		map<int,Task*,smaller> task;	
};

#endif