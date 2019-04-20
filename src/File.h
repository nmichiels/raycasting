//***************************************************************************************************
// FILE.H
// Opslaan van data in files en deze terug inladen
//***************************************************************************************************
// AUTEUR: KENNETH
// De to-string en from-string functies in elke klasse horen bij deze file-klasse en zijn ook
// geschreven door Kenneth
//***************************************************************************************************


#ifndef BESTAND_H
#define BESTAND_H 1
#include <sstream> 
using namespace std; 
#include <iostream>
#include <fstream>
#include "World.h"
#include "Viewer.h"
#include "Triggers.h"
#include <QString>

class File{
public:
	File(){}
	//deze functies laden de wereld en triggers uit en in
	static bool save(char* name,World *world, Viewer *viewer, Triggers *triggers);
	static bool load(char* name,World *world, Viewer *viewer, Triggers *triggers);
	//deze functies laden enkel triggers uit en in
	static bool trigSave(char* name, Triggers *triggers);
	static bool trigLoad(char* name, Triggers *triggers);
	static bool saveSomething(char* name, char* content);
	static bool loadSomething(char* name, QString *content);
};


#endif