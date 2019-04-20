//***************************************************************************************************
// EDITOR.CPP
//***************************************************************************************************
// AUTEUR: NICK
//***************************************************************************************************

#include "Editor.h"

//** Constructor ***********************************************************************************
Editor::Editor(QWidget *parent) : QGraphicsView(parent)
{
	m_Scale = 1.0;
	
	pix = new QPixmap(floor(64*m_Scale),floor(64*m_Scale));
	m_View = new QGraphicsScene;

	copy = NULL;
	selectedEditItem = NULL;

	arrow = NULL;
	arrowItem = NULL;

	draw = false;
	isCtrl = false;
}
//**************************************************************************************************


//** Destructor ************************************************************************************
Editor::~Editor()
{
	delete(pix);
	delete(m_View);
		
}
//**************************************************************************************************


//**************************************************************************************************
// De pointer naar een wereld veranderen
//**************************************************************************************************
void Editor::SetWorld(World* w, Viewer* v)
{
	m_World = w;
	m_Viewer = v;
}
//**************************************************************************************************


//**************************************************************************************************
// Editor legen
//**************************************************************************************************
void Editor::Reset()
{
	//if (m_View != NULL){
		delete(m_View);
		m_View=NULL;
		draw = false;
	//}
	if (copy != NULL){
		delete(copy);
		copy = NULL;
	}
}
//**************************************************************************************************



//**************************************************************************************************
// View aanpassen
// Bij het aanroepen van deze functie wordt de overeenkomstige view ingeladen in de editor
// Naargelang de view wordt een extra functie aangeroepen om een juiste afbeelding in een item te zetten
//**************************************************************************************************
void Editor::SetView(view v){
	// geen copy gegevens meer beschikbaar bij nieuwe platte grond
	if (copy != NULL){
		delete(copy);
		copy = NULL;
	}
	draw = true;
	selectedEditItem = NULL;
	emit RefreshEditTextures();
	m_CurrentView = v;
	delete(pix);
	// pixmap opnieuw aanmaken om de schaal in orde te houden
	pix = new QPixmap(floor(64*m_Scale),floor(64*m_Scale));
	delete(m_View);
	m_View = new QGraphicsScene; // al de items van de scene deleten
	// elke cel in world wordt een Item in de editor
	for(int i=0; i<m_World->WorldHeight(); i++){
		for (int j = 0; j<m_World->WorldWidth(); j++){
			p = new EditQGraphicsItem(i, j);
			QString grid = "(";
			grid += QString::number(j);
			grid += ",";
			grid += QString::number(i);
			grid += ")";
			p->SetTip(grid);
			//(p->)Widget::setAcceptDrops(true);
			//p->setZValue(0.0);
			// signaal voor het selecteren van een item
			connect(p, SIGNAL(ItemClick(EditQGraphicsItem*)), this, SLOT(ClickItem(EditQGraphicsItem*)));
			connect(p, SIGNAL(DropArrow(int, int, int, int)), this, SLOT(DropArrow(int, int, int, int)));
			connect(p, SIGNAL(Drop(EditQGraphicsItem *, EditQGraphicsItem *)), this, SLOT(Drop(EditQGraphicsItem *, EditQGraphicsItem *)));
			if (v == WALL)
				FillPixmapWall(pix, i, j); // speciale texture voor muren
			else
				FillPixmap(pix, i, j, v); // textures voor vloer en plafond
			p->setPixmap(*pix);
			m_View->addItem( p );
			p->setPos(i*floor(64*m_Scale), j*floor(64*m_Scale));
		}
	}
	arrowItem = new ArrowItem();
	m_View->addItem( arrowItem );
	arrowItem->setZValue ( 6.0 ); // breng de pijl voor de andere items
//	arrowItem->setAcceptDrops(true);
	setScene(m_View);
	DrawArrow();
}
//**************************************************************************************************


