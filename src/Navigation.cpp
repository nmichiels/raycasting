//***************************************************************************************************
// NAVIGATION.CPP
// Beheert het berekenen en uittekenen van de minimap
//***************************************************************************************************
// AUTEUR: NICK & KENNETH
// Standaard navigatie: Nick
// Collision detection: Kenneth
//***************************************************************************************************

#include "Navigation.h"
#include <iostream>

//** Constructor *****************************************************************************************
Navigation::Navigation(World* world, Viewer* viewer) : world(world), viewer(viewer)
{
	rotateSpeed = 0;
	moveSpeed = 0;
	strafeSpeed = 0;
	lookupSpeed=0;

	isMouseTracking = false;

}
//********************************************************************************************************


//** Destructor* *****************************************************************************************
Navigation::~Navigation(){};
//********************************************************************************************************


//********************************************************************************************************
// Deze functie wordt bij elke refresh van het screen aangeroepen
// Hij doet standaard een rotatie, verplaatsing en strafe naargelang de speeds die zijn ingesteld
//********************************************************************************************************
void Navigation::Update(void)
{
	Rotate(rotateSpeed);
	Move_Up();
	Strafe();
	lookUpDown();
}
//********************************************************************************************************


//********************************************************************************************************
// Draait die viewer over een hoek rond zijn as
//********************************************************************************************************
void Navigation::Rotate(int angle)
{
	viewer->viewAngle = viewer->viewAngle + angle; // draaien rond zijn as is gewoon de kijkhoek veranderen
	// kijkhoek van de viewer tussen de 0° en 360° houden
	if (viewer->viewAngle >= 360.0){
		viewer->viewAngle = viewer->viewAngle - 360.0;
	}
	if (viewer->viewAngle < 0.0){
		viewer->viewAngle = 360.0 + viewer->viewAngle;
	}
}
//********************************************************************************************************


//******************************************************************************************************
// naar boven en beneden kijken
//******************************************************************************************************
void Navigation::lookUpDown(){
	viewer->looking+=lookupSpeed;
}

//********************************************************************************************************
// Verplaatst de viewer over een afstand (movespeed) in zijn kijkrichting
// Hier moet ook een boundary-systeem instaan
//********************************************************************************************************
void Navigation::Move_Up(){
		int xBack = viewer->x;	// back-up positie om later deze terug te kunnen zetten als men probeert... 
		int yBack = viewer->y;	// door een muur te lopen
		viewer->x += cos(viewer->viewAngle*PI / 180.0)*moveSpeed; // formule uit driehoeksmeetkunde
		viewer->y -= sin(viewer->viewAngle*PI / 180.0)*moveSpeed;
		int oldX = floor(xBack / 64.0);
		int oldY = floor(yBack / 64.0);

		int newX = (viewer->x+ cos(viewer->viewAngle*PI / 180.0)*moveSpeed*2/3)/64;
		int newY = (viewer->y - sin(viewer->viewAngle*PI / 180.0)*moveSpeed*2/3)/64;
		bool x=true;
		// berekenen of de nieuwe positie niet in een muur ligt, anders terugzetten naar backup
		//probeer de nieuwe positie
		if ((*world)[newX][newY].wall || !(*world)[newX][newY].walktrough){
			//probeer te herstellen langs x of y
			if ((*world)[newX][oldY].wall ||!(*world)[newX][oldY].walktrough  ){
				viewer->x = xBack;x=false;
			}
			if ((*world)[oldX][newY].wall  || !(*world)[oldX][newY].walktrough){
				viewer->y = yBack;x=false;
			}
			//indien dat niet gelukt is, zet de positie volledig terug
			if(x){
			viewer->y = yBack;
			viewer->x = xBack;
			}
		}
	
}
//********************************************************************************************************


