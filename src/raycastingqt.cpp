//***************************************************************************************************
// RAYCASTINGQT.CPP
// Deze klasse koppelt alle klasse. Hij start de raycasting engine en zorgt vooral al voor een
// uitgebreide basis voor de editor en GUI
//***************************************************************************************************
// AUTEUR: NICK
// SUBAUTEUR: KENNETH
//***************************************************************************************************

#include "raycastingqt.h"

//** Constructor ***********************************************************************************
RaycastingQT::RaycastingQT(QWidget *parent, Qt::WFlags flags) : QMainWindow(parent, flags)
{
	this->world = NULL;
	this->viewer = NULL;
	//this->textures = NULL;
	this->navigatie = NULL;
	this->engine = NULL;
	this->texturesWindow = NULL;
	ui.setupUi(this);
	//lastTick=GetTickCount();
	//currTick=GetTickCount();
	nr=0;

	world = new World(0, 32, 32);
	viewer=new Viewer();
//	textures = new Textures();
	//File::load("wiii.txt",world, viewer, &triggers);
	//texturesWindow = new QTexturesWindow(world);
	ui.editor->SetWorld(world, viewer);
	navigatie = new Navigation(world, viewer);//, ui.scr);
	engine = new Engine(world, viewer, navigatie,&ui,&triggers);
	texturesWindow = new QTexturesWindow(world);
	triggersWindow = new TriggersWindow(world, &triggers);


	// Kompas instellen
	QPixmap pixCompass(260,260);
	pixCompass.load("images/compassWood.png");
	compassScene = new QGraphicsScene;
	compassScene->addPixmap(pixCompass);
	ui.kompas->setScene(compassScene);

	InitializeEditor();
	connect(ui.scr, SIGNAL(move(QMouseEvent *)), this, SLOT(move(QMouseEvent *)));
	connect(ui.actionCopy, SIGNAL(triggered()), ui.editor, SLOT(Copy()));
	connect(ui.actionPast, SIGNAL(triggered()), ui.editor, SLOT(Past()));
	connect(this->engine, SIGNAL(ScreenUpdate()), this, SLOT(ScreenUpdate()));

	connect(ui.actionQuit, SIGNAL(triggered()), this, SLOT(Quit()));
	connect(ui.actionSave, SIGNAL(triggered()), this, SLOT(Save()));
	connect(ui.actionLoad, SIGNAL(triggered()), this, SLOT(Load()));
	connect(ui.actionNieuwe_Wereld, SIGNAL(triggered()), this, SLOT(New()));
	connect(ui.actionNieuwe_Grootte, SIGNAL(triggered()), this, SLOT(Resize()));
	connect(ui.actionEditor, SIGNAL(triggered()), this, SLOT(SetTabEditor()));
	connect(ui.actionRaycasting, SIGNAL(triggered()), this, SLOT(SetTabRaycasting()));
	connect(ui.tabWidget, SIGNAL(currentChanged(int)), this, SLOT(SetTab(int)));

	connect(ui.actionInzoomen, SIGNAL(triggered()), this, SLOT(on_MiniMapPlus_clicked()));
	connect(ui.actionUitzoomen, SIGNAL(triggered()), this, SLOT(on_MiniMapMin_clicked()));
	connect(ui.actionAbout, SIGNAL(triggered()), this, SLOT(About()));




	//connect(navigatie, SIGNAL(PositionChanged()), this, SLOT(PositionChanged()));



	
}
//**************************************************************************************************


