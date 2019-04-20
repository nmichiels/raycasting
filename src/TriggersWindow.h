//***************************************************************************************************
// TRIGGERSWINDOW.H
// De window uit de GUI die een trigger-file inlaadt en kan laten aanpassen
//***************************************************************************************************
// AUTEUR: NICK
//***************************************************************************************************

#ifndef TRIGGERS_WINDOW_H
#define TRIGGERS_WINDOW_H 1

#include "ui_TriggersWindow.h"
#include "ui_raycastingqt.h"
#include <QtGui/QMainWindow>
#include <QWidget>
#include <QDialog>
#include <qobject.h>
#include <QImage>
#include <QFileDialog>
#include <QStringList>
#include <QMessageBox>

#include "World.h"
#include "File.h"
#include "Triggers.h"


class TriggersWindow : public QDialog
{
	Q_OBJECT

	public:
		TriggersWindow(World* w, Triggers* triggers);
		virtual ~TriggersWindow();

		void SetText();

	private:
		Ui_Dialog triggersWindow;

		
		World *m_World;
		Triggers *m_Triggers;

	private slots:
		void saveButtonClicked();
		void backUpButtonClicked();
		void backUpButton2Clicked();
		void okButtonClicked();

};

#endif 