//**************************************************************************************************
// Functie om een texture te zetten voor FLOOR en CEILING
//**************************************************************************************************
void Editor::FillPixmap(QPixmap* pix, int gridX, int gridY, view v)
{
	int nieuwx = 64*m_Scale;
	int nieuwy = 64*m_Scale;
	static QImage image(64, 64, QImage::Format_RGB32 );
	if (v==FLOOR)
		image = *(m_World->textures.getm((*m_World)[gridX][gridY].floor));
	else if (v==CEILING)
		image = *(m_World->textures.getm((*m_World)[gridX][gridY].ceil));
	DrawSprite(&image, gridX, gridY);
	DrawWalkThrough(&image, gridX, gridY);
	image = image.scaled(nieuwx, nieuwy); // image verkleinen naar de juiste schaal
	if ((*m_World)[gridX][gridY].wall){
		pix->fill(Qt::red);	// image wordt rood gezet als er een muur is
	}
	else {
		*pix = pix->fromImage(image);
	}
}
//**************************************************************************************************


//**************************************************************************************************
// geeft aan in ceiling en floor dat er een sprite is
//**************************************************************************************************
void Editor::DrawSprite(QImage *image, int gridX, int gridY)
{
	if (m_World->isSprite(gridX,gridY)){
		QColor value;
		if ( (*m_World)[gridX][gridY].walktrough )
			value = QColor(0,200, 0);
		else 
			value = QColor(200,0, 0);
		for (int i=-7; i<7; i++){
			for (int j=-7; j<7; j++){
				image->setPixel(32+i, 32+j, value.rgb());
			}
		}
		value = QColor(255,255, 255);
		for (int i=-8; i<8; i++){
			image->setPixel(32-8, 32+i, value.rgb());
			image->setPixel(32+8, 32-i, value.rgb());
			image->setPixel(32-i, 32+8, value.rgb());
			image->setPixel(32+i, 32-8, value.rgb());
		}
	}
}
//**************************************************************************************************


//**************************************************************************************************
// geeft aan of je door een vloer mag lopen of niet
//**************************************************************************************************
void Editor::DrawWalkThrough(QImage *image, int gridX, int gridY)
{
	if ((*m_World)[gridX][gridY].walktrough == false && !m_World->isSprite(gridX,gridY)){
		QColor value;
		value = QColor(200, 0, 0);

		for (int i=0; i<64; i++){
			image->setPixel(0, i, value.rgb());
			image->setPixel(1, i, value.rgb());
			image->setPixel(62, i, value.rgb());
			image->setPixel(63, i, value.rgb());
			image->setPixel(i, 0, value.rgb());
			image->setPixel(i, 1, value.rgb());
			image->setPixel(i, 63, value.rgb());
			image->setPixel(i, 62, value.rgb());
		}
	}
}
//**************************************************************************************************


//**************************************************************************************************
// functie om texture te zetten van de muren
//**************************************************************************************************
void Editor::FillPixmapWall(QPixmap* pix, int gridX, int gridY)
{
	int nieuwx = 64*m_Scale;
	int nieuwy = 64*m_Scale;
	static QImage image(64, 64, QImage::Format_RGB32 );
	static QImage imageNew(64*m_Scale, 64*m_Scale, QImage::Format_RGB32 ); // image verkleinen op schaal
	int i;
	int j;
	
	if ((*m_World)[gridX][gridY].wall){
		// item opdelen in vier en voor elke kant van een muur een deel opvullen
		for (i=0; i<32; i++){
			for(j=i; j<64-i; j++){
				image.setPixel(i, j, m_World->textures.getm((*m_World)[gridX][gridY].west)->pixel(i,j));
				image.setPixel(63-i, j, m_World->textures.getm((*m_World)[gridX][gridY].east)->pixel(63-i,j));
			}
			for(j=i; j<64-i; j++){
				image.setPixel(j, i, m_World->textures.getm((*m_World)[gridX][gridY].north)->pixel(j,i));
				image.setPixel(j, 63-i, m_World->textures.getm((*m_World)[gridX][gridY].south)->pixel(j,63-i));
			}
		}
		imageNew = image.scaled(nieuwx, nieuwy);
		*pix = pix->fromImage(imageNew);
	}
	else if (m_World->isSprite(gridX,gridY))
	{
		// als er een sprite is, deze uittekenen
		QColor value;
		value = QColor(0,0, 200);
		image = *(m_World->textures.getm((*m_World)[gridX][gridY].sprite));
		if ( (*m_World)[gridX][gridY].walktrough )
			value = QColor(0,200, 0);
		else 
			value = QColor(200,0, 0);
		for (int i=0; i<64; i++){
			image.setPixel(0, i, value.rgb());
			image.setPixel(1, i, value.rgb());
			image.setPixel(2, i, value.rgb());
			image.setPixel(64-1, i, value.rgb());
			image.setPixel(64-2, i, value.rgb());
			image.setPixel(64-3, i, value.rgb());
			image.setPixel(i, 0, value.rgb());
			image.setPixel(i, 1, value.rgb());
			image.setPixel(i, 2, value.rgb());
			image.setPixel(i, 64-1, value.rgb());
			image.setPixel(i, 64-2, value.rgb());
			image.setPixel(i, 64-3, value.rgb());
		}
		imageNew = image.scaled(nieuwx, nieuwy);
		*pix = pix->fromImage(imageNew);
	}
	else {
		QColor value;
		value = QColor(255, 255, 255);
		image.fill(value.rgb());
		DrawWalkThrough(&image, gridX, gridY);
		imageNew = image.scaled(nieuwx, nieuwy);
		*pix = pix->fromImage(imageNew);
	}
}
//**************************************************************************************************