//**************************************************************************************************
// Deze funcitie is een constructor voor alles wat met de editor te maken heeft
// Hier worden ook de nodige connects aangemaakt
//**************************************************************************************************
void RaycastingQT::InitializeEditor(void)
{
	connect(ui.editor, SIGNAL(SelectItem(EditQGraphicsItem*)), this, SLOT(SelectItem(EditQGraphicsItem*)));
	connect(ui.checkBoxWall, SIGNAL(stateChanged(int)), this, SLOT(WallChanged(int)));
	connect(ui.checkBoxSprite, SIGNAL(stateChanged(int)), this, SLOT(SpriteChanged(int)));
	connect(ui.checkBoxWalkThrough, SIGNAL(stateChanged(int)), this, SLOT(WalkThroughChanged(int)));

	connect(ui.dialAngle, SIGNAL(valueChanged(int)), this, SLOT(AngleChanged(int)));

	connect(ui.spinBoxX, SIGNAL(valueChanged(int)), this, SLOT(XChanged(int)));
	connect(ui.spinBoxY, SIGNAL(valueChanged(int)), this, SLOT(YChanged(int)));

	connect(texturesWindow, SIGNAL(TextureAdded(QString)), this, SLOT(TextureAdded(QString)));
	connect(texturesWindow, SIGNAL(TextureDeleted(int)), this, SLOT(TextureDeleted(int)));
	connect(texturesWindow, SIGNAL(TextureEdited(QString, int)), this, SLOT(TextureEdited(QString, int)));

	connect(ui.editor, SIGNAL(ViewerChanged()), this, SLOT(ViewerChanged()));


	

	

	selectedEditItem = NULL;
	
	QPixmap* pix = new QPixmap(64, 64);
	pix->load("images/no.png");
	textureFloorScene = new QGraphicsScene;
	textureFloor = new QEditTexture(EDIT_FLOOR);
	textureFloor->setPixmap(*pix);
	textureFloorScene->addItem( textureFloor );
	ui.graphicsViewFloor->setScene(textureFloorScene);
	connect(textureFloor, SIGNAL(TextureClick(QEditTexture*)), this, SLOT(EditTexture(QEditTexture*)));

	textureFloor->blockSignals(1); // zolang er geen vakje geselecteerd is mag deze geen signalen doorlaten
	
	textureCeilingScene = new QGraphicsScene;
	textureCeiling = new QEditTexture(EDIT_CEILING);
	textureCeiling->setPixmap(*pix);
	textureCeilingScene->addItem( textureCeiling );
	ui.graphicsViewCeiling->setScene(textureCeilingScene);
	connect(textureCeiling, SIGNAL(TextureClick(QEditTexture*)), this, SLOT(EditTexture(QEditTexture*)));
	textureCeiling->blockSignals(1); // zolang er geen vakje geselecteerd is mag deze geen signalen doorlaten

	textureSpriteScene = new QGraphicsScene;
	textureSprite = new QEditTexture(EDIT_SPRITE);
	textureSprite->setPixmap(*pix);
	textureSpriteScene->addItem( textureSprite );
	ui.graphicsViewSprite->setScene(textureSpriteScene);
	connect(textureSprite, SIGNAL(TextureClick(QEditTexture*)), this, SLOT(EditTexture(QEditTexture*)));
	textureSprite->blockSignals(1); // zolang er geen vakje geselecteerd is mag deze geen signalen doorlaten

	textureWallNorthScene = new QGraphicsScene;
	textureWallNorth = new QEditTexture(EDIT_WALL_NORTH);
	textureWallNorth->setPixmap(*pix);
	textureWallNorthScene->addItem( textureWallNorth );
	ui.graphicsViewWallNorth->setScene(textureWallNorthScene);
	connect(textureWallNorth, SIGNAL(TextureClick(QEditTexture*)), this, SLOT(EditTexture(QEditTexture*)));
	textureWallNorth->blockSignals(1); // zolang er geen vakje geselecteerd is mag deze geen signalen doorlaten

	textureWallWestScene = new QGraphicsScene;
	textureWallWest = new QEditTexture(EDIT_WALL_WEST);
	textureWallWest->setPixmap(*pix);
	textureWallWestScene->addItem( textureWallWest );
	ui.graphicsViewWallWest->setScene(textureWallWestScene);
	connect(textureWallWest, SIGNAL(TextureClick(QEditTexture*)), this, SLOT(EditTexture(QEditTexture*)));
	textureWallWest->blockSignals(1); // zolang er geen vakje geselecteerd is mag deze geen signalen doorlaten

	textureWallSouthScene = new QGraphicsScene;
	textureWallSouth = new QEditTexture(EDIT_WALL_SOUTH);
	textureWallSouth->setPixmap(*pix);
	textureWallSouthScene->addItem( textureWallSouth );
	ui.graphicsViewWallSouth->setScene(textureWallSouthScene);
	connect(textureWallSouth, SIGNAL(TextureClick(QEditTexture*)), this, SLOT(EditTexture(QEditTexture*)));
	textureWallSouth->blockSignals(1); // zolang er geen vakje geselecteerd is mag deze geen signalen doorlaten

	textureWallEastScene = new QGraphicsScene;
	textureWallEast = new QEditTexture(EDIT_WALL_EAST);
	textureWallEast->setPixmap(*pix);
	textureWallEastScene->addItem( textureWallEast );
	ui.graphicsViewWallEast->setScene(textureWallEastScene);
	connect(textureWallEast, SIGNAL(TextureClick(QEditTexture*)), this, SLOT(EditTexture(QEditTexture*)));
	textureWallEast->blockSignals(1); // zolang er geen vakje geselecteerd is mag deze geen signalen doorlaten
	delete(pix);

	connect(this->texturesWindow, SIGNAL(UpdateTexture(EditQGraphicsItem*, bool)), ui.editor, SLOT(UpdateTexture(EditQGraphicsItem*, bool)));

	connect(ui.editor, SIGNAL(RefreshEditTextures()), this, SLOT(RefreshEditTextures()));

	

}
//**************************************************************************************************



