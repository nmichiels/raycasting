//***************************************************************************************************
// QTEXTURESWINDOW.H
// De klasse die aan de window hangt om textures aan te passen
//***************************************************************************************************
// AUTEUR: NICK
//***************************************************************************************************

#include "QTexturesWindow.h"
#include <iostream>
using namespace std;

//** Constructor ***********************************************************************************
QTexturesWindow::QTexturesWindow(World *w) : m_World(w)
{
	texturesWindow.setupUi(this);

	scene = new QGraphicsScene;

	state = false;

	pix = new QPixmap(64, 64);
	tex = new QGraphicsPixmapItem;
	scene->addItem(tex);
	texturesWindow.textureGraphicsView->setScene(scene);

	selectedTextureEditItem = NULL; // verlopig geen texture geselecteerd om te veranderen
	selectedEditItem = NULL; // verlopig geen cel geselecteerd

	texturesWindow.spinBox->setMaximum((*m_World).textures.getSize()-1);  // je mag niet hoger gaan dan het aantal textures

	this->connect(this, SIGNAL(accepted()), this, SLOT(accept()));
	this->connect(this, SIGNAL(rejected ()), this, SLOT(reject()));
}
//**************************************************************************************************


//** Destructor ************************************************************************************
QTexturesWindow::~QTexturesWindow()
{	
	delete(tex);
	delete(pix);
	delete(scene);
	
}
//**************************************************************************************************


//**************************************************************************************************
// Bij het veranderen van een waarde in de spinbox moet de overeenkomstige texture worden getoond
//**************************************************************************************************
void QTexturesWindow::on_spinBox_valueChanged(int val)
{
	static QImage image(64, 64, QImage::Format_RGB32 );
	if (val == 0) // als 0 wordt gekozen krijg je een verbodsteken
		image.load("images/no.png");
	else
		image = *(m_World->textures.getm(val));
	*pix = pix->fromImage(image);
	tex->setPixmap(*pix);

}
//**************************************************************************************************


//**************************************************************************************************
// Bij het drukken op ok wordt deze accept-functie aangesproken
// Deze zorgt ervoor dat navenant de soort van het geselecteerde texture om te veranderen...
// ...de juiste texture in de world klasse wordt aangepast
//**************************************************************************************************
void QTexturesWindow::accept()
{
	int val = texturesWindow.spinBox->value();
	switch(this->selectedTextureEditItem->sort){
		case EDIT_FLOOR:
			(*m_World)[this->selectedEditItem->gridX][this->selectedEditItem->gridY].floor = val;
			break;
		case EDIT_CEILING:
			(*m_World)[this->selectedEditItem->gridX][this->selectedEditItem->gridY].ceil = val;
			break;
		case EDIT_SPRITE:
			(*m_World)[this->selectedEditItem->gridX][this->selectedEditItem->gridY].sprite = val;
			break;
		case EDIT_WALL_NORTH:
			(*m_World)[this->selectedEditItem->gridX][this->selectedEditItem->gridY].north = val;
			break;
		case EDIT_WALL_WEST:
			(*m_World)[this->selectedEditItem->gridX][this->selectedEditItem->gridY].west = val;
			break;
		case EDIT_WALL_SOUTH:
			(*m_World)[this->selectedEditItem->gridX][this->selectedEditItem->gridY].south = val;
			break;
		case EDIT_WALL_EAST:
			(*m_World)[this->selectedEditItem->gridX][this->selectedEditItem->gridY].east = val;
			break;
	}
	selectedTextureEditItem = NULL;
	this->hide();
	emit UpdateTexture(selectedEditItem, state); // De texture die weergeeft welke er instaat moet upgedate worden
}
//**************************************************************************************************


//**************************************************************************************************
// Als op cancel of op kruisje wordt gedrukt dan wordt deze functie aangesproken
// Deze zorgt ervoor dat het kaderje weggaat en de geselecteerde item om te wijzigen op NULL wordt gezet
//**************************************************************************************************
void QTexturesWindow::reject()
{
	selectedTextureEditItem = NULL;
	this->hide();
}
//**************************************************************************************************


//**************************************************************************************************
// Nieuwe texture toevoegen aan de lijst vanuit file
//**************************************************************************************************
void QTexturesWindow::on_pushButtonTexture_clicked()
{
	QString fileName = QFileDialog::getOpenFileName(this, tr("Afbeelding openen"), "", "Afbeeldingen (*.bmp *.png *.xpm *.jpg *.gif);;Alle bestanden (*)");
	if(fileName != ""){
		emit TextureAdded(fileName);
	}
}
//**************************************************************************************************


//**************************************************************************************************
// De spinbox op een bepaalde value zetten
//**************************************************************************************************
void QTexturesWindow::SetTextureValue(int value)
{
	cout << "QTexturesWindow: maximum voor: " << texturesWindow.spinBox->maximum() <<endl;
	cout << "QTexturesWindow: maximum na: " << texturesWindow.spinBox->maximum() <<endl;
	cout << "QTexturesWindow: value voor: " << texturesWindow.spinBox->value() <<endl;
	texturesWindow.spinBox->setValue(value);
	cout << "QTexturesWindow: value na: " << texturesWindow.spinBox->value() <<endl;
}
//**************************************************************************************************


//**************************************************************************************************
// De maximale waarde van de spinbox zetten (deze moet veranderen als er een texture wordt toegevoegd
//**************************************************************************************************
void QTexturesWindow::SetTextureMaxValue(int value)
{
	cout << "QTexturesWindow: maximum voor: " << texturesWindow.spinBox->maximum() <<endl;
	texturesWindow.spinBox->setMaximum((*m_World).textures.getSize()-1);
	cout << "QTexturesWindow: maximum na: " << texturesWindow.spinBox->maximum() <<endl;
	cout << "QTexturesWindow: value voor: " << texturesWindow.spinBox->value() <<endl;
	cout << "QTexturesWindow: value na: " << texturesWindow.spinBox->value() <<endl;
}
//**************************************************************************************************


//**************************************************************************************************
// Een texture verwijderen uit de lijst
//**************************************************************************************************
void QTexturesWindow::on_pushButtonDeleteTexture_clicked()
{
	state = true;
	emit TextureDeleted(texturesWindow.spinBox->value());
}
//**************************************************************************************************


//**************************************************************************************************
// Een texture wijzeigen door een andere uit een file te halen
//**************************************************************************************************
void QTexturesWindow::on_pushButtonEditTexture_clicked()
{
	QString fileName = QFileDialog::getOpenFileName(this, tr("Afbeelding openen"), "", "Afbeeldingen (*.bmp *.png *.xpm *.jpg *.gif);;Alle bestanden (*)");
	if(fileName != ""){
		state = true;
		emit TextureEdited(fileName, texturesWindow.spinBox->value());
	}
}
//**************************************************************************************************