//**************************************************************************************************
// functie om een texture van een item up to date te houden met world gegevens
//**************************************************************************************************
void Editor::UpdateTexture(EditQGraphicsItem* item, bool state)
{
	if (m_CurrentView == WALL)
		FillPixmapWall(pix, item->gridX, item->gridY);
	else
		FillPixmap(pix, item->gridX, item->gridY, m_CurrentView);
	item->setPixmap(*pix);
	if (state)
		SetView(m_CurrentView);
	else
		emit SelectItem(item);
}
//**************************************************************************************************


//**************************************************************************************************
// Slot: Krijgt een item mee waarop geklikt is en zet deze in selectedEditItem
// Deze geeft ook een signaal naar andere klasses
//**************************************************************************************************
void Editor::ClickItem(EditQGraphicsItem * item)
{
	selectedEditItem = item;
	// geeft een waarschuwing aan het hoofdprogramma dat er een item is geselecteerd
	emit SelectItem(item);
}
//**************************************************************************************************


//**************************************************************************************************
// Slot: Deze wordt aangesproken bij CTR+C
// De celgegevens van het geselecteerde item worden opgeslagen in de copy
//**************************************************************************************************
void Editor::Copy()
{
	// afbreken van oudere gegevens om memory leaks tegen te gaan
	if (copy != NULL){
		delete(copy);
	}
	// de celgegevens van een geselecteerd vakje overkopieren in de copy
	if (selectedEditItem != NULL)
		copy = new cel((*m_World)[selectedEditItem->gridX][selectedEditItem->gridY]);
}
//**************************************************************************************************


//**************************************************************************************************
// Slot: Deze wordt aangesproken bij CTR+V
// De celgegevens van copy worden in de huidige geselecteerde item geplaatst en ververst
//**************************************************************************************************
void Editor::Past()
{
	// enkel als er copy gegevens zijn mag er gepast worden
	if (copy != NULL){
		// geselecteerd vakje uit world updaten met de copy gegevens
		if ( !(m_World->isBoundary(selectedEditItem->gridX, selectedEditItem->gridY) && !copy->wall)){
			(*m_World)[selectedEditItem->gridX][selectedEditItem->gridY] = *copy;
			UpdateTexture(selectedEditItem, false); // texture van het item updaten
			emit SelectItem(selectedEditItem);
		}
	}
}
//**************************************************************************************************


//**************************************************************************************************
// Den viewer uittekenen in de editor
//**************************************************************************************************
void Editor::DrawArrow()
{
	// als editor getekend is enkel pixmap laten veranderen anders bestaat deze niet
	if (draw){
			
		static QPixmap pixmap(64, 64);
		pixmap.load("images/arrow.png");
		QPixmap scalePixmap;
		scalePixmap = pixmap;
		scalePixmap = scalePixmap.scaled(floor(64*m_Scale), floor(64*m_Scale));
		scalePixmap = scalePixmap.transformed(QTransform().translate(-floor(64*m_Scale)/2, -floor(64*m_Scale)/2).rotate(-(m_Viewer->viewAngle-90)).translate(floor(64*m_Scale)/2, floor(64*m_Scale)/2));
		arrowItem->setPixmap(scalePixmap);
		arrowItem->setPos(((m_Viewer->x / 64.0) * floor(64*m_Scale) - floor(64*m_Scale/2)), ((m_Viewer->y / 64.0) * floor(64*m_Scale) - floor(64*m_Scale/2)));
	}
}
//**************************************************************************************************


