//***************************************************************************************************
// QEDITTEXTURE.H
// De items die de huidige textures van een geselecteerd object weergeven
//***************************************************************************************************
// AUTEUR: NICK
//***************************************************************************************************

#ifndef QEDITTEXTURE_H
#define QEDITTEXTURE_H 1

#include <QtGui/QMainWindow>
#include <QGraphicsView>
#include <QPixmap>
#include <QGraphicsPixmapItem>
#include <QGraphicsSceneMouseEvent>

enum wichEdit{ EDIT_FLOOR, EDIT_CEILING, EDIT_SPRITE, EDIT_WALL_NORTH, EDIT_WALL_EAST, EDIT_WALL_SOUTH, EDIT_WALL_WEST };

class QEditTexture : public QObject, public QGraphicsPixmapItem

{
	Q_OBJECT
	public:	
		QEditTexture(wichEdit sort, QGraphicsItem * parent = 0, QGraphicsScene * scene = 0);
		virtual ~QEditTexture(){};
		wichEdit sort;

		void mousePressEvent ( QGraphicsSceneMouseEvent * event );

	signals:
			void TextureClick(QEditTexture* item);
		
};

#endif