//***************************************************************************************************
// TIF.H
// Task : kan een vakje testen en een consequentie laten gebeuren
//***************************************************************************************************
// AUTEUR: KENNETH
//***************************************************************************************************

#ifndef IF_H
#define IF_H 1
#include "task.h"
#include <sstream> 
using namespace std; 
#include <iostream>
#include <fstream>

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
#include "TMusic.h"

enum part{NORTH,EAST,SOUTH,WEST,TRIGGER,SPRITE,POSX,POSY,ANGLE};
enum oper{SMALL,EQUAL,BIG};

//doet eerst een iftest en voert daarna pas een opdracht uit
class If: public Task 
{
public:
	If(int x, int y):x(x),y(y){}

	void execute(TriggerData a);

	//wegschrijven
	string toString();
	//initialisator om in te lezen via een file
	If(fstream *s);
private:
	//de eis
	int demand;
	//bijgevoegde task indien succes
	Task* task; 
	//de coördinaten
	int x,y;
	//te controleren deel
	part subject;
	//operator
	oper op;
};

#endif