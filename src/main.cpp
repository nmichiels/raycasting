#include <QtGui/QApplication>
#include "raycastingqt.h"

//***************************************************************************************************************
// Hoofdfunctie die alles laat starten
//***************************************************************************************************************
int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	RaycastingQT w;
	w.show();
	a.connect(&a, SIGNAL(lastWindowClosed()), &w, SLOT(Quit()));
	return a.exec();
}
