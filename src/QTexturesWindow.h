//***************************************************************************************************
// QTEXTURESWINDOW.H
// De klasse die aan de window hangt om textures aan te passen
//***************************************************************************************************
// AUTEUR: NICK
//***************************************************************************************************

#ifndef QTEXTURES_WINDOW_H
#define QTEXTURES_WINDOW_H 1

#include "ui_TexturesWindow.h"
#include "ui_raycastingqt.h"
#include <QtGui/QMainWindow>
#include <QWidget>
#include <QDialog>
#include <qobject.h>
#include <QImage>
#include <QFileDialog>
#include <QStringList>

#include "World.h"
#include "QEditTexture.h"
#include "EditQGraphicsItem.h"


class QTexturesWindow : public QDialog
{
	Q_OBJECT

public:
	QTexturesWindow(World* w);
	virtual ~QTexturesWindow();

	QEditTexture * selectedTextureEditItem;
	EditQGraphicsItem *selectedEditItem;
	void SetTextureValue(int value);
	void SetTextureMaxValue(int value);

	bool state; // geeft aan of er een texture is ge-edit of verwijderd, zo ja moet alles hertekend worden


	signals:
		void UpdateTexture(EditQGraphicsItem*, bool);

	
private:
	Ui::TexturesForm texturesWindow;
	QGraphicsPixmapItem *tex;
	QPixmap *pix;
	QGraphicsScene* scene;
	
	World *m_World;

	public slots:
		void on_spinBox_valueChanged(int val);

private slots:
	void on_pushButtonEditTexture_clicked();
	void on_pushButtonDeleteTexture_clicked();
	void on_pushButtonTexture_clicked();
	
	void accept();
	void reject();
signals:
	void TextureAdded(QString fileName);
	void TextureDeleted(int value);
	void TextureEdited(QString fileName, int value);
};

#endif 
