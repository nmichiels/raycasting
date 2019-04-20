//***************************************************************************************************
// TEXTURES.H
// Een container om textures te beheren
//***************************************************************************************************
// AUTEUR: KENNETH
//***************************************************************************************************

#ifndef TEXTURES_H
#define TEXTURES_H 1

#include <map>
#include <vector>
#include <qstring>
#include <qimage>
#include <qrgb.h>
#include <sstream> 
#include <iostream>
#include <fstream>
using namespace std; 
//struct die de imagedata bij houdt
class img{
	public:
		img(){large=NULL; medium=NULL; image=NULL; small=NULL;}
		void Clear()
		{
			delete(large);
			delete(medium);
			delete(image);
			delete(small);
		}
		//verschillende groottes voor verschillende afstanden
		QImage *large;
		QImage *medium;
		QImage *image;
		QImage *small;
		QString pad;
};


class Textures{
public:
	Textures();
	~Textures();
	//textures toevoegen
	void add(int nr, QString pad);
	void add(int nr, char* pad);
	//een texture verwijderen
	void del(int i);
	//textures opvragen
	QImage* get(int i);
	QImage* gets(int i);
	QImage* getm(int i);
	QImage* getl(int i);
	QImage* getdist(int i, int distance);
	QImage* getdist2(int i, int distance);
	QString getUrl(int i);
	//extras
	QVector<QRgb>  colortable(int i);
	QRgb px(int i, int x, int y);
	int getSize();
	//in en uit files laden 
	void fromString(fstream *s);
	string toString();
	//hulp voor de destructor
	void Clear(void);
private:
	//de vector die de afbeeldingen bijhoudt
	vector<img> tex;
};



#endif	