//********************************************************************************************************
// Deze kan events ontvangen van andere klasses
// Bij het indrukken van een key gaat de speed van een beweging positief of negatief worden
//********************************************************************************************************
void Navigation::keyPressEvent ( QKeyEvent * e )
{

	if ((e->key() == Qt::Key_Left) || (e->key() == Qt::Key_Q)){		// turn to the left
		if (isMouseTracking)
			strafeSpeed = 10;
		else
			rotateSpeed = 10;
	}
	else if ((e->key() == Qt::Key_Right) || (e->key() == Qt::Key_D)){		//turn to the right
		if (isMouseTracking)
			strafeSpeed = -10;
		else
			rotateSpeed = -10;
	}
	else if ((e->key() == Qt::Key_Up) || (e->key() == Qt::Key_Z)){	// forwards
		moveSpeed = 20;
	}
	else if ((e->key() == Qt::Key_Down) || (e->key() == Qt::Key_S)){	// backwards
		moveSpeed = -20;
	}
	else if (e->key() == Qt::Key_R){	// looking up
		lookupSpeed=5;	// projection plane verhogen
	}
	else if (e->key() == Qt::Key_F){	// looking down
		lookupSpeed=-5;	// projection plane verlagen
	}
	else if (e->key() == Qt::Key_E){	// running
		moveSpeed = 40;
	}
	else if (e->key() == Qt::Key_Space){ // mousetracking
		isMouseTracking = !isMouseTracking;
		//scr->setMouseTracking(isMouseTracking);


	}		
}
//********************************************************************************************************


//********************************************************************************************************
// Deze kan events ontvangen van andere klasses
// Bij het loslaten van een key gaat de speed van een beweging 0 worden, dus de beweging wordt gestopt
//********************************************************************************************************
void Navigation::keyReleaseEvent(QKeyEvent *e)
{
	if ((e->key() == Qt::Key_Left) || (e->key() == Qt::Key_Q)){		// turn to the left
		strafeSpeed = 0;
		rotateSpeed = 0;
	}
	else if ((e->key() == Qt::Key_Right) || (e->key() == Qt::Key_D)){	// turn to the left
		strafeSpeed = 0;
		rotateSpeed = 0;
	}
	else if ((e->key() == Qt::Key_Up) || (e->key() == Qt::Key_Z)){		// forwards
		moveSpeed = 0;
	}
	else if ((e->key() == Qt::Key_Down) || (e->key() == Qt::Key_S)){		// backwards
		moveSpeed = 0;
	}
	else if (e->key() == Qt::Key_E){		// running
		moveSpeed = 0;
	}
	else if (e->key() == Qt::Key_R){	// looking up
		lookupSpeed=0;	// projection plane verhogen
	}
	else if (e->key() == Qt::Key_F){	// looking down
		lookupSpeed=0;	// projection plane verlagen
	}
}
//********************************************************************************************************


//********************************************************************************************************
// Deze wordt gebruikt voor mousetracking waardoor je ook kan bewegen naar links en rechts
//********************************************************************************************************
void Navigation::mouseMoveEvent(QMouseEvent *e)
{
	if (isMouseTracking){

		int newX = e->pos().x();
		int newY = e->pos().y();

		if( newX < 320 )
		{
			// test is groter navenant de mouse verder van het midden gaat
			double test = (320.0 - newX) / 100.0;
			Rotate(0.5+test);
		}
		else if( newX >= 320 )
		{
			double test = ((double)newX - 320.0) / 100.0;
			Rotate(-0.5-test);
		}
	}
}
//********************************************************************************************************


//********************************************************************************************************
// Functie voor de viewer te bewegen over de as, loodrecht op de viewhoek met een bepaalde snelheid
//********************************************************************************************************
void Navigation::Strafe()
{
		int xBack = viewer->x; // coordinaten back-uppen om ze later terug te kunnen zetten
		int yBack = viewer->y;
		viewer->x += cos((viewer->viewAngle+90)*PI / 180.0)*strafeSpeed; // formule uit driehoeksmeetkunde
		viewer->y -= sin((viewer->viewAngle+90)*PI / 180.0)*strafeSpeed;
		int oldX = floor(xBack / 64.0);
		int oldY = floor(yBack / 64.0);
		int newX = (viewer->x+ cos((viewer->viewAngle+90)*PI / 180.0)*strafeSpeed*2/3)/64;
		int newY = (viewer->y - sin((viewer->viewAngle+90)*PI / 180.0)*strafeSpeed*2/3)/64;
		bool x=true;
		// berekenen of de nieuwe positie niet in een muur ligt, anders terugzetten naar backup
		//probeer de positie
		if ((*world)[newX][newY].wall || !(*world)[newX][newY].walktrough){
			//herstel langs x of y
			if ((*world)[newX][oldY].wall ||!(*world)[newX][oldY].walktrough  ){
				viewer->x = xBack;x=false;
			}
			if ((*world)[oldX][newY].wall  || !(*world)[oldX][newY].walktrough){
				viewer->y = yBack;x=false;
			}
			//indien herstel niet gelukt is, zet volledig terug
			if(x){
				viewer->y = yBack;
				viewer->x = xBack;
			}
		}
}
//********************************************************************************************************
