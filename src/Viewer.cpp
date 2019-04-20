//***************************************************************************************************
// VIEWER.CPP
// Deze klasse stelt een camera voor in de wereld
//***************************************************************************************************
// AUTEUR: NICK
//***************************************************************************************************

#include "Viewer.h"


//**************************************************************************************************
// Viewerinformatie op standaard waardes zetten
//**************************************************************************************************
void Viewer::Reset()
{
	x=96;
	y=96;
	FOV=66;
	viewAngle=315;
	looking=240;
	DPP=985;
}
//**************************************************************************************************