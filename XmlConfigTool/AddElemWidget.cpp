#include "AddElemWidget.h"
#include <QStackedWidget>
//#include <QPushButton>

AddElemWidget::AddElemWidget(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);
	ui.btn_ok->setEnabled(false);
	connect(ui.comboBox_type, qOverload<int>(&QComboBox::currentIndexChanged), this, &AddElemWidget::slot_enableBtn);
	connect(ui.btn_ok, &QPushButton::clicked, this, &AddElemWidget::slot_addData);
	connect(ui.btn_cancel, &QPushButton::clicked, this, &AddElemWidget::close);
	connect(ui.btn_add, &QPushButton::clicked, this, &AddElemWidget::slot_addComItem);
	connect(ui.btn_del, &QPushButton::clicked, this, &AddElemWidget::slot_delComItem);
	/*connect(ui.lineEdit_strV, &QLineEdit::textChanged, this, [this]()
		{
			if (ui.lineEdit_name->text().isEmpty() || ui.lineEdit_strV->text().isEmpty())
			{
				ui.btn_ok->setEnabled(false);
			}
			else
			{
				ui.btn_ok->setEnabled(true);
			}
		});
	connect(ui.lineEdit_NumV, &QLineEdit::textChanged, this, [this]()
		{
			if (ui.lineEdit_name->text().isEmpty() || ui.lineEdit_NumV->text().isEmpty())
			{
				ui.btn_ok->setEnabled(false);
			}
			else
			{
				ui.btn_ok->setEnabled(true);
			}
		});
	connect(ui.lineEdit_NumDV, &QLineEdit::textChanged, this, [this]()
		{
			if (ui.lineEdit_name->text().isEmpty() || ui.lineEdit_NumV->text().isEmpty())
			{
				ui.btn_ok->setEnabled(false);
			}
			else
			{
				ui.btn_ok->setEnabled(true);
			}
		});*/
	connectStr();
	
	model = new QStringListModel(this);
	ui.listView->setModel(model);
	setWindowFlag(Qt::FramelessWindowHint);
}

AddElemWidget::~AddElemWidget()
{}

void AddElemWidget::slot_addData() 
{
	switch (ui.comboBox_type->currentIndex())
	{
	case 0:
	{
		QMap<QString, QString> data;
		data.insert("name", ui.lineEdit_name->text());
		data.insert("value", ui.lineEdit_strV->text());
		emit sig_addElem2(0, data);
		break; 
	}
	case 1:
	{
		QMap<QString, QString> data;
		data.insert("name", ui.lineEdit_name->text());
		data.insert("value",ui.lineEdit_NumV->text());
		if (!ui.lineEdit_NumMin->text().isEmpty())
		{
			data.insert("min", ui.lineEdit_NumMin->text());
		}
		if (!ui.lineEdit_NumMax->text().isEmpty())
		{
			data.insert("max", ui.lineEdit_NumMax->text());
		}
		
		emit sig_addElem2(1, data);
		break; 
	}
	case 2:
	{
		QMap<QString, QString> data;
		data.insert("name", ui.lineEdit_name->text());
		data.insert("value", ui.lineEdit_NumDV->text());
		if (!ui.lineEdit_NumDMin->text().isEmpty())
		{
			data.insert("min", ui.lineEdit_NumDMin->text());
		}
		if (!ui.lineEdit_NumDMax->text().isEmpty())
		{
			data.insert("max", ui.lineEdit_NumDMax->text());
		}
		emit sig_addElem2(2, data);
		break;
	};
	case 3:
	{
		QMap<QString, QString> data;
		data.insert("name", ui.lineEdit_name->text());
		if (ui.checkBox_default->isChecked()) 
		{
			data.insert("value", "1");
		}
		else 
		{
			data.insert("value", "0");
		}
		emit sig_addElem2(3, data);
		break;
	};
	case 4:
	{
		emit sig_addQCombo(ui.lineEdit_name->text(),model->stringList());
		break;
	};
	default:
		break;
	}
	close();
}

void AddElemWidget::slot_addComItem() 
{
	model->insertRow(model->rowCount());
	ui.listView->setCurrentIndex(model->index(model->rowCount()-1));
	//ui.listView->select
}

void AddElemWidget::slot_delComItem() 
{
	if (ui.listView->currentIndex().isValid())
	{
		model->removeRow(ui.listView->currentIndex().row());
	}
}

void AddElemWidget::slot_enableBtn(int type) 
{
	ui.stackedWidget->setCurrentIndex(type);
	/*switch (type)
	{
	case 0:
	{
		if (ui.lineEdit_name->text().isEmpty()||ui.lineEdit_strV->text().isEmpty())
		{
			ui.btn_ok->setEnabled(false);
		}
		else {
			ui.btn_ok->setEnabled(true);
		}
		
		disconnect(ui.lineEdit_name, SIGNAL(textChanged()));
		connectStr();
		break;
	}
	case 1:
	{
		if (ui.lineEdit_name->text().isEmpty()||ui.lineEdit_NumV->text().isEmpty())
		{
			ui.btn_ok->setEnabled(false);
		}
		else 
		{
			ui.btn_ok->setEnabled(true);
		}
		disconnect(ui.lineEdit_name, SIGNAL(textChanged());
		disconnect(ui.lineEdit_strV);
	}
	case 2:
	{

	}
	case 3:
	{

	}
	case 4:
	{
	
	}

	}*/
}

void AddElemWidget::connectStr() 
{
	connect(ui.lineEdit_name, &QLineEdit::textChanged, this, [this]()
		{
			if (ui.lineEdit_name->text().isEmpty())
			{
				ui.btn_ok->setEnabled(false);
			}
			else
			{
				ui.btn_ok->setEnabled(true);
			}
		});
	/*connect(ui.lineEdit_strV, &QLineEdit::textChanged, this, [this]()
		{
			if (ui.lineEdit_name->text().isEmpty() || ui.lineEdit_strV->text().isEmpty())
			{
				ui.btn_ok->setEnabled(false);
			}
			else
			{
				ui.btn_ok->setEnabled(true);
			}
		});*/
}
