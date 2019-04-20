//***************************************************************************************************
// RAYCASTINGQT.H
// Deze klasse koppelt alle klasse. Hij start de raycasting engine en zorgt vooral al voor een
// uitgebreide basis voor de editor en GUI
//***************************************************************************************************
// AUTEUR: NICK
// SUBAUTEUR: KENNETH
//***************************************************************************************************

#ifndef RAYCASTINGQT_H
#define RAYCASTINGQT_H

#include "ui_raycastingqt.h"
#include "ui_TexturesWindow.h"
#include <QtGui/QMainWindow>

#include <QKeyEvent>
#include <QMessageBox>
#include <QFileDialog>
#include <QInputDialog>


#include "Screen.h"
#include "Engine.h"
#include "Viewer.h"
#include "World.h"
#include "MiniMap.h"
#include "Triggers.h"
#include "QEditTexture.h"
#include "EditQGraphicsItem.h"
#include "File.h"
#include "Navigation.h"
#include "QTexturesWindow.h"
#include "TriggersWindow.h"

class RaycastingQT : public QMainWindow
{
	Q_OBJECT

	public:
		RaycastingQT(QWidget *parent = 0, Qt::WFlags flags = 0);
		~RaycastingQT();

	public slots:
		void move( QMouseEvent * event  );
		void SelectItem( EditQGraphicsItem*);
		void WallChanged(int state);
		void SpriteChanged(int state);
		void WalkThroughChanged(int state);
		void EditTexture(QEditTexture*);
		void RefreshEditTextures();
		void TextureAdded(QString fileName);

	protected:
		void keyPressEvent ( QKeyEvent * e );
		void keyReleaseEvent ( QKeyEvent * e );

	private:

		ULONG lastTick,currTick;
		Ui::RaycastingQTClass ui;
		long nr;
		Viewer *viewer;
		World *world;
		Engine *engine;
		//Textures* textures;
		Triggers triggers;
		Navigation *navigatie;

		// Items die de textures weergeven om te editten
		QTexturesWindow *texturesWindow;
		QEditTexture* textureFloor;
		QEditTexture* textureCeiling;
		QEditTexture* textureWallNorth;
		QEditTexture* textureWallEast;
		QEditTexture* textureWallSouth;
		QEditTexture* textureWallWest;
		QEditTexture* textureSprite;
		QGraphicsScene* textureFloorScene;
		QGraphicsScene* textureCeilingScene;
		QGraphicsScene* textureWallNorthScene;
		QGraphicsScene* textureWallEastScene;
		QGraphicsScene* textureWallSouthScene;
		QGraphicsScene* textureWallWestScene;
		QGraphicsScene* textureSpriteScene;

		TriggersWindow *triggersWindow;

		// kompass
		QGraphicsScene* compassScene;

		// geselecteerde item om aan te passen
		EditQGraphicsItem * selectedEditItem;
		
		void InitializeEditor(void);
		void StartNewWorld(int height, int width);
		
	private slots:
	void on_pushButtonTrigger_clicked();
	void on_spinBoxTrigger_valueChanged(int value);
		void on_pushButtonWall_clicked();
		void on_pushButtonPlafond_clicked();
		void on_pushButtonFloor_clicked();
		void on_zoomEditor_valueChanged(int zoom);
		void on_MiniMapPlus_clicked();
		void on_MiniMapMin_clicked();
		void ScreenUpdate();
		void Quit(void);
		void Save(void);
		void Load(void);
		void New(void);
		void Resize(void);
		void About(void);
		void SetTab(int index);
		void SetTabRaycasting();
		void SetTabEditor();
		void AngleChanged(int value);
		void XChanged(int value);
		void YChanged(int value);
		void ViewerChanged();		// viewer is ergens anders aangepast en de items moeten worden upgedate
		void TextureDeleted(int value);
		void TextureEdited(QString fileName, int value);
};

#endif // RAYCASTINGQT_H
