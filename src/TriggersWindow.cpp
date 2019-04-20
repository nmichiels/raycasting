#include "TriggersWindow.h"
#include <iostream>
using namespace std;

//** Constructor ***********************************************************************************
TriggersWindow::TriggersWindow(World *w, Triggers *triggers) : m_World(w), m_Triggers(triggers)
{
	triggersWindow.setupUi(this);
	connect(triggersWindow.saveButton, SIGNAL(clicked()), this, SLOT(saveButtonClicked()));
	connect(triggersWindow.saveBackUp, SIGNAL(clicked()), this, SLOT(backUpButtonClicked()));
	connect(triggersWindow.saveBackUp2, SIGNAL(clicked()), this, SLOT(backUpButton2Clicked()));
	connect(triggersWindow.okButton, SIGNAL(clicked()), this, SLOT(okButtonClicked()));
	connect(triggersWindow.cancelButton, SIGNAL(clicked()), this, SLOT(okButtonClicked()));
}
//**************************************************************************************************


//** Destructor ************************************************************************************
TriggersWindow::~TriggersWindow()
{}
//**************************************************************************************************


//**************************************************************************************************
// De tekst opvullen met de trigger informatie
//**************************************************************************************************
void TriggersWindow::SetText()
{
	QString inhoud;
	QString file = m_World->fileName + ".tri";
	if (File::loadSomething( file.toUtf8().data(), &inhoud)){
		triggersWindow.textTrigger->setText(inhoud);
	}
	else {
		cout << "trigger tekst is niet kunnen inladen" << endl;
	}
}
//**************************************************************************************************


//**************************************************************************************************
// Triggers laten wegschrijven in de triggerfile
//**************************************************************************************************
void TriggersWindow::saveButtonClicked()
{
	QString content = triggersWindow.textTrigger->toPlainText();
	QString file = m_World->fileName + ".tri";
	if (File::saveSomething( file.toUtf8().data(), content.toUtf8().data())){
		File::trigLoad(file.toUtf8().data(), m_Triggers);
		cout << "Save van trigger file gelukt";
	}
	else {
		cout << "trigger tekst is niet kunnnen saven" << endl;
	}
}
//**************************************************************************************************


//**************************************************************************************************
// Backup van de triggers laten wegschrijven in de back up file
//**************************************************************************************************
void TriggersWindow::backUpButtonClicked()
{
	QString content = triggersWindow.textTrigger->toPlainText();
	QString file = m_World->fileName + ".backup.tri";
	if (File::saveSomething( file.toUtf8().data(), content.toUtf8().data())){
		cout << "Save van trigger backup file gelukt";
	}
	else {
		cout << "trigger backup is niet kunnnen saven" << endl;
	}
}
//**************************************************************************************************


//**************************************************************************************************
// Backup file inladen in de tekst
//**************************************************************************************************
void TriggersWindow::backUpButton2Clicked()
{
	QString inhoud;
	QString file = m_World->fileName + ".backup.tri";
	if (File::loadSomething( file.toUtf8().data(), &inhoud)){
		triggersWindow.textTrigger->setText(inhoud);
	}
	else {
		cout << "trigger backup tekst is niet kunnen inladen" << endl;
		int ret = QMessageBox::warning(this, tr("Raycasting"),
                      tr("Er is geen geldige backup-file aanwezig.\n"),
                      QMessageBox::Ok);
	}
}
//**************************************************************************************************


//**************************************************************************************************
// Triggers worden geaccepteerd, toch nog eens checken of ze willen opslaan of niet
//**************************************************************************************************
void TriggersWindow::okButtonClicked()
{
	int ret = QMessageBox::warning(this, tr("Raycasting"),
                      tr("Het document is waarschijnlijk gewijzigd.\n"
                         "Wilt u uw wijzigingen opslaan?"),
                       QMessageBox::Cancel,
                      QMessageBox::Save);
	if (ret == QMessageBox::Save)
		saveButtonClicked();
}
//**************************************************************************************************

