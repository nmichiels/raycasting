//***************************************************************************************************
// WORLD.H
// Houdt alle data voor de wereld bij
//***************************************************************************************************
// AUTEUR: KENNETH
//***************************************************************************************************

#ifndef WORLD_H
#define WORLD_H 1
#include <qrgb.h>
#include <stdlib.h>
#include <qstring>
 
#include <sstream> 
using namespace std; 
#include <iostream>
#include "Textures.h"


//de informatie per vakje in de wereld
struct cel{
	bool wall;
	int north;
	int east;
	int south;
	int west;
	
	int trigger;
	//sprites
	bool isSprite;
	int sprite;
	bool walktrough;

	int floor;
	int ceil;
	
};

typedef struct cel** Cellen;

class World
{
	public:	
		QString fileName;

		World(QRgb value, int worldHeight = 32,int worldWidth = 32);
		
		~World();
		//resize (door de editor)
		void resizeMap(int worldWidth, int worldHeight);
		//controles binnen de map
		bool isOutOfBound( int celX, int celY );
		bool isBoundary ( int celX, int celY );
		bool isWall ( int celX, int celY );
		bool isSprite ( int celX, int celY );
		//oproepen waardes
		cel* operator[](int row){return map[row];}
		int WorldWidth(){return m_WorldWidth;}
		int WorldHeight()
		{
			return m_WorldHeight;
		}

		//editen waardes
		void editWorldHeight(int worldHeight){resizeMap(this->m_WorldWidth, worldHeight);}
		void editWorldWidth(int worldWidth){resizeMap(worldWidth, this->m_WorldHeight);}
		void Fill();
		//de texturen voor deze wereld
		Textures textures;

		//om op te slaan en in te laden
		string toString();
		void fromString(fstream *s); 
		//kleurwaardes voor de wereld(bvb voor mist)
		int r,g,b;
		//bepaald of een trigger 1 of meerdere keren mag afgaan bij het op een vakje staan
		bool trigger;
	private:
		//de cellen die de data over de wereld in detail bevatten
		Cellen map;
		void resize(int worldWidth, int worldHeight);
		//waarde die over de echte kleuren komen (bvb mist)
		int m_WorldWidth;	
		int m_WorldHeight;

		void FillMap(Cellen map, int height, int width);
		
};
#endif