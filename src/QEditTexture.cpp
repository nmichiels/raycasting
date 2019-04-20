//***************************************************************************************************
// QEDITTEXTURE.CPP
// De items die de huidige textures van een geselecteerd object weergeven
//***************************************************************************************************
// AUTEUR: NICK
//***************************************************************************************************

#include "QEditTexture.h"

//** Constructor ********************************************************************************************
QEditTexture::QEditTexture(wichEdit sort, QGraphicsItem * parent, QGraphicsScene * scene) 
	: sort(sort), QGraphicsPixmapItem(parent, scene)
{}
//***********************************************************************************************************


//***********************************************************************************************************
// Deze geeft een signaal aan de hoofdklasse dat er geklikt is om een texture te veranderen
// Bij dit signaal geeft hij ook zijn eigen pointer door om te weten welke texture er moet veranderd worden
//***********************************************************************************************************
void QEditTexture::mousePressEvent ( QGraphicsSceneMouseEvent * event )
{
	emit TextureClick(this);
}
//***********************************************************************************************************
