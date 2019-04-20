//***************************************************************************************************
// ENGINE.H
// De engine voor de raycasing. Het algoritme van raycasting + sprites wordt hier uitgerekend en 
// uitgetekend
//***************************************************************************************************
// AUTEUR: KENNETH & NICK
// NICK: Basis van draw functie voor muren en vloeren
// KENNETH: Alles verder uitgewerkt en geoptimaliseerd, vloeren en plafond aangepast en symmetrisch
//			gemaakt, sprites uittekenen, triggers aanroepen, ...
//***************************************************************************************************

#ifndef ENGINE_H
#define ENGINE_H 1

#include <stdio.h>
#include <math.h>
#include<conio.h>
#include <QTimer>
#include <QThread>

// applicatie specifieke includes
#include "World.h"
#include "Viewer.h"
#include "Screen.h"
#include "Triggers.h"
#include "Textures.h"
#include "Navigation.h"
#include "ui_raycastingqt.h"
#include "ui_TexturesWindow.h"

#define PI 3.14159265
#define P180 0.01745329
#define i180P 57.2967795



class Engine : public QThread
{
	Q_OBJECT
	public:	
		
		Engine(World* world, Viewer* viewer, Navigation *navigation,Ui::RaycastingQTClass *ui,Triggers *triggers );
		// wereld en camera terug opnieuw instellen
		void Refresh(World* world, Viewer* viewer){m_World = world; m_Viewer=viewer;}
		virtual ~Engine();

	public slots:
			void Redraw(void);

	private:
		//wereldonderdelen
		Viewer* m_Viewer;
		World* m_World;
		Navigation* m_Navigation;
		Triggers *m_Triggers;
		//refresh
		QTimer* m_Timer;
		//ui
		Ui::RaycastingQTClass *m_Ui;
		//oude positie om te controleren of er een nieuwe trigger af moet gaan
		int oldX;
		int oldY;
		//afstand van de sprites tov de viewer, om muren en plafonds die dichter staan zeker uit
		//te tekenen
		int SpriteDist;
		//check om niks te overschrijven in het canvas
		bool *done;
		void run();
		enum sort{HORIZONTALLY, VERTICALLY}; // geeft aan welke soort van botsing er is
		enum direction{RIGHT, FIRST_QUADRANT, UP, SECOND_QUADRANT, LEFT, THIRD_QUADRANT, DOWN, FOURTH_QUADRANT} m_RayDirection;

		// Raycast-algortime functies
		double Cast(double angle, sort &sortCasting, int &wallGridX, int &wallGridY, double &wallX, double &wallY,int &i);
		void floorCeilSymCast(double &angle, int intersection, int &column);
		void floorCeilAsCast(double &angle, int intersection1,int intersection2, int &column);
		void Sprite(double &angle,  int &wallGridX, int &wallGridY, double &wallX, double &wallY,int &i);
		void wallCast(double &wallX,double &wallY,int &wallGridX,int &wallGridY,int &projectedSliceHeight,sort &sortCasting,int &i);

		// interpreteert de richting van de straal
		void InterpretRayDirection(double angle); 
		//neemt de juiste pixel van een texture en plaats deze op positie (i, j)
		QImage* getWallTexture(int wallGridX, int wallGridY,  sort sortCasting,int distance);
		//extra variabelen om kleuren aan te passen
		bool color;
		int r,g,b;

	signals:
			void ScreenUpdate();
	public slots:
			void triggerCheck();
};
	



#endif