//***************************************************************************************************
// WORLD.CPP
// Houdt alle data voor de wereld bij
//***************************************************************************************************
// AUTEUR: KENNETH
//***************************************************************************************************

#include <stdlib.h>
#include <qrgb.h>
#include <fstream>
#include <sstream> 
using namespace std; 
#include <iostream>
#include "World.h"

//** Constructor ***********************************************************************************
World::World(QRgb value, int worldHeight ,int worldWidth): m_WorldWidth(worldWidth), m_WorldHeight(worldHeight)
{
	r=qRed(value);
	g=qGreen(value);
	b=qBlue(value);
	trigger=false;
	this->fileName="standard";
	//map-tabel initialiseren
	Fill();
}
//**************************************************************************************************


//** Destructor ************************************************************************************
World::~World(){
	for(int i=0; i< this->m_WorldHeight; i++)
		delete [] map[i];
	delete [] map;
	textures.Clear();
}
//**************************************************************************************************


//**************************************************************************************************
// Wereld opvullen met standaardwaardes
//**************************************************************************************************
void World::Fill()
{
	map=new cel*[m_WorldHeight];
	for(int i=0; i< m_WorldHeight; i++)
		map[i]=new cel[m_WorldWidth];
	FillMap(this->map, this->m_WorldHeight, this->m_WorldWidth);
}
//**************************************************************************************************


//**************************************************************************************************
// Een standaardmap inladen
//**************************************************************************************************
void World::FillMap(Cellen map, int height, int width)
{
	for (int i=1; i<height-1; i++){
		for (int j=1; j<width-1; j++){
			map[i][j].wall=false;
			map[i][j].north = 0;
			map[i][j].south = 0;
			map[i][j].east = 0;
			map[i][j].west = 0;
			map[i][j].trigger=0;
			map[i][j].sprite=0;
			map[i][j].floor = 2;
			map[i][j].ceil = 3;
			map[i][j].isSprite=false;
			map[i][j].walktrough=true;
		}
	}

	for (int i=0; i<height; i++){
		map[i][0].wall=true;
		map[i][0].north = 1;
		map[i][0].south = 1;
		map[i][0].east = 1;
		map[i][0].west = 1;
		map[i][0].trigger=0;
		map[i][0].sprite=0;
		map[i][0].floor = 2;
		map[i][0].ceil = 3;
		map[i][0].walktrough=true;
		map[i][0].isSprite=false;
		map[i][width-1].wall=true;
		map[i][width-1].north = 1;
		map[i][width-1].south = 1;
		map[i][width-1].east = 1;
		map[i][width-1].west = 1;
		map[i][width-1].trigger=0;
		map[i][width-1].sprite=0;
		map[i][width-1].floor = 2;
		map[i][width-1].ceil = 3;
		map[i][width-1].isSprite=false;
		map[i][width-1].walktrough=true;
	}
	for (int i=0; i<width; i++){
		map[0][i].wall=true;
		map[0][i].north = 1;
		map[0][i].south = 1;
		map[0][i].east = 1;
		map[0][i].west = 1;
		map[0][i].trigger=0;
		map[0][i].sprite=0;
		map[0][i].floor = 2;
		map[0][i].ceil = 3;
		map[0][i].isSprite=false;
		map[0][i].walktrough=true;
		map[height-1][i].wall=true;
		map[height-1][i].north = 1;
		map[height-1][i].south = 1;
		map[height-1][i].east = 1;
		map[height-1][i].west = 1;
		map[height-1][i].trigger=0;
		map[height-1][i].sprite=0;
		map[height-1][i].floor = 2;
		map[height-1][i].ceil = 3;
		map[height-1][i].isSprite=false;
		map[height-1][i].walktrough=true;
	}
}
//**************************************************************************************************


//**************************************************************************************************
// De grootte van de map veranderen
//**************************************************************************************************
void World::resizeMap(int worldWidth, int worldHeight){
	resize(worldHeight, worldWidth);
	this->m_WorldHeight=worldHeight;
	this->m_WorldWidth=worldWidth;
}
//**************************************************************************************************


//**************************************************************************************************
// De grootte van de map veranderen
//**************************************************************************************************
void World::resize(int worldHeight, int worldWidth){
	//tijdelijke hulpcellen maken
	Cellen temp;
	temp=new cel*[worldHeight];
	for(int i=0; i< worldHeight; i++)
		temp[i]=new cel[worldWidth];
	//de map vullen
	FillMap(temp, worldHeight, worldWidth); // standaard opvulling geven
	//de vorige opvulling erbij laden
	for(int i=0; i<worldHeight && i < this->m_WorldHeight;i++)
		for(int j=0; j<worldWidth && j< this->m_WorldWidth;j++)
			temp[i][j]=map[i][j];
	//de oude map deleten en de nieuwe toekennen
	for(int i=0; i< this->m_WorldHeight; i++)
		delete [] map[i];
	delete [] map;
	map=temp;
	m_WorldHeight=worldHeight;
	m_WorldWidth=worldWidth;
	//buitenmuren aan zetten
	for(int i=0;  i < this->m_WorldHeight;i++){
		map[i][0].wall=true;
		map[i][m_WorldWidth-1].wall=true;
		}
		for(int i=0;  i < this->m_WorldWidth;i++){
			map[0][i].wall=true;
			map[m_WorldHeight-1][i].wall=true;
		}
}
//**************************************************************************************************


//**************************************************************************************************
// Zet de World gegevens om naar een string
//**************************************************************************************************
string World::toString(){
	stringstream s;
	//de kleurwaarde
	s<<"red: "<<r<<" green: "<<g<<" blue: "<<b;
	//triggeroptie voor de wereld
	s<<" trigger_everytime_on_square? "<<trigger<<"\n";
	//afmetingen
	s<< " Height: "<<m_WorldHeight<<" width: "<< m_WorldWidth <<"\n";
	//alle cellen
	for(int i=0;i<m_WorldHeight;i++)
	{
		s<<i;
		for(int j=0; j< m_WorldWidth;j++){
			s<<"{ "<<map[i][j].wall<<" "<<map[i][j].north<<" "<<map[i][j].east<<" "<<map[i][j].south<<" "<<map[i][j].west<<" "<<map[i][j].floor<<" "<<map[i][j].ceil<<" "<<map[i][j].trigger<<" "<<map[i][j].sprite<<" "<<map[i][j].isSprite<<" "<<map[i][j].walktrough<<" }";
		}
		
		s<<"\n";		
	}
	//textures wegschrijven
	s<<textures.toString();
	//als string teruggeven
	return s.str();
}
//**************************************************************************************************


//**************************************************************************************************
// Zet een string om naar World gegevens
//**************************************************************************************************
void World::fromString(	fstream *s){
	//de oude map legen
	if(map!=0)
	{	for(int i=0; i< this->m_WorldHeight; i++)
			delete [] map[i];
		delete [] map;
	}
	//een dumpwaarde
	char* d=new char[222];
	//data inlezen
	(*s)>>d>>r>>d>>g>>d>>b;
	(*s)>>d>>trigger;
	(*s)>>d>>m_WorldHeight>>d>>m_WorldWidth;
	//de nieuwe map inlezen
	map=new cel*[m_WorldHeight];
	for(int i=0; i< m_WorldHeight; i++)
		map[i]=new cel[m_WorldWidth];
	for(int i=0; i< m_WorldHeight; i++)
	{
		for(int j=0; j< m_WorldWidth; j++)
			(*s)>>d>>map[i][j].wall>>map[i][j].north>>map[i][j].east>>map[i][j].south>>map[i][j].west>>map[i][j].floor>>map[i][j].ceil>>map[i][j].trigger>>map[i][j].sprite>>map[i][j].isSprite>>map[i][j].walktrough;
	(*s)>>d;
	}
	cout << "World: Begin clear textures" << endl;
	//textures legen
	textures.Clear();
	cout << "World: Einde clear textures" << endl;
	cout << "World: Begin fromstring textures" << endl;
	//textures inlezen
	textures.fromString(s);
	cout << "World: Einde fromstring textures" << endl;
}
//**************************************************************************************************


//**************************************************************************************************
// Kijkt of een coordinaat (x, y) buiten de world ligt of niet en geeft een bool terug
//**************************************************************************************************
bool World::isOutOfBound( int celX, int celY ){
	if (celX >= WorldHeight() ||  celX < 0){
		return true;
	}
	else if (celY >= WorldWidth() || celY < 0){
		return true;
	}
	else {
		return false;
	}
}
//**************************************************************************************************


//**************************************************************************************************
// Kijkt of een coordinaat (x, y) op de rand ligt van world en geeft een bool terug
//**************************************************************************************************
bool World::isBoundary ( int celX, int celY ){
	if (celY >= WorldWidth()-1 ||  celY <= 0){
		return true;
	}
	else if (celX >= WorldHeight()-1 || celX <= 0){
		return true;
	}
	else {
		return false;
	}
}
//**************************************************************************************************

//**************************************************************************************************
// Kijkt of een coordinaat (x, y) een sprite is of niet en geeft een bool terug
//**************************************************************************************************
bool World::isSprite ( int celX, int celY )
{
	return (map[celX][celY].isSprite);
}

//**************************************************************************************************
// Kijkt of een coordinaat (x, y) een muur is of niet en geeft een bool terug
//**************************************************************************************************
bool World::isWall ( int celX, int celY )
{
	return map[celX][celY].wall;

}
//**************************************************************************************************