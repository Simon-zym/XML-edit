#pragma once

#include <QMainWindow>
#include "ui_MainWindow.h"
#include "XmlParser.h"


QT_BEGIN_NAMESPACE
class QAction;
class QMenu;

QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	explicit MainWindow();
	~MainWindow();
	void loadFile(const QString file);

protected:

private:
	Ui::MainWindowClass ui;

	void createActions();
	QString curFile;
	XmlParser *parser;
	QLabel *label_curFile;
private slots:
	void open();
	bool save();
	bool saveAs();

	
};
