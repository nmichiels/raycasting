//***************************************************************************************************
// TADDTEXT.H
// Task : tekst uitprinten
//***************************************************************************************************
// AUTEUR: KENNETH
//***************************************************************************************************

#ifndef ADDTEXT_H
#define ADDTEXT_H 1
#include "task.h"
#include "ui_raycastingqt.h"
#include <sstream> 
using namespace std; 
#include <iostream>
#include <fstream>
#include <stdlib.h>

//toevoegen van tekst in het kader onderaan
class AddText: public Task 
{
public:
	AddText(QString qstr):qstr(qstr){}

	void execute(TriggerData a){
	a.ui->text->setText(qstr);
	a.ui->text->update();		
	}
	
	string toString(){
		stringstream s;
		s<<"AddText\n"<< qstr.toStdString() << "\n~ \n"; 
		return s.str();
	}
	//initialisator om in te lezen via een file
	AddText(fstream *s)
	{
		char *a=new char[5000];
		(*s).getline(a,5000,'\n');
		(*s).getline(a,5000,'~');
		qstr+=a;
	}
private:
	QString qstr;
};

#endif