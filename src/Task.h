//***************************************************************************************************
// TASK.H
// Task : algemene klasse waar alle tasks van zijn afgeleid
//***************************************************************************************************
// AUTEUR: KENNETH
//***************************************************************************************************

#ifndef Task_H
#define Task_H 1
#include "World.h"
#include <string>
#include "Viewer.h"
#include "ui_raycastingqt.h"
/*De basis voor elke task die kan uitgevoerd worden in de wereld.
Algemeen staan x, y, lup, ldown,xleft en xright voor coordinaten.
De meeste tasks zijn overbodig maar zijn er om gemakkelijker te werken met de tasks.
Algemeen gebeuren er altijd controles om errors te vermijden.

Change: Pas een onderdeel van een vakje aan.
Set: Zet een bool op true of false.
Flip: Indien bool aan-> uit, indien uit-> aan.

De general trigger dient voor de grotere aanpassingen .
*/
//de data die elke task bij houdt
struct TriggerData{
	World *world;
	Viewer *viewer;
	Ui::RaycastingQTClass *ui;
};

class Task
{
public:
	//elke task moet iets kunnen uitvoeren
	virtual void execute(struct TriggerData a)=0;
	//elke task moet kunnen worden opgeslaan
	virtual string toString()=0;
};

#endif