//**************************************************************************************************
// Een signaal doorgekregen van een item die zegt dat de viewer ergens is gedropt
//**************************************************************************************************
void Editor::DropArrow(int posx, int posy, int gridX, int gridY)
{
	cout << "(" << posx << ", " << posy << ")" << " met grid: " << "(" << gridX << ", " << gridY << ")" << endl;
	if (!(*m_World)[gridX][gridY].wall && (*m_World)[gridX][gridY].walktrough){
		m_Viewer->x = gridX*64.0 + ((posx / (64.0 * m_Scale))*64.0);
		m_Viewer->y = gridY*64.0 + ((posy / (64.0 * m_Scale))*64.0);
		DrawArrow();
	}
	emit ViewerChanged();
}
//**************************************************************************************************


//**************************************************************************************************
// Een signaal doorgekregen van een item die zegt dat er een andere item is gedropt
//**************************************************************************************************
void Editor::Drop(EditQGraphicsItem *from, EditQGraphicsItem *to)
{
		// geselecteerd vakje uit world updaten met de copy gegevens
		if (!(from->gridX==to->gridX && from->gridY==to->gridY) ){
			if (this->isCtrl && (!m_World->isBoundary(to->gridX, to->gridY) || (*m_World)[from->gridX][from->gridY].wall)){ // vakje kopieren
				(*m_World)[to->gridX][to->gridY] = (*m_World)[from->gridX][from->gridY];
				UpdateTexture(to, false); // texture van het item updaten
				UpdateTexture(from, false);
				cout << "Editor:drag met ctrl" << endl;
			}
			else { // vakje verplaatsen
				if (!m_World->isBoundary(to->gridX, to->gridY)){
					(*m_World)[to->gridX][to->gridY] = (*m_World)[from->gridX][from->gridY];
					ClearItem(from); // maakt het vakje in zijn standaardvorm
					UpdateTexture(to, false); // texture van het item updaten
					UpdateTexture(from, false);
					cout << "Editor:drag zonder ctrl" << endl;
					emit SelectItem(to);
				}
			}
		}

}
//**************************************************************************************************


//**************************************************************************************************
// Het opvangen van de globale key-events
// Deze worden ook doorgestuurd naar de navigatieklasse
//**************************************************************************************************
void Editor::keyPressEvent ( QKeyEvent * e ){

	
	if ( e->isAutoRepeat() )
    {
         e->ignore();
         return;
    }
	if(e->key()==Qt::Key_Control){
		this->isCtrl = true;	
		cout << "control pressed" <<endl;
	}
	if(e->key()==Qt::Key_Delete){
		if (this->selectedEditItem != NULL){
			ClearItem(selectedEditItem); // maakt het vakje in zijn standaardvorm
			UpdateTexture(selectedEditItem, false);
		}
	}

	
}
//**************************************************************************************************


//**************************************************************************************************
// KeyRealeaseEvents worden doorgestuurd naar de navigatieklasse
//**************************************************************************************************
void Editor::keyReleaseEvent ( QKeyEvent * e )
{
	
	/*if ( e->isAutoRepeat() )
    {
         e->ignore();
         return;
    }*/
	if(e->key()==Qt::Key_Control){
		this->isCtrl = false;	
		cout << "control released" <<endl;
	}
}
//**************************************************************************************************

//**************************************************************************************************
// Maakt een vakje helemaal leeg
//**************************************************************************************************
void Editor::ClearItem(EditQGraphicsItem * item)
{
	if (m_World->isBoundary(item->gridX, item->gridY))
		(*m_World)[item->gridX][item->gridY].wall=true;
	else
		(*m_World)[item->gridX][item->gridY].wall=false;
	(*m_World)[item->gridX][item->gridY].north = 0;
	(*m_World)[item->gridX][item->gridY].south = 0;
	(*m_World)[item->gridX][item->gridY].east = 0;
	(*m_World)[item->gridX][item->gridY].west = 0;
	(*m_World)[item->gridX][item->gridY].trigger=0;
	(*m_World)[item->gridX][item->gridY].sprite=0;
	(*m_World)[item->gridX][item->gridY].floor = 0;
	(*m_World)[item->gridX][item->gridY].ceil = 0;
	(*m_World)[item->gridX][item->gridY].isSprite=false;
	(*m_World)[item->gridX][item->gridY].walktrough=true;
}
//**************************************************************************************************


