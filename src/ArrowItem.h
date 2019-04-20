//***************************************************************************************************
// ARROWITEM.H
// Item die de viewer in de editor voorstelt
//***************************************************************************************************
// AUTEUR: NICK
//***************************************************************************************************

#ifndef ARROW_ITEM_H
#define ARROW_ITEM_H 1

#include <QtGui/QMainWindow>
#include <QGraphicsView>
#include <QPixmap>
#include <QGraphicsPixmapItem>
#include <QMouseEvent>
#include <QDrag>


class ArrowItem : public QGraphicsPixmapItem, public QWidget

{
	public:	
		ArrowItem(QGraphicsItem * parent = 0, QGraphicsScene * scene = 0);
		virtual ~ArrowItem(){};

	protected:
		void mousePressEvent(QGraphicsSceneMouseEvent *event);
		void dragEnterEvent(QDragEnterEvent *event);
		void dragLeaveEvent  ( QDropEvent *event );
		void dropEvent(QDropEvent *event);
};

#endif