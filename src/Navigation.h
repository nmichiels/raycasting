//***************************************************************************************************
// NAVIGATION.H
// Beheert het berekenen en uittekenen van de minimap
//***************************************************************************************************
// AUTEUR: NICK & KENNETH
// Standaard navigatie: Nick
// Collision detection: Kenneth
//***************************************************************************************************

#ifndef NAVIGATIE_H
#define NAVIGATIE_H 1

#include <QtGui/QMainWindow>
#include <QKeyEvent>
#include <math.h>

#include "World.h"
#include "Viewer.h"
#include "Screen.h"


#define PI 3.14159265

class Navigation : public QObject
{
	Q_OBJECT
	public:	
		Navigation(){};
		Navigation(World* world, Viewer* viewer);//, Screen* scr);
		~Navigation();

		void SetViewer(Viewer* v){viewer = v;}
		void SetWorld(World* w){world = w;}

		void keyPressEvent ( QKeyEvent * e );
		void keyReleaseEvent(QKeyEvent *e);
		void mouseMoveEvent(QMouseEvent *e);


	public slots:
			void Update(void);

	private:
		World* world;
		Viewer* viewer;
		//Screen* scr;


		int moveSpeed;
		int rotateSpeed;
		int strafeSpeed;
		int lookupSpeed;

		//int mouseX;
		//int mouseY;
		//void setMousePos( QPoint p ){ mouseX = p.x(); mouseY = p.y(); }
		bool isMouseTracking; // deze bepaalt of de muis kan gebruikt worden om te draaien of niet
		
		void Rotate(int angle);
		void Move_Up();
		void Strafe();
		void lookUpDown();
		
};
		


#endif