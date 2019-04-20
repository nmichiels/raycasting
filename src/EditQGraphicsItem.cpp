//***************************************************************************************************
// EDITQGRAPHICSITEM.CPP
// Item die een cel van de wereld voorstelt in de editor
//***************************************************************************************************
// AUTEUR: NICK
//***************************************************************************************************

#include "EditQGraphicsItem.h"
#include <iostream>
using namespace std;

//** Constructor ************************************************************************************************
EditQGraphicsItem::EditQGraphicsItem(int x, int y, QGraphicsItem * parent, QGraphicsScene * scene) : gridX(x), gridY(y), QGraphicsPixmapItem(parent, scene)
{
	QGraphicsItem::ItemIsFocusable;
	QWidget::setAcceptDrops(true);
	QGraphicsItem::setAcceptDrops(true);
	//dragItem = NULL;
}
//***************************************************************************************************************


//** Destructor *************************************************************************************************
EditQGraphicsItem::~EditQGraphicsItem(){};
//***************************************************************************************************************


//***************************************************************************************************************
// Een item wordt gedropt, er moet getest worden of er een viewer gedropt is of een ander vakje
//***************************************************************************************************************
void EditQGraphicsItem::dropEvent(QGraphicsSceneDragDropEvent *event)
{
	event->acceptProposedAction();
	QString string = event->mimeData()->text();
	if (string == "a"){
		emit DropArrow(event->pos().x(), event->pos().y(), this->gridX, this->gridY);
	}
	else {
		emit Drop(dragItem, this);
	}

}
//***************************************************************************************************************


//***************************************************************************************************************
// Informatie bij het vakje zetten zodat u gemakkelijk kan aflezen bij welke cel het hoort
//***************************************************************************************************************
void EditQGraphicsItem::SetTip(QString tip)
{
	QGraphicsPixmapItem::setToolTip(tip);
}
//***************************************************************************************************************


//***************************************************************************************************************
// Bij het klikken op een item moet er de drag and drop van beginnen
//***************************************************************************************************************
void EditQGraphicsItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{

	QDrag *drag = new QDrag(this);		 
	QMimeData *mimeData = new QMimeData;
	QString temp;
	temp.append('n');  // n megeven aan mime voor drag van gewoon item
	mimeData->setText(temp);
	drag->setMimeData(mimeData);
	QPoint point;
	QSize size = this->pixmap().size();
	point.setX(size.width()/2);
	point.setY(size.height()/2);
	QPixmap image = this->pixmap(); // pixmap van item meegeven aan drag
	drag->setPixmap(image);
	drag->setHotSpot(drag->hotSpot() + point); // zodat de image centraal over muis staat
	dragItemGridX = this->gridX;
	dragItemGridY = this->gridY;
	dragItem = this;

	Qt::DropAction dropAction = drag->exec();
	emit ItemClick(this);
}
//***************************************************************************************************************


//***************************************************************************************************************
// Begin van een drag
//***************************************************************************************************************
void EditQGraphicsItem::dragEnterEvent(QDragEnterEvent *event)
{
	event->acceptProposedAction();
	event->accept();
}
//***************************************************************************************************************


//***************************************************************************************************************
// Einde van een drag
//***************************************************************************************************************
void EditQGraphicsItem::dragLeaveEvent  ( QDropEvent *event ){
	event->acceptProposedAction();
}
//***************************************************************************************************************