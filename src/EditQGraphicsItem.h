//***************************************************************************************************
// EDITQGRAPHICSITEM.H
// Item die een cel van de wereld voorstelt in de editor
//***************************************************************************************************
// AUTEUR: NICK
//***************************************************************************************************

#ifndef EDIT_QGRAPHICS_ITEM
#define EDIT_QGRAPHICS_ITEM 1

#include <QtGui/QMainWindow>
#include <QGraphicsView>
#include <QPixmap>
#include <QGraphicsPixmapItem>
#include <QGraphicsSceneMouseEvent>
#include <QKeyEvent>

#include <math.h>

static int dragItemGridX;
static int dragItemGridY;

class EditQGraphicsItem : public QWidget, public QGraphicsPixmapItem

{
	Q_OBJECT
	public:	
		EditQGraphicsItem(int x=0, int y=0, QGraphicsItem * parent = 0, QGraphicsScene * scene = 0);
		virtual ~EditQGraphicsItem();
		int gridX;	// Geeft aan met welke cel van world deze item overeen komt
		int gridY;

	signals:
			void ItemClick(EditQGraphicsItem* item);
			void DropArrow(int posx, int posy, int gridX, int gridY);
			void Drop(EditQGraphicsItem *from, EditQGraphicsItem *to);

	public:
		void mousePressEvent ( QGraphicsSceneMouseEvent * event );
		void dropEvent(QGraphicsSceneDragDropEvent *event);
		void dragEnterEvent(QDragEnterEvent *event);
		void dragLeaveEvent  ( QDropEvent *event );
		void SetTip(QString tip);

	
};

static EditQGraphicsItem* dragItem;



#endif