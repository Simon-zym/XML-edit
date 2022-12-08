#include <iostream>
#include <QApplication>
#include "MainWindow.h"
#include <QFile>
#include <QTextStream>
#include <QMessageBox>

int main(int argc, char* argv[]) 
{
	QFile file("test1.xml");
	file.open(QIODevice::ReadOnly | QIODevice::Text);
	QDomDocument doc;
	doc.setContent(&file);
	QDomNode root = doc.documentElement();
	bool isentity = root.isEntity();
	QDomNode t1 = root.firstChild();
	bool hasch = root.hasChildNodes();
	isentity = t1.isEntity();
	hasch = t1.hasChildNodes();
	QDomNode t11 = t1.firstChild();
	isentity = t11.isEntity();
	hasch = t11.hasChildNodes();
	QString nodeval = t11.toElement().text();
	bool iselem = root.isElement();
	QApplication a(argc, argv);
	QFile qssFile("m.qss");

	if (qssFile.open(QFile::ReadOnly))
	{
		QString qssStr = QLatin1String(qssFile.readAll());

		a.setStyleSheet(qssStr);
		//qDebug() << qssStr;
		qssFile.close();
	}
	
	MainWindow w;
	w.show();
	return a.exec();
}