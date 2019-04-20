//***************************************************************************************************
// TASKS.CPP
// Task: een bundeling van tasks die kunnen worden uitgevoerd 
//***************************************************************************************************
// AUTEUR: KENNETH
//***************************************************************************************************

#include "Tasks.h"


//**************************************************************************************************
// Maak een string op die de trigger voorstelt
//**************************************************************************************************
Tasks::~Tasks(){
	if(!task.empty())
		{
			task.clear();
		}
	}
//**************************************************************************************************


//**************************************************************************************************
// Laad de tasks in vanuit een string 
//**************************************************************************************************
void Tasks::fromString(fstream *s){
	int i;
	char *b=new char[500];
	(*s)>>i;
	//zolang de lijst niet stopt
	while(i>=0)
	{
		//commentaar
		if(i==0)
			s->getline(b,500);
		else{
			(*s)>>b;
			//zoek eerst volgens letter en daarna gedetailleerd
			//roep d bijbehorende constructor aan
			if(b[0]=='C'){
				if(strcmp(b,"ChangeCeil")==0){
					task[i]=new ChangeCeil(s);
				}else if(strcmp(b,"ChangeCeils")==0){
					task[i]=new ChangeCeils(s);
				}else if(strcmp(b,"ChangeFloor")==0){
					task[i]=new ChangeFloor(s);
				}else if(strcmp(b,"ChangeFloors")==0){
					task[i]=new ChangeFloors(s);
				}else if(strcmp(b,"ChangeSprite")==0){
					task[i]=new ChangeSprite(s);
				}else if(strcmp(b,"ChangeSprites")==0){
					task[i]=new ChangeSprites(s);
				}else if(strcmp(b,"ChangeTrigger")==0){
					task[i]=new ChangeTrigger(s);
				}else if(strcmp(b,"ChangeTriggers")==0){
					task[i]=new ChangeTriggers(s);
				}else if(strcmp(b,"ChangeWall")==0){
					task[i]=new ChangeWall(s);
				}else if(strcmp(b,"ChangeWalls")==0){
					task[i]=new ChangeWalls(s);
				}
			}else if(b[0]=='F'){
				if(strcmp(b,"FlipWall")==0){
					task[i]=new FlipWall(s);
				}else if(strcmp(b,"FlipWalls")==0){
					task[i]=new FlipWalls(s);
				}else if(strcmp(b,"FlipWalktrough")==0){
					task[i]=new FlipWalktrough(s);
				}else if(strcmp(b,"FlipWalktroughs")==0){
					task[i]=new FlipWalktroughs(s);
				}else if(strcmp(b,"FlipSprite")==0){
					task[i]=new FlipSprite(s);
				}else if(strcmp(b,"FlipSprites")==0){
					task[i]=new FlipSprites(s);
				}
			}else if(b[0]=='S'){
				if(strcmp(b,"SetWall")==0){
					task[i]=new SetWall(s);
				}else if(strcmp(b,"SetWalls")==0){
					task[i]=new SetWalls(s);
				}else if(strcmp(b,"SetWalktrough")==0){
					task[i]=new SetWalktrough(s);
				}else if(strcmp(b,"SetWalktroughs")==0){
					task[i]=new SetWalktroughs(s);
				}else if(strcmp(b,"SetSprite")==0){
					task[i]=new SetSprite(s);
				}else if(strcmp(b,"SetSprites")==0){
					task[i]=new SetSprites(s);
				}else if(strcmp(b,"SetImage")==0){
					task[i]=new SetImage(s);
				}else if(strcmp(b,"SetColors")==0){
					task[i]=new SetColors(s);
				}
			}else if(strcmp(b,"MoveViewer")==0){
				task[i]=new MoveViewer(s);
			}else if(strcmp(b,"Wait")==0){
				task[i]=new Wait(s);
			}else if(strcmp(b,"General")==0){
				task[i]=new General(s);
			}else if(strcmp(b,"Generals")==0){
				task[i]=new Generals(s);
			}else if(strcmp(b,"AddText")==0){
				task[i]=new AddText(s);
			}else if(strcmp(b,"RemoveImage")==0){
				task[i]=new RemoveImage(s);
			}else if(strcmp(b,"TriggerMultipleFlip")==0){
				task[i]=new TriggerMultipleFlip(s);
			}else if(strcmp(b,"TriggerMultipleSet")==0){
				task[i]=new TriggerMultipleSet(s);
			}else if(strcmp(b,"If")==0){
				task[i]=new If(s);
			}else if(strcmp(b,"Music")==0){
				task[i]=new Music(s);
			}else
				return;
		}
		(*s)>>i;
	}
	delete[] b;
}
//**************************************************************************************************


//**************************************************************************************************
// tasks wgschrijven naar een string 
//**************************************************************************************************
string Tasks::toString(){
	stringstream s;
	map<int, Task*,smaller>::iterator it;
	for(it=task.begin();it !=task.end();it++)
	{
		//per task individueel geval
		s<<(*it).first<< " "<< (*it).second->toString();
	}			
	return s.str();
}
//**************************************************************************************************
