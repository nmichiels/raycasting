//***************************************************************************************************
// TSETIMAGE.H
// Task : plaats een afbeelding op het scherm
//***************************************************************************************************
// AUTEUR: KENNETH
//***************************************************************************************************

#ifndef SETIMAGE_H
#define SETIMAGE_H 1
#include "task.h"
#include "ui_raycastingqt.h"
#include <QgraphicsPixmapItem>
#include <sstream> 
using namespace std; 
#include <iostream>
#include <fstream>

//plaatst een afbeelding op bepaalde coordinaten op het scherm
class SetImage: public Task 
{
public:
	SetImage(int t, int x=680, int y =20, int w=140,int h=270):texture(t),x(x),y(y),w(w),h(h){}

	void execute(TriggerData a){
		//een scene toekennen aan een picture
		QPixmap *t=new QPixmap;//(64,64);
		*t = t->fromImage(*(a.world->textures.get(texture)));
		QGraphicsPixmapItem *b=new QGraphicsPixmapItem(*t);
		QGraphicsScene *f=new QGraphicsScene();
		f->addItem(b);
		a.ui->pic->setGeometry(x,y,w,h);
		a.ui->pic->setScene(f);
		a.ui->pic->show();
		a.ui->pic->update();
	}
	
	//wegschrijven
	string toString(){
		stringstream s;
		s<<"SetImage"<< " to "<< texture<<" startcoords( "<<x<<" , "<<y <<" ) ,offset( "<<w<<" , "<< h <<" )\n"; 
		return s.str();
	}

	//initialisator om in te lezen via een file
	SetImage(fstream *s){
		char *d=new char[222];
		(*s)>>d>>texture>>d>>x>>d>>y>>d>>d>>h>>d>>w>>d;
		delete[] d;
	}
private:
	int texture;
	int x, y, h, w;
};

#endif