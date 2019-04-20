//***************************************************************************************************
// MINIMAP.H
// Beheert het berekenen en uittekenen van de minimap
//***************************************************************************************************
// AUTEUR: NICK
//***************************************************************************************************

#ifndef MINIMAP_H
#define MINIMAP_H 1

#include <QtGui/QMainWindow>
#include <QImage.h>
#include <qpainter.h>
#include <QKeyEvent>

#include <stdio.h>
#include <math.h>
#include <conio.h>

#include "World.h"
#include "Viewer.h"

#define PI 3.14159265

class MiniMap : public QWidget
{
	Q_OBJECT
	public:	
		MiniMap(QWidget *parent=0, Qt::WFlags flags=0);
		virtual ~MiniMap();

		void DrawMiniMap(World *world, Viewer *viewer);					// minimap uittekenen volgend een wereld en camera
		void Clear(void);												// minimap resetten
		void ScaleAdd(void);											// schaal vergroten
		void ScaleSub(void);											// schaal verkleinen	

	protected:
		void paintEvent( QPaintEvent* e );
		void resizeEvent( QResizeEvent* e );
		
	private:
		static const int scrWidth = 120;								// resolutie minimap
		static const int scrHeight = 120;								// resolutie minimap
		QImage *m_Map;
		int m_Scale;								// schaalgrootte waarop de minimap moet worden getekend

		// kleuren die worden gebruikt
		QRgb colorOOB;								// Kleur die aangeeft dat die positie buiten de map is
		QRgb colorNoWall;							// Kleur die aangeeft dat je er kan rondlopen
		QRgb colorWall;								// Kleur die aangeeft dat er een muur is
		QRgb colorWalktrough;						// Kleur die aangeeft dat er een onzichtbare muur is
		QRgb colorSprite;						// Kleur die aangeeft dat er een sprite is
		

		void Point(int x, int y, const QRgb& color);					// punt in bepaalde kleur tekenen
		bool VerticalLine(int x, int y1, int y2, const QRgb& color);	// verticale lijn tekenen
		void Rotate(double angle);										// draaien van de minimap
		bool Line(int x1, int y1, int x2, int y2, const QRgb& color);	// lijn via Bresenham algoritme
		int DistanceToMiddle(int x, int y);								// geeft afstand van midden tot punt (x, y) terug

		
		
};
		


#endif