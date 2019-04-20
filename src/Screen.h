//***************************************************************************************************
// SCREEN.H
// De klasse houdt de projection plane bij en heeft functies gedeclareerd om punten op het scherm
// uit te tekenen
//***************************************************************************************************
// AUTEUR: NICK
//***************************************************************************************************

#ifndef SCREEN_H
#define SCREEN_H 1

#include <QtGui/QMainWindow>
#include <qwidget.h>
#include <qimage.h>
#include <qpen.h>
#include <qpoint.h>
#include <qrgb.h>
#include <qpainter.h>
#include <QKeyEvent>

static QImage *m_Canvas; // globaal omdat we deze dan static kunnen gebruiken

class Screen : public QWidget
{
	Q_OBJECT
	public:	
		Screen(QWidget *parent=0, Qt::WFlags flags=0);
		virtual ~Screen();
		static void Clear(void); // Wis het scherm
		static bool VerticalLine(int x, int y1, int y2, const QRgb& color);
		static void Point(int x, int y, const QRgb& color);


		void paintEvent( QPaintEvent* e );
		void resizeEvent( QResizeEvent* e );
		void mouseMoveEvent ( QMouseEvent * event );

	private:
		static const int scrWidth = 640; //schermresolutie
		static const int scrHeight = 480; //schermresolutie

	signals:
		void move(QMouseEvent * event);
};
		


#endif