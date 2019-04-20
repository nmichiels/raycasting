//***************************************************************************************************
// EDITOR.H
// Klasse die de editor beheert
//***************************************************************************************************
// AUTEUR: NICK
//***************************************************************************************************

#ifndef EDITOR_H
#define EDITOR_H 1

#include <QtGui/QMainWindow>
#include <QGraphicsView>
#include <QGraphicsPixmapItem>
#include <QGraphicsSceneMouseEvent>

#include <math.h>

#include "World.h"
#include "Viewer.h"
#include "EditQGraphicsItem.h"
#include "ArrowItem.h"

enum view{FLOOR, CEILING, WALL};

class Editor : public QGraphicsView
{
	Q_OBJECT
	public:	
		Editor(QWidget *parent=0);
		virtual ~Editor();

		void SetScale(double s){ m_Scale = s;}
		void SetWorld(World* w, Viewer* v);
		void SetView(view v);
		view CurrentView() const {return m_CurrentView;} // geeft aan wat we aan het wijzigen zijn
		void DrawSprite(QImage *image, int gridX, int gridY);
		void DrawWalkThrough(QImage *image, int gridX, int gridY);
		void Reset();
		void DrawArrow();

	protected:
		void keyPressEvent ( QKeyEvent * e );
		void keyReleaseEvent ( QKeyEvent * e );

	private:
		double m_Scale;		// schaal waarop de editor moet worden getekend
		World* m_World;		// pointer naar een wereld
		Viewer* m_Viewer;		// pointer naar een wereld
		QPixmap *pix;		// een pixmap die in een item van de editor wordt ingeladen

		QPixmap *arrow;		// pijl die de viewer aangeeft
		ArrowItem *arrowItem;

		bool draw;
		bool isCtrl;
		
		view m_CurrentView;		// enum die aangeeft welke view er nu uitgetekend is
		QGraphicsScene* m_View; // een scene die een plattegrond geeft met de vloertextures
		EditQGraphicsItem* p;

		EditQGraphicsItem * selectedEditItem;	// item dat geselecteerd is om te wijzigen

		cel* copy;	// bij het kopieren van een vakje worden de celgegevens hier in bewaard

		void FillPixmap(QPixmap* pix, int gridX, int gridY, view v); // vul een item met de juiste texture van een view
		void FillPixmapWall(QPixmap* pix, int gridX, int gridY);	// vul een item met de textures van de muren
		void ClearItem(EditQGraphicsItem * item);

	public slots:
		void UpdateTexture(EditQGraphicsItem* item, bool state);	// herteken de texture van een item
		void ClickItem(EditQGraphicsItem * item);		// Op deze item is gedrukt en wordt dus de huidige selectedEditItem
		void Copy();									// zorgt voor het kopieren van een item
		void Past();									// zorgt voor het plakken van een item
		void DropArrow(int posx, int posy, int gridX, int gridY);
		void Drop(EditQGraphicsItem *from, EditQGraphicsItem *to);

	signals:
		void SelectItem(EditQGraphicsItem*);		// geeft een signaal aan de hoofdklasse welk item geselecteerd wordt
		void RefreshEditTextures();					// Geeft een signaal aan de hoofdklasse dat bepaalde items moeten worden refreshed	
		void ViewerChanged();						// signaal naar hoofdprogramma die zegt dat viewer is aangepast
};
		


#endif