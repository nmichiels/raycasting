//***************************************************************************************************
// TRIGGERS.CPP
// Deze klasse houdt alle triggers bij
//***************************************************************************************************
// AUTEUR: KENNETH
//***************************************************************************************************

#include "Triggers.h"


//**************************************************************************************************
// constructor
//**************************************************************************************************
	Triggers::Triggers(){
		tricky[0]=new Trigger();
	}
//**************************************************************************************************


//**************************************************************************************************
// Voegt een trigger toe.
//**************************************************************************************************
	void Triggers::add(int nr,Trigger *a){
		tricky[nr]=a;
	}
//**************************************************************************************************


//**************************************************************************************************
// Geeft een trigger terug
//**************************************************************************************************
	Trigger* Triggers::get(int i){
		//controle op null, indien 0 wordt de standaardtrigger teruggegeven
		if(tricky[i]!=0)
			return tricky[i];
		return tricky[0];
	}
//**************************************************************************************************


//**************************************************************************************************
// destructor
//**************************************************************************************************
	Triggers::~Triggers(){
		if(!tricky.empty())
		{
			tricky.clear();
		}
	}
//**************************************************************************************************


//**************************************************************************************************
// leest in vanuit een file
//**************************************************************************************************
	void Triggers::fromString(fstream *s){
		//legen indien nodig
		if(!tricky.empty()){
			tricky.clear();
			tricky[0]=new Trigger();
		}
		int i;
		(*s)>>i;
		char *b=new char[222];
		//tot de lijst afgesloten wordt met -1...
		while(i!=-1)
		{
			printf("Trigger kan beginnen\n");
			//lees de nieuwe trigger in
			tricky[i]=new Trigger(s);
			(*s)>>i;
		}
	}
//**************************************************************************************************


//**************************************************************************************************
// Geeft een string met de triggerslijst terug.
//**************************************************************************************************
	string Triggers::toString(){
		stringstream s;
		map<int,Trigger*,smaller>::iterator it;
		//maakt de lijst van triggers op en sluit af met -1
		for(it=tricky.begin() ;it != tricky.end() ;it++)
		{
			s<<(*it).first<<"\n";
			s<<(*it).second->toString();
			s<<"-1\n";
			s<<"\n";
		}
		//ook de file wordt met -1 afgesloten
		s<<"-1";
		return s.str();
	}
//**************************************************************************************************