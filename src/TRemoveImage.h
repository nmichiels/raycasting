//***************************************************************************************************
// TREMOVEIMAGE.H
// Task : de afbeelding die met een andere trigger geset is weghalen
//***************************************************************************************************
// AUTEUR: KENNETH
//***************************************************************************************************

#ifndef REMOVEIMAGE_H
#define REMOVEIMAGE_H 1
#include "task.h"

#include <sstream> 
using namespace std; 
#include <iostream>
#include <fstream>

//verwijderd een afbeelding van het scherm
class RemoveImage: public Task 
{
public:
	RemoveImage(){}

	void execute(TriggerData a){
		a.ui->pic->hide();
		a.ui->pic->setGeometry(680,20,5,5);
	}
	
	//wegschrijven
	string toString(){
		stringstream s;
		s<<"RemoveImage \n"; 
		return s.str();
	}
	//initialisator om in te lezen via een file
	RemoveImage(fstream *st){
		
	}
};

#endif