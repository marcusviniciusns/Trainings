#include <QtGui/QApplication>
#include <QTranslator>
#include <QLocale>
#include <QLibraryInfo>

#include "dialog.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

		// ==== a) traduzir strings da pr�pria Qt:

	// == a.1) primeiro m�todo:
	QTranslator qtTranslator;
//	qtTranslator.load("qt_" + QLocale::system().name(),
//		QLibraryInfo::location(QLibraryInfo::TranslationsPath));
//	a.installTranslator(&qtTranslator);


	// == a.2) segundo m�todo (usando um arquivo pr�prio):
	qtTranslator.load(":/translations/qt_" + QLocale::system().name());
	a.installTranslator(&qtTranslator);

	// ==== b) traduzir strings da aplica��o
	// (no c�digo, essas strings devem usar a est�tica QObject::tr):
	QTranslator myappTranslator;
	myappTranslator.load(":/translations/app_"+
											 QLocale::system().name());
	a.installTranslator(&myappTranslator);


	Dialog w;
    w.show();
    return a.exec();
}