//**************************************************************************************************
// Een nieuwe wereld creeren met als parameter de hoogte en breedte ervan
//**************************************************************************************************
void RaycastingQT::StartNewWorld(int height, int width)
{	
	ui.editor->Reset();
	viewer->Reset();
	world->resizeMap(height, width);
	world->Fill();
}
//**************************************************************************************************

//** Destructor ************************************************************************************
RaycastingQT::~RaycastingQT()
{
	delete(compassScene);
	delete(world);
	delete(viewer);
	delete(texturesWindow);
	delete(navigatie);
	delete(engine);
	delete(textureFloor);
	delete(textureCeiling);
	delete(textureWallNorth);
	delete(textureWallEast);
	delete(textureWallSouth);
	delete(textureWallWest);
	delete(textureFloorScene);
	delete(textureCeilingScene);
	delete(textureWallNorthScene);
	delete(textureWallEastScene);
	delete(textureWallSouthScene);
	delete(textureWallWestScene);
}
//**************************************************************************************************


//**************************************************************************************************
// Zorgt ervoor dat alles naast de screen wordt ververst
//**************************************************************************************************
void RaycastingQT::ScreenUpdate()
{
	ui.scr->update();
	ui.min->DrawMiniMap(world, viewer);
	ui.kompas->setTransform(QTransform().translate(125, 125).rotate(viewer->viewAngle-90).translate(-125, -125));
	ui.kompas->update();
	//currTick=GetTickCount();
	//float tickDiff=currTick-lastTick;
	//nr++;
	//lastTick=currTick;
	//char *a=new char[1000];
	//sprintf(a,"%f",(nr/tickDiff)*1000);
	//ui.text->setText(QString(a));
	//delete [] a;
}
//**************************************************************************************************


