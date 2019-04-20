//***************************************************************************************************
// MINIMAP.CPP
// Beheert het berekenen en uittekenen van de minimap
//***************************************************************************************************
// AUTEUR: NICK
//***************************************************************************************************

#include "MiniMap.h"


//** Constructor ***********************************************************************************
MiniMap::MiniMap(QWidget *parent, Qt::WFlags flags) : QWidget(parent, flags)
{
	m_Map = new QImage(120, 120, QImage::Format_RGB32);
	m_Map->load("images/minimap.png"); // transparante image inladen anders geeft hij groen
	m_Scale = 10;

	colorNoWall = qRgb(0, 0, 0);	// black
	colorOOB = qRgb(234, 7, 7);		// red
	colorWall = qRgb(44, 90, 0);	// green
	colorWalktrough =qRgb(252, 92, 39);   //oranje
	colorSprite =qRgb(0, 84, 168);   //lichtblauw
}
//**************************************************************************************************


//** Destructor ************************************************************************************
MiniMap::~MiniMap()
{
	delete(m_Map);
}
//**************************************************************************************************


//**************************************************************************************************
// Vergroot de schaal van de minimap
//**************************************************************************************************
void MiniMap::ScaleAdd(void){
	if(m_Scale <=30)
		m_Scale += 5;
}
//**************************************************************************************************


//**************************************************************************************************
// Verkleint de schaal van de minimap
//**************************************************************************************************
void MiniMap::ScaleSub(void){
	if(m_Scale > 5)
		m_Scale -= 5;
}

//**************************************************************************************************


//**************************************************************************************************
// Tekent de minimap uit aan de hand van positie van de viewer en de wereld waar hij in loopt
//**************************************************************************************************
void MiniMap::DrawMiniMap(World *world, Viewer *viewer)
{
	int miniMapx, miniMapy, mapx, mapy;
	miniMapx = 0;						// coordinaat op minimap zelf
	miniMapy = 0;						// coordinaat op minimap zelf
	mapx = viewer->x - 60*m_Scale;		// coordinaten in de wereld vanwaar de minimap moet beginnen te tekenen
	mapy = viewer->y - 60*m_Scale;		// (-60 omdat minimap 120 groot is, schaal geeft aan hoe ver je moet gaan kijken)
	int celx, cely;						// de celcoordinaten waar mapx en mapy zich bevind

	for (miniMapy = 0; miniMapy < 120; miniMapy++){
		for (miniMapx = 0; miniMapx < 120; miniMapx++){
			celx = floor(double(mapx) / 64.0);
			cely = floor(double(mapy) / 64.0);
			if (celx < 0 || celx >= world->WorldHeight() || cely < 0 || cely >= world->WorldWidth()){
				Point(miniMapx, miniMapy, colorOOB);	// Punt bevindt zich buiten de wereldmap
			}
			else if ((*world)[celx][cely].wall){
				Point(miniMapx, miniMapy, colorWall);	// Het punt is een muur
			}else if (!(*world)[celx][cely].walktrough){
				Point(miniMapx, miniMapy, colorWalktrough);	// Op dit punt kan je (nog) niet wandelen
			}
			else { 
				Point(miniMapx, miniMapy, colorNoWall);	// Het punt is een plaats waar je kan lopen
			}
			if(!(celx < 0 || celx >= world->WorldHeight() || cely < 0 || cely >= world->WorldWidth()) && (*world)[celx][cely].isSprite)
			{
				if(sqrt(double((mapx-64*celx-31)*(mapx-64*celx-31)+(mapy-64*cely-31)*(mapy-64*cely-31)))<20)
					Point(miniMapx, miniMapy, colorSprite);	
			}
			 mapx += m_Scale;
		}
		mapx = viewer->x - 60*m_Scale;
		mapy += m_Scale;
	}

	Rotate(viewer->viewAngle);

	// Punt tekenen om aan te geven waar je zelf zit (altijd in het midden)
	Point(60, 60, colorOOB);
	Point(60, 61, colorOOB);
	Point(60, 59, colorOOB);
	Point(61, 60, colorOOB);
	Point(59, 60, colorOOB);

	// twee lijnen tekenen die gezichtsveld aangeven
	int linksx = cos(57 * PI / 180.0) * 60;
	int linksy = sin(57 * PI / 180.0) * 60;
	Line(59, 59, 60 - linksx, 60 - linksy, colorOOB);
	Line(61, 59, 60 + linksx, 60 - linksy, colorOOB);
	update();
}

//**************************************************************************************************


//**************************************************************************************************
// Draait de minimap rond een hoek
//**************************************************************************************************
void MiniMap::Rotate(double angle){
	QImage newImage( 120, 120, QImage::Format_RGB32 );
	newImage.load("images/minimap.png"); // transparante image inladen om groen tegen te gaan
	QPainter painter( &newImage );
	painter.setTransform(QTransform().translate(60, 60).rotate(angle-90).translate(-60, -60));
	
	painter.drawImage( QPoint(0, 0), *m_Map );
	*m_Map = newImage; // minimap gelijk stellen aan de geroteerde image
	update();	
}
//**************************************************************************************************


//**************************************************************************************************
// Vult de minimap met zwart
//**************************************************************************************************
void MiniMap::Clear(void)
{
  m_Map->fill( qRgb(255, 255, 255) );
}
//**************************************************************************************************

//**************************************************************************************************
// Snelle verticale lijn van (x,y1) tot (x,y2), met rgb kleur
//**************************************************************************************************
bool MiniMap::VerticalLine(int x, int y1, int y2, const QRgb& color)
{
	int hulp;
  if(y2 < y1) {hulp=y2; y2=y1, y1=hulp;}	//	verwissel y1 en y2
  if(x < 0 || x >= scrWidth) return 0;		//	punt ligt niet op het scherm
  if(y1 < 0) y1 = 0;						// afknippen
  if(y2 >= scrHeight) y2 = scrHeight - 1;	// afknippen
   
  for(int y = y1; y <= y2; y++)
  {
     m_Map->setPixel(x, y, color);
  }
  return 1;
}
//**************************************************************************************************


//**************************************************************************************************
// Tekent een punt op (x, y) met een rgb kleur
//**************************************************************************************************
void MiniMap::Point(int x, int y, const QRgb& color)
{
	if(y < scrHeight && x < scrWidth && x>=0 && y>=0){
		int afstand = DistanceToMiddle(x, y);
		if (afstand <= 64){
			// procentueel aangeven hoe ver we verwijderd zijn van het midden
			double procent = 1.0 - (double) afstand / 60.0 + 0.2; // +0.2 om het iets mooier te maken
			if (procent > 1.0)
				procent = 1.0;
			QRgb col;
			// des te verder van het midden des te donkerder het wordt uitgetekend
			col = qRgb(qRed(color) * procent, qGreen(color) * procent, qBlue(color) * procent);
			m_Map->setPixel(x, y, col);
		}
	}
}
//**************************************************************************************************


//**************************************************************************************************
// Geeft de afstand van een punt (x, y) tot het midden van de minimap terug
//**************************************************************************************************
int MiniMap::DistanceToMiddle(int x, int y){
	int xOffset = abs(x - 60);
	int yOffset = abs(y - 60);
	return (int)(sqrt(pow(xOffset, 2.0) + pow(yOffset, 2.0))); // pythagoras
}
//**************************************************************************************************


//**************************************************************************************************
// MidpointLine algoritme of Bresenham algoritme om snel een lijn te tekenen
//**************************************************************************************************
bool MiniMap::Line(int x0, int y0, int x1, int y1, const QRgb& color)
{
	if (x0 == x1){
		return false;
	}
	if (x1 < x0){
		int hulp;
		hulp = x1;
		x1 = x0;
		x0 = hulp;
		hulp = y1;
		y1 = y0;
		y0 = hulp;
	}

	// y = mx+b met m = (y1-y0)/(x1-x0)
	double m = double(y1-y0) / double(x1-x0);
	double b = y0 - m*x0;
	int y, x;
	for (x=x0; x<=x1; x++){
		y = m*x + b;
		Point(x, y, color);
	}
	return true;
}
//**************************************************************************************************


//**************************************************************************************************
// PaintEvent
//**************************************************************************************************
void MiniMap::paintEvent( QPaintEvent* e ) {
	QPainter painter( this );
	painter.drawImage( QPoint(0, 0), *m_Map );
}
//**************************************************************************************************


//**************************************************************************************************
// resizeEvent: gaat opnieuw de minimap inladen
//**************************************************************************************************
void MiniMap::resizeEvent( QResizeEvent* e ) { 
	QWidget::resizeEvent( e );
	QImage newImage( 120, 120, QImage::Format_RGB32 );
	newImage.load("minimap.png");
	QPainter painter( &newImage );
	painter.drawImage( QPoint(0, 0), *m_Map );
	*m_Map = newImage;
}
//**************************************************************************************************

