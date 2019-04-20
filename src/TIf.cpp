//***************************************************************************************************
// TIF.CPP
// Task : kan een vakje testen en een consequentie laten gebeuren
//***************************************************************************************************
// AUTEUR: KENNETH
//***************************************************************************************************

#include "TIf.h"

	void If::execute(TriggerData a){
		int toCheck;
		switch(subject)
		{
		case NORTH:
			toCheck=(*a.world)[x%a.world->WorldHeight()][y%a.world->WorldWidth()].north;
		break;
		case SOUTH:
			toCheck=(*a.world)[x%a.world->WorldHeight()][y%a.world->WorldWidth()].south;
		break;
		case EAST:
			toCheck=(*a.world)[x%a.world->WorldHeight()][y%a.world->WorldWidth()].east;
		break;
		case WEST:
			toCheck=(*a.world)[x%a.world->WorldHeight()][y%a.world->WorldWidth()].west;
		break;
		case TRIGGER:
			toCheck=(*a.world)[x%a.world->WorldHeight()][y%a.world->WorldWidth()].trigger;
		break;
		case SPRITE:
			toCheck=(*a.world)[x%a.world->WorldHeight()][y%a.world->WorldWidth()].sprite;
		break;
		case POSX:
			toCheck=a.viewer->x;
		break;
		case POSY:
			toCheck=a.viewer->y;
		break;
		default:
			toCheck=a.viewer->viewAngle;
			break;
		}
		switch(op){
			case BIG:
				if(toCheck>demand)
					task->execute(a);
				break;
			case SMALL:
				if(toCheck<demand)
					task->execute(a);
				break;
			default:
				if(toCheck==demand)
					task->execute(a);
				break;
		}



	}

	//wegschrijven
	string If::toString(){
		stringstream s;
		s<<"If " ; 
		switch(subject)
		{
		case SPRITE:
			s<<"SPRITE ";
			break;
		case NORTH:
			s<<"NORTH ";
			break;
		case EAST:
			s<<"EAST ";
			break;
		case SOUTH:
			s<<"SOUTH ";
			break;
		case WEST:
			s<<"WEST ";
			break;
		case POSX:
			s<<"POSX ";
			break;
		case TRIGGER:
			s<<"TRIGGER ";
			break;
		case POSY:
			s<<"POSY ";
			break;
			s<<"ANGLE ";
			break;
		}
		s<< x <<" "<<y<<" ";
		switch(op)
		{
		case EQUAL:
			s<<"EQUAL ";
			break;
		case BIG:
			s<<"BIG ";
			break;
		case SMALL:
			s<<"SMALL ";
			break;
		}
		s<<demand<<" "<<task->toString();
		return s.str();
	}
	//initialisator om in te lezen via een file
	If::If(fstream *s){
		char *d=new char[222];
		(*s)>>d;
		if(strcmp(d,"NORTH")==0)
			subject=NORTH;
		else if(strcmp(d,"EAST")==0)
			subject=EAST;
		else if(strcmp(d,"SOUTH")==0)
			subject=SOUTH;
		else if(strcmp(d,"WEST")==0)
			subject=WEST;
		else if(strcmp(d,"SPRITE")==0)
			subject=SPRITE;
		else if(strcmp(d,"TRIGGER")==0)
			subject=TRIGGER;
		else if(strcmp(d,"POSX")==0)
			subject=POSX;
		else if(strcmp(d,"POSY")==0)
			subject=POSY;
		else 
			subject=ANGLE;
		(*s)>>x>>y;
		(*s)>>d;
		if(strcmp(d,"SMALL")==0)
			op=SMALL;
		else if(strcmp(d,"EQUAL")==0)
			op=EQUAL;
		else 
			op=BIG;
		(*s)>>demand;
		
		char *b=new char[222];
		(*s)>>b;
			//zoek eerst volgens letter en daarna gedetailleerd
			//roep d bijbehorende constructor aan
			if(b[0]=='C'){
				if(strcmp(b,"ChangeCeil")==0){
					task=new ChangeCeil(s);
				}else if(strcmp(b,"ChangeCeils")==0){
					task=new ChangeCeils(s);
				}else if(strcmp(b,"ChangeFloor")==0){
					task=new ChangeFloor(s);
				}else if(strcmp(b,"ChangeFloors")==0){
					task=new ChangeFloors(s);
				}else if(strcmp(b,"ChangeSprite")==0){
					task=new ChangeSprite(s);
				}else if(strcmp(b,"ChangeSprites")==0){
					task=new ChangeSprites(s);
				}else if(strcmp(b,"ChangeTrigger")==0){
					task=new ChangeTrigger(s);
				}else if(strcmp(b,"ChangeTriggers")==0){
					task=new ChangeTriggers(s);
				}else if(strcmp(b,"ChangeWall")==0){
					task=new ChangeWall(s);
				}else if(strcmp(b,"ChangeWalls")==0){
					task=new ChangeWalls(s);
				}
			}else if(b[0]=='F'){
				if(strcmp(b,"FlipWall")==0){
					task=new FlipWall(s);
				}else if(strcmp(b,"FlipWalls")==0){
					task=new FlipWalls(s);
				}else if(strcmp(b,"FlipWalktrough")==0){
					task=new FlipWalktrough(s);
				}else if(strcmp(b,"FlipWalktroughs")==0){
					task=new FlipWalktroughs(s);
				}else if(strcmp(b,"FlipSprite")==0){
					task=new FlipSprite(s);
				}else if(strcmp(b,"FlipSprites")==0){
					task=new FlipSprites(s);
				}
			}else if(b[0]=='S'){
				if(strcmp(b,"SetWall")==0){
					task=new SetWall(s);
				}else if(strcmp(b,"SetWalls")==0){
					task=new SetWalls(s);
				}else if(strcmp(b,"SetWalktrough")==0){
					task=new SetWalktrough(s);
				}else if(strcmp(b,"SetWalktroughs")==0){
					task=new SetWalktroughs(s);
				}else if(strcmp(b,"SetSprite")==0){
					task=new SetSprite(s);
				}else if(strcmp(b,"SetSprites")==0){
					task=new SetSprites(s);
				}else if(strcmp(b,"SetImage")==0){
					task=new SetImage(s);
				}else if(strcmp(b,"SetColors")==0){
					task=new SetColors(s);
				}
			}else if(strcmp(b,"MoveViewer")==0){
				task=new MoveViewer(s);
			}else if(strcmp(b,"Wait")==0){
				task=new Wait(s);
			}else if(strcmp(b,"General")==0){
				task=new General(s);
			}else if(strcmp(b,"Generals")==0){
				task=new Generals(s);
			}else if(strcmp(b,"AddText")==0){
				task=new AddText(s);
			}else if(strcmp(b,"RemoveImage")==0){
				task=new RemoveImage(s);
			}else if(strcmp(b,"TriggerMultipleFlip")==0){
				task=new TriggerMultipleFlip(s);
			}else if(strcmp(b,"TriggerMultipleSet")==0){
				task=new TriggerMultipleSet(s);
			}else if(strcmp(b,"If")==0){
				task=new If(s);
			}else if(strcmp(b,"Music")==0){
				task=new Music(s);
			}else
				task=new AddText("trigger gefaald");


		delete[] d;
		delete[] b;
	}

