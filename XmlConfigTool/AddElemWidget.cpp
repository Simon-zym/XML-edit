#include "AddElemWidget.h"
//#include <QPushButton>

AddElemWidget::AddElemWidget(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);
	ui.btn_ok->setEnabled(false);
	connect(ui.btn_ok, &QPushButton::clicked, this, [this]() 
		{
			emit sig_addElem(ui.lineEdit_name->text(), ui.lineEdit_val->text());
			close();
		});
	connect(ui.btn_cancel, &QPushButton::clicked, this, &AddElemWidget::close);
	connect(ui.lineEdit_name, &QLineEdit::textChanged, this, [this]() 
		{
			if (ui.lineEdit_name->text().isEmpty() || ui.lineEdit_val->text().isEmpty())
			{
				ui.btn_ok->setEnabled(false);
			}
			else 
			{
				ui.btn_ok->setEnabled(true);
			}
		});
	connect(ui.lineEdit_val, &QLineEdit::textChanged, this, [this]()
		{
			if (ui.lineEdit_name->text().isEmpty() || ui.lineEdit_val->text().isEmpty())
			{
				ui.btn_ok->setEnabled(false);
			}
			else
			{
				ui.btn_ok->setEnabled(true);
			}
		});
	setWindowFlag(Qt::FramelessWindowHint);
}

AddElemWidget::~AddElemWidget()
{}
