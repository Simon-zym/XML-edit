#include "MainWindow.h"
#include <QFile>
#include <QMessageBox>
#include <QFileDialog>
#include <QMenuBar>

MainWindow::MainWindow()
	: parser(new XmlParser)
{
	setCentralWidget(parser);
	createActions();
	//setWindowFlag(Qt::FramelessWindowHint);
	resize(600, 600);
}

MainWindow::~MainWindow()
{}

void MainWindow::createActions()
{
	QMenu *fileMenu = menuBar()->addMenu(tr("File"));
	label_curFile = new QLabel(this);
	label_curFile->setText("");
	statusBar()->addWidget(label_curFile);
	QAction *open_ac = new QAction(this);
	open_ac->setText("open");
	open_ac->setShortcut(QKeySequence::Open);
	fileMenu->addAction(open_ac);
	QAction *save_ac = new QAction(this);
	save_ac->setShortcut(QKeySequence::Save);
	save_ac->setText("save");
	fileMenu->addAction(save_ac);
	connect(save_ac, &QAction::triggered, this, &MainWindow::save);
	connect(open_ac, &QAction::triggered,this, [this]() 
		{
			
			curFile = QFileDialog::getOpenFileName(this,"XML",QDir::currentPath(),"*.xml");
			if (curFile.isEmpty())
			{
				return;
			}
			loadFile(curFile);
		});
}

void MainWindow::loadFile(const QString filePath) 
{
	QFile file(filePath);
	if (!file.open(QIODevice::ReadOnly|QFile::Text))
	{
		QMessageBox::warning(this, tr("XML parser"), tr("cannot open file:%1!").arg(filePath));
		return;
	}
	if (parser->read(&file)) 
	{
		label_curFile->setText(filePath);
	}
	file.close();

}

void MainWindow::open() 
{

}

bool MainWindow::save() 
{
	QFile wf(curFile);
	if (!wf.open(QIODevice::WriteOnly | QFile::Truncate)) 
	{
		QMessageBox::warning(this, tr("XML parser"), tr("cannot open file:%1!"));
		return false;
	}
	return parser->write(&wf);
	
}

bool MainWindow::saveAs() 
{
	return true;
}
