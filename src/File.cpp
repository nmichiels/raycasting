//***************************************************************************************************
// FILE.CPP
// Opslaan van data in files en deze terug inladen
//***************************************************************************************************
// AUTEUR: KENNETH
// De to-string en from-string functies in elke klasse horen bij deze file-klasse en zijn ook
// geschreven door Kenneth
//***************************************************************************************************

#include "File.h"
#include <stdlib.h>

//***************************************************************************************************************
// Zorgt dat alle fromstrings worden aangeroepen en schrijft deze weg in een file
//***************************************************************************************************************
bool File::save(char* name,World *world, Viewer *viewer, Triggers *triggers)
{
	fstream filestr;
	filestr.open (name, std::fstream::in | std::fstream::out | std::fstream::trunc);
	if (!filestr)
		return false;
	filestr << viewer->toString();
	filestr << world->toString();
	filestr.close();
	char *b = new char[strlen(name)+5];
	sprintf(b,"%s.tri",name);
	filestr.open(b,std::fstream::in | std::fstream::out | std::fstream::trunc);
	if (!filestr)
		return false;
	filestr << triggers->toString();
	filestr.close();
	return true;
}
//***************************************************************************************************************


//***************************************************************************************************************
// Opslaan van de triggers
//***************************************************************************************************************
bool File::trigSave(char* name, Triggers *triggers)
{
	fstream filestr;
	filestr.open(name,std::fstream::in | std::fstream::out | std::fstream::trunc);
	if (!filestr){
		return false;
	}
	filestr << triggers->toString();
	filestr.close();
	return true;
}
//***************************************************************************************************************


//***************************************************************************************************************
// Inladen van de triggers
//***************************************************************************************************************
bool File::trigLoad(char* name, Triggers *triggers)
{
	fstream filestr;
	filestr.open(name,std::fstream::in | std::fstream::out);
	if (!filestr)
		return false;
	triggers->fromString(&filestr);
	filestr.close();
	return true;
}
//***************************************************************************************************************


//***************************************************************************************************************
// haalt alle gegevens uit de file en doet een fromstring op de overeenkomstige data
//***************************************************************************************************************
bool File::load(char* name,World *world, Viewer *viewer, Triggers *triggers)
{
	fstream filestr;
	filestr.open (name, std::fstream::in | std::fstream::out );
	if (!filestr)
		return false;
	viewer->fromString(&filestr);
	world->fromString(&filestr);
	filestr.close();
	char *b = new char[strlen(name)+5];
	sprintf(b,"%s.tri",name);
	filestr.open(b,std::fstream::in | std::fstream::out);
	if (!filestr)
		return false;
	triggers->fromString(&filestr);
	filestr.close();
	return true;
}
//***************************************************************************************************************


//***************************************************************************************************************
// Kan een file laten opslaan met gegeven inhoud
//***************************************************************************************************************
bool File::saveSomething(char* name, char* content)
{
	fstream filestr;
	filestr.open(name,std::fstream::in | std::fstream::out | std::fstream::trunc);
	if (!filestr){
		return false;
	}
	filestr << content;
	filestr.close();
	return true;
}
//***************************************************************************************************************


//***************************************************************************************************************
// Geeft de inhoud van een file terug
//***************************************************************************************************************
bool File::loadSomething(char* name, QString* content)
{
	fstream filestr;
	filestr.open(name,std::fstream::in);
	if (!filestr){
		return false;
	}
	QString test;
	char a;
	content->clear();
	while (!filestr.eof()){
		a = filestr.get();
		(*content)+=a;
	}
	filestr.close();
	return true;
}
//***************************************************************************************************************
