//***************************************************************************************************
// ARROWITEM.CPP
// Item die de viewer in de editor voorstelt
//***************************************************************************************************
// AUTEUR: NICK
//***************************************************************************************************

#include "ArrowItem.h"
#include <iostream>
using namespace std;

//** Constructor ********************************************************************************************
ArrowItem::ArrowItem(QGraphicsItem * parent, QGraphicsScene * scene) 
	: QGraphicsPixmapItem(parent, scene)
{
	//this->setAcceptDrops(true);
	QWidget::setAcceptDrops(true);
	QGraphicsItem::setAcceptDrops(true);
	cout << "aangemaakt" << endl;
}
//***********************************************************************************************************

//***********************************************************************************************************
// Als er op de muis wordt geklikt ergens begint de drag and drop van de pijl
//***********************************************************************************************************
void ArrowItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
	QDrag *drag = new QDrag(this);		 
	QMimeData *mimeData = new QMimeData;
	QString temp;
	temp.append('a'); // a meegeven aan mime als het om een viewer (arrow) gaat, later nodig om te testen welke drop er gebeurd is
	mimeData->setText(temp);
	drag->setMimeData(mimeData);
	QPoint *punt = new QPoint();
	QSize size = this->pixmap().size();
	punt->setX(size.width()/2);
	punt->setY(size.height()/2);
	QPixmap image = this->pixmap(); // pixmap van pijl meegeven aan drag
	drag->setPixmap(image);
	drag->setHotSpot(drag->hotSpot() + *punt); // zodat de image centraal over muis staat
	Qt::DropAction dropAction = drag->exec();
}
//***********************************************************************************************************


//***********************************************************************************************************
// Begin drag
//***********************************************************************************************************
void ArrowItem::dragEnterEvent(QDragEnterEvent *event)
{
	event->acceptProposedAction();
	event->accept();
}
//***********************************************************************************************************


//***********************************************************************************************************
// Einde van de drag
//***********************************************************************************************************
void ArrowItem::dragLeaveEvent  ( QDropEvent *event ){
	event->acceptProposedAction();
}

//***********************************************************************************************************
// Pijl wordt gedropt
//***********************************************************************************************************
void ArrowItem::dropEvent(QDropEvent *event)
{
	event->acceptProposedAction();
}
//***********************************************************************************************************


//***********************************************************************************************************
