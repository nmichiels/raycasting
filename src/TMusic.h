//***************************************************************************************************
// TMUSIC.H
// Task : speelt een geluid af
//***************************************************************************************************
// AUTEUR: KENNETH
//***************************************************************************************************

#ifndef MUSIC_H
#define MUSIC_H 1
#include "task.h"
#include <sstream> 
using namespace std; 
#include <iostream>
#include <fstream>
#include <QSound.h>
#include <QString.h>

//Speelt muziek af
class Music: public Task 
{
public:
	Music(char *a):music(a){}

	void execute(TriggerData a){
		QSound::play( music);
	}
	//wegschrijven
	string toString(){
		stringstream s;
		s<<"Music"<<  music.toStdString()<<"\n"; 
		return s.str();
	}
		//initialisator om in te lezen via een file
	Music(fstream *s){
		char *d=new char[222];
		(*s)>>d;
		music=d;
		delete[] d;
	}
private:
	QString music;
};

#endif