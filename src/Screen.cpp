//***************************************************************************************************
// SCREEN.H
// De klasse houdt de projection plane bij en heeft functies gedeclareerd om punten op het scherm
// uit te tekenen
//***************************************************************************************************
// AUTEUR: NICK
//***************************************************************************************************

#include "Screen.h"

//** Constructor ***********************************************************************************
Screen::Screen(QWidget *parent, Qt::WFlags flags) : QWidget(parent, flags){	
	m_Canvas = new QImage(640, 480, QImage::Format_RGB32);
	m_Canvas->fill( qRgb(0, 0, 0) );
}
//**************************************************************************************************


//** Destructor ************************************************************************************
Screen::~Screen()
{
	delete(m_Canvas);
}
//**************************************************************************************************


//**************************************************************************************************
// Vult het canvas met zwart
//**************************************************************************************************
void Screen::Clear(void)
{
  m_Canvas->fill( qRgb(0, 0, 0) );
}

//**************************************************************************************************
//Snelle verticale lijn van (x,y1) tot (x,y2), met rgb color
//**************************************************************************************************
bool Screen::VerticalLine(int x, int y1, int y2, const QRgb& color)
{
	int hulp;
   if(y2 < y1) {hulp=y2; y2=y1, y1=hulp;}	//	verwissel y1 en y2
  if(x < 0 || x >= scrWidth) return 0;		//	punt ligt niet op het scherm
  if(y1 < 0) y1 = 0;						// afknippen
  if(y2 >= scrHeight) y2 = scrHeight - 1;	// afknippen
   
  for(int y = y1; y <= y2; y++)
  {
     m_Canvas->setPixel(x, y, color);
  }
  return 1;
}
//**************************************************************************************************


//**************************************************************************************************
// Tekent een punt op plaats (x, y) met qrgb color
//**************************************************************************************************
void Screen::Point(int x, int y, const QRgb& color)
{
	if (x < scrWidth && x >= 0 && y >= 0 && y < scrHeight) // enkel tekenen als het binnen het canvas ligt
		m_Canvas->setPixel(x, y, color);
}
//**************************************************************************************************


//**************************************************************************************************
// Screen::paintEvent( QPaintEvent* e )
//**************************************************************************************************
void Screen::paintEvent( QPaintEvent* e ) {
	QPainter painter( this );
	painter.drawImage( QPoint(0, 0), *m_Canvas );
}
//**************************************************************************************************


//**************************************************************************************************
// Bij het resizen moet het canvas terug opnieuw worden opgevuld
//**************************************************************************************************
void Screen::resizeEvent( QResizeEvent* e ) { 
	QWidget::resizeEvent( e );
	QImage newImage( scrWidth, scrHeight, QImage::Format_RGB32 );
	newImage.fill( qRgb(0, 0, 0) );
	QPainter painter( &newImage );
	painter.drawImage( QPoint(0, 0), *m_Canvas );
	*m_Canvas = newImage;
}
//**************************************************************************************************


//**************************************************************************************************
// Een muisbeweging op het scherm wordt doorgestuurd naar hoofdklasse
// Deze is nodig voor mousetracking mogelijk te maken
//**************************************************************************************************
void Screen::mouseMoveEvent ( QMouseEvent * event )
{
	emit move(event);
}
//**************************************************************************************************