//**************************************************************************************************
// Deze gaat ervoor zorgen dat geen enkele texture meer toegankelijk is om te veranderen
// Er worden geen signalen doorgelaten en op de items wordt een verbodsteken gezet
//**************************************************************************************************
void RaycastingQT::RefreshEditTextures()
{
	selectedEditItem = NULL;

	ui.spinBoxTrigger->setEnabled(true); // je mag de waarde van triggers niet veranderen
	

	QPixmap* pix = new QPixmap(64, 64);
	pix->load("images/no.png");
	textureFloor->setPixmap(*pix);
	textureFloor->blockSignals(1); // zolang er geen vakje geselecteerd is mag deze geen signalen doorlaten
	
	textureCeiling->setPixmap(*pix);
	textureCeiling->blockSignals(1); // zolang er geen vakje geselecteerd is mag deze geen signalen doorlaten

	textureSprite->setPixmap(*pix);
	textureSprite->blockSignals(1); // zolang er geen vakje geselecteerd is mag deze geen signalen doorlaten

	textureWallNorth->setPixmap(*pix);
	textureWallNorth->blockSignals(1); // zolang er geen vakje geselecteerd is mag deze geen signalen doorlaten

	textureWallWest->setPixmap(*pix);
	textureWallWest->blockSignals(1); // zolang er geen vakje geselecteerd is mag deze geen signalen doorlaten

	textureWallSouth->setPixmap(*pix);
	textureWallSouth->blockSignals(1); // zolang er geen vakje geselecteerd is mag deze geen signalen doorlaten

	textureWallEast->setPixmap(*pix);
	textureWallEast->blockSignals(1); // zolang er geen vakje geselecteerd is mag deze geen signalen doorlaten
	delete(pix);
}
//**************************************************************************************************


//**************************************************************************************************
// Als de editor aangeeft dat er een muur is bijgekomen of weggehaald dan krijgt deze een signaal
// door emt de state van die muur
// De cel van de geselecteerde item wordt gewijzigd
// Als de cel op de rand van de world ligt kan een muur niet worden weggehaald
//**************************************************************************************************
void RaycastingQT::WallChanged(int state)
{
	if (selectedEditItem != NULL)
	{
		if (state == Qt::Checked){
			(*world)[selectedEditItem->gridX][selectedEditItem->gridY].wall = true;
		}
		else if (state == Qt::Unchecked){
			if (world->isBoundary(selectedEditItem->gridX, selectedEditItem->gridY)) { // als het de boundary van de map is mag de muur niet weg
				;
			}
			else
				(*world)[selectedEditItem->gridX][selectedEditItem->gridY].wall = false;
		}
		ui.editor->UpdateTexture(selectedEditItem, false);
	}
}
//**************************************************************************************************


//**************************************************************************************************
// Als de checkbox in de editor aangeeft dat een sprite aan of uitmoet komen we hier terecht
// De cel van de geselecteerde item wordt gewijzigd
//**************************************************************************************************
void RaycastingQT::SpriteChanged(int state)
{
	if (selectedEditItem != NULL)
	{
		if (state == Qt::Checked){
			(*world)[selectedEditItem->gridX][selectedEditItem->gridY].isSprite = true;
		}
		else if (state == Qt::Unchecked){
			(*world)[selectedEditItem->gridX][selectedEditItem->gridY].isSprite = false;
		}
		ui.editor->UpdateTexture(selectedEditItem, false);
	}
}
//**************************************************************************************************


//**************************************************************************************************
// Als de checkbox in de editor aangeeft dat een sprite doorloopbaar is of niet komen we hier terecht
// De cel van de geselecteerde item wordt gewijzigd
//**************************************************************************************************
void RaycastingQT::WalkThroughChanged(int state)
{
	if (selectedEditItem != NULL)
	{
		if (state == Qt::Checked){
			(*world)[selectedEditItem->gridX][selectedEditItem->gridY].walktrough = true;
		}
		else {
			(*world)[selectedEditItem->gridX][selectedEditItem->gridY].walktrough = false;
		}
		ui.editor->UpdateTexture(selectedEditItem, false);
	}
}
//**************************************************************************************************


//**************************************************************************************************
// Een signaal uit de editor zet deze functie in werking
// Er is een item geselecteerd om aan te passen
// Deze functie zorgt ervoor dat edit-texture velden klaargemaakt worden zodat ze gebruikt kunnen worden
//**************************************************************************************************
void RaycastingQT::SelectItem(EditQGraphicsItem* item)
{
	selectedEditItem = item;
	this->texturesWindow->selectedEditItem = item;
	QPixmap* pix = new QPixmap(64, 64);
	static QImage image(64, 64, QImage::Format_RGB32 );
	image = *(world->textures.getm((*world)[item->gridX][item->gridY].floor));
	*pix = pix->fromImage(image);
	textureFloor->setPixmap(*pix);

	image = *(world->textures.getm((*world)[item->gridX][item->gridY].ceil));
	*pix = pix->fromImage(image);
	textureCeiling->setPixmap(*pix);

	image = *(world->textures.getm((*world)[item->gridX][item->gridY].sprite));
	*pix = pix->fromImage(image);
	textureSprite->setPixmap(*pix);
	if (world->isSprite(item->gridX, item->gridY)){
		ui.checkBoxSprite->setChecked(true);
		textureSprite->blockSignals(0); // signalen doorlaten
	}
	else {
		ui.checkBoxSprite->setChecked(false);
		textureSprite->blockSignals(1); // signalen niet doorlaten
	}

	if ((*world)[selectedEditItem->gridX][selectedEditItem->gridY].walktrough == true )
		ui.checkBoxWalkThrough->setChecked(true);
	else
		ui.checkBoxWalkThrough->setChecked(false);



	if ((*world)[item->gridX][item->gridY].wall){

		image = *(world->textures.getm((*world)[item->gridX][item->gridY].north));
		*pix = pix->fromImage(image);
		textureWallNorth->setPixmap(*pix);

		image = *(world->textures.getm((*world)[item->gridX][item->gridY].east));
		*pix = pix->fromImage(image);
		textureWallEast->setPixmap(*pix);

		image = *(world->textures.getm((*world)[item->gridX][item->gridY].west));
		*pix = pix->fromImage(image);
		textureWallWest->setPixmap(*pix);

		image = *(world->textures.getm((*world)[item->gridX][item->gridY].south));
		*pix = pix->fromImage(image);
		textureWallSouth->setPixmap(*pix);
		ui.checkBoxWall->setChecked(true);

		textureWallEast->blockSignals(0); // signalen doorlaten
		textureWallWest->blockSignals(0); // signalen doorlaten
		textureWallNorth->blockSignals(0); // signalen doorlaten
		textureWallSouth->blockSignals(0); // signalen doorlaten
		textureFloor->blockSignals(0); // signalen doorlaten
		textureCeiling->blockSignals(0); // signalen doorlaten
		
	}
	else {
		pix->load("images/no.png");
		textureWallNorth->setPixmap(*pix);
		textureWallEast->setPixmap(*pix);
		textureWallWest->setPixmap(*pix);
		textureWallSouth->setPixmap(*pix);
		ui.checkBoxWall->setChecked(false);

		textureWallEast->blockSignals(1); // signalen doorlaten
		textureWallWest->blockSignals(1); // signalen doorlaten
		textureWallNorth->blockSignals(1); // signalen doorlaten
		textureWallSouth->blockSignals(1); // signalen doorlaten
		textureFloor->blockSignals(0); // signalen doorlaten
		textureCeiling->blockSignals(0); // signalen doorlaten
	}

	// trigger waarde updaten naar waarde van item en terug toegankelijk maken
	ui.spinBoxTrigger->setEnabled(true);
	ui.spinBoxTrigger->setValue((*world)[item->gridX][item->gridY].trigger);
}
//**************************************************************************************************


//**************************************************************************************************
// Het opvangen van de globale key-events
// Deze worden ook doorgestuurd naar de navigatieklasse
//**************************************************************************************************
void RaycastingQT::keyPressEvent ( QKeyEvent * e ){

	if (e->key() == Qt::Key_Plus)
		on_MiniMapPlus_clicked();
	if (e->key() == Qt::Key_Minus)
		on_MiniMapMin_clicked();
	
	if ( e->isAutoRepeat() )
    {
         e->ignore();
         return;
    }
	if(e->key()==Qt::Key_T)
		triggers.get((*world)[(viewer->x)/64][(viewer->y)/64].trigger)->startTasks(true,viewer,world,&ui);

	navigatie->keyPressEvent(e);
	
	

}
//**************************************************************************************************


//**************************************************************************************************
// KeyRealeaseEvents worden doorgestuurd naar de navigatieklasse
//**************************************************************************************************
void RaycastingQT::keyReleaseEvent ( QKeyEvent * e )
{
	
	if ( e->isAutoRepeat() )
    {
         e->ignore();
         return;
    }
	navigatie->keyReleaseEvent(e);
}
//**************************************************************************************************


//**************************************************************************************************
// Slot dat de schaal van de minimap gaat verkleinen
//**************************************************************************************************
void RaycastingQT::on_MiniMapMin_clicked()
{
	ui.min->ScaleAdd();
	ui.min->DrawMiniMap(world, viewer);
}
//**************************************************************************************************


//**************************************************************************************************
// Slot dat de schaal van de minimap gaat vergroten
//**************************************************************************************************
void RaycastingQT::on_MiniMapPlus_clicked()
{
	ui.min->ScaleSub();
	ui.min->DrawMiniMap(world, viewer);
}
//**************************************************************************************************


//**************************************************************************************************
// Slot die de mouseevents uit de screenklasse opvangt en deze doorstuurt naar de navigatieklasse
//**************************************************************************************************
void RaycastingQT::move( QMouseEvent * event  ) {
	navigatie->mouseMoveEvent(event);
}
//**************************************************************************************************


//**************************************************************************************************
// Als de schuifbalk van de zoom geschoven wordt gaat deze functie dit opvangen en de zoom...
// ... overeenkomstig veranderen
//**************************************************************************************************
void RaycastingQT::on_zoomEditor_valueChanged(int zoom)
{
	ui.editor->SetScale(zoom / 10.0);
	ui.editor->SetView(ui.editor->CurrentView());
}
//**************************************************************************************************


//**************************************************************************************************
// Slot die de view op FLOOR zet als er op pushButtonFloor wordt geklikt
//**************************************************************************************************
void RaycastingQT::on_pushButtonFloor_clicked()
{
	ui.editor->SetView(FLOOR);
}
//**************************************************************************************************


//**************************************************************************************************
// Slot die de view op CEILING zet als er op pushButtonPlafond wordt geklikt
//**************************************************************************************************
void RaycastingQT::on_pushButtonPlafond_clicked()
{
	ui.editor->SetView(CEILING);
}
//**************************************************************************************************


//**************************************************************************************************
// Slot die de view op WALL zet als er op pushButtonWall wordt geklikt
//**************************************************************************************************
void RaycastingQT::on_pushButtonWall_clicked()
{
	ui.editor->SetView(WALL);
}
//**************************************************************************************************


//**************************************************************************************************
// Slot die van de editor klasse een signaal krijgt dat er op een texture is geklikt om aan te passen
// Deze functie gaat de window openen om de texture aan te passen
// Ook gaat hij de geselecteerde texture bijhouden om te weten welke hij moet aanpassen als er een
// ...texture is gekozen
//**************************************************************************************************
void RaycastingQT::EditTexture(QEditTexture* item)
{
	texturesWindow->show();
	texturesWindow->state = false;
	texturesWindow->selectedTextureEditItem = item;
}
//**************************************************************************************************


//**************************************************************************************************
// Raycasting afsluiten
//**************************************************************************************************
void RaycastingQT::Quit(void)
{
	int p;
	p = QMessageBox::question(ui.widget, "Raycasting afsluiten", "\nBent u zeker dat u deze wereld wil afsluiten?\n", QMessageBox::Ok, QMessageBox::Cancel, QMessageBox::Save);
	if (p == QMessageBox::Ok){
		this->close();
	}
	else if (p == QMessageBox::Save){
	}

}
//**************************************************************************************************


//**************************************************************************************************
// Opslaan van de wereld waar de gebruiker kan kiezen waar hij het wil
//**************************************************************************************************
void RaycastingQT::Save(void)
{
	QString fileName = QFileDialog::getSaveFileName(ui.widget, tr("Deze wereld opslaan"), "/home/untitled.txt", tr("Raycasting-wereld (*.txt)"));
	if(fileName != ""){
		File::save(fileName.toUtf8().data(), world, viewer, &triggers);
		world->fileName=fileName;
	}
}
//**************************************************************************************************


//**************************************************************************************************
// laden van een wereld waar de gebruiker kan uit een file kiezen
//**************************************************************************************************
void RaycastingQT::Load(void)
{
	texturesWindow->SetTextureValue(0);
	texturesWindow->SetTextureMaxValue(0);
	QString fileName = QFileDialog::getOpenFileName(this, tr("Spel openen"), "/home/untitled.txt", tr("Raycasting-wereld (*.txt)"));
	if(fileName != ""){
		if (!File::load(fileName.toUtf8().data(), world, viewer, &triggers))
			QMessageBox::warning ( NULL, "Fout bij laden de wereld", "\nBestand niet gevonden... \n");
		else{
			world->fileName=fileName;
			ui.editor->Reset();
			ui.pic->hide();
			ui.text->setText("");
			ui.text->update();	
			ui.pic->setGeometry(680,20,5,5);
		}
	}
	texturesWindow->SetTextureValue(0);
	texturesWindow->SetTextureMaxValue(world->textures.getSize()-1);
}
//**************************************************************************************************


//**************************************************************************************************
// nieuwe wereld laten starten
//**************************************************************************************************
void RaycastingQT::New(void)
{
	bool okHeight;
	bool okWidth;
	int width;
	int height =  QInputDialog::getInteger ( ui.widget, "Hoogte van uw wereld", "hoogte: ", 32, 5, 300, 1, &okHeight);
	if (okHeight)
		width =  QInputDialog::getInteger ( ui.widget, "Breedte van uw wereld", "breedte: ", 32, 5, 300, 1, &okWidth);
	if (okHeight && okWidth)
		StartNewWorld(height, width);
}
//**************************************************************************************************


//**************************************************************************************************
// Grootte van de wereld aanpassen
//**************************************************************************************************
void RaycastingQT::Resize(void)
{
	bool okHeight;
	bool okWidth;
	int width;
	int height =  QInputDialog::getInteger ( ui.widget, "Hoogte van uw wereld", "hoogte: ", 32, 5, 300, 1, &okHeight);
	if (okHeight)
		width =  QInputDialog::getInteger ( ui.widget, "Breedte van uw wereld", "breedte: ", 32, 5, 300, 1, &okWidth);
	if (okHeight && okWidth){
		viewer->Reset();
		ui.editor->Reset();
		world->resizeMap(height, width);
	}
}
//**************************************************************************************************


//**************************************************************************************************
// De tab zetten op raycasting of op editor
//**************************************************************************************************
void RaycastingQT::SetTab(int index)
{
	if (index == 0)
	{
		ui.tabWidget->setCurrentIndex(0);
	}
	else if (index == 1){
		ui.tabWidget->setCurrentIndex(1);
		ui.editor->DrawArrow();
		ui.dialAngle->setValue(270 - (int)viewer->viewAngle );
		ui.spinBoxX->setMaximum((world->WorldWidth()-1)*64 - 10);
		ui.spinBoxY->setMaximum((world->WorldHeight()-1)*64 - 10);
		ui.spinBoxX->setValue(viewer->x);
		ui.spinBoxY->setValue(viewer->y);
	}
}
//**************************************************************************************************


//**************************************************************************************************
// de tab op raycasting zetten
//**************************************************************************************************
void RaycastingQT::SetTabRaycasting()
{
	SetTab(0);
}
//**************************************************************************************************


//**************************************************************************************************
// de tab op editor zetten
//**************************************************************************************************
void RaycastingQT::SetTabEditor()
{
	SetTab(1);
}
//**************************************************************************************************


//**************************************************************************************************
// About venster laten zien
//**************************************************************************************************
void RaycastingQT::About()
{
	QMessageBox::about(this, "Over Raycasting", "Raycasting is een trimesteroverscheidend project\ngemaakt door:\nKenneth Devloo (0623746)\nNick Michiels (0623764)\nGroep 12\n2007-2008");
}
//**************************************************************************************************


//**************************************************************************************************
// GUI: signaal die hij krijgt als de angle van de viewer is gewijzigd, deze moet in de editor
// ook worden gewijzigd
//**************************************************************************************************
void RaycastingQT::AngleChanged(int value)
{
	viewer->viewAngle = 270.0 - (double)value;
	ui.editor->DrawArrow();
}
//**************************************************************************************************


//**************************************************************************************************
// Ook in de editor de waarde van x-coördinaat aanpassen
//**************************************************************************************************
void RaycastingQT::XChanged(int value)
{
	viewer->x = value;
	ui.editor->DrawArrow();
}
//**************************************************************************************************


//**************************************************************************************************
// Ook in de editor de waarde van t-coördinaat aanpassen
//**************************************************************************************************
void RaycastingQT::YChanged(int value)
{
	viewer->y = value;
	ui.editor->DrawArrow();
}
//**************************************************************************************************


//**************************************************************************************************
// Als er een texture wordt toegevoegd moet de editor deze texture ook toevoegen
//**************************************************************************************************
void RaycastingQT::TextureAdded(QString fileName)
{
	world->textures.add(world->textures.getSize(), fileName);
	texturesWindow->SetTextureMaxValue(world->textures.getSize()-1);
	texturesWindow->SetTextureValue(world->textures.getSize()-1);
}
//**************************************************************************************************


//**************************************************************************************************
// Als de viewer wordt veranderd moet ook de viewer in de editor worden veranderd
//**************************************************************************************************
void RaycastingQT::ViewerChanged()
{
	ui.spinBoxX->setValue(viewer->x);
	ui.spinBoxY->setValue(viewer->y);
}
//**************************************************************************************************


//**************************************************************************************************
// Als texture wordt gedelete moet dit ook in de editor gebeuren
//**************************************************************************************************
void RaycastingQT::TextureDeleted(int value)
{
	world->textures.del(value);
	texturesWindow->SetTextureMaxValue(world->textures.getSize()-1);
	texturesWindow->SetTextureValue(value);
	texturesWindow->on_spinBox_valueChanged(value);
}
//**************************************************************************************************


//**************************************************************************************************
// Texture is ge-edit dus moet de editor ook weten dat die veranderd is
//**************************************************************************************************
void RaycastingQT::TextureEdited(QString fileName, int value)
{
	world->textures.add(value, fileName);
	texturesWindow->on_spinBox_valueChanged(value);
}
//**************************************************************************************************


//**************************************************************************************************
// Als de spinbox van een grid is veranderd in de editor moet de viewer in de wereld ook worden aangepast
//**************************************************************************************************
void RaycastingQT::on_spinBoxTrigger_valueChanged(int value)
{
	if (selectedEditItem != NULL)
		(*world)[selectedEditItem->gridX][selectedEditItem->gridY].trigger = value;
}
//**************************************************************************************************


//**************************************************************************************************
// Trigger window opstarten
//**************************************************************************************************
void RaycastingQT::on_pushButtonTrigger_clicked()
{
	triggersWindow->show();
	triggersWindow->SetText();
}
//**************************************************************************************************