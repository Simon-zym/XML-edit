#pragma once

#include <QDialog>
#include "ui_AddElemWidget.h"

class AddElemWidget : public QDialog
{
	Q_OBJECT

public:
	AddElemWidget(QWidget *parent = nullptr);
	~AddElemWidget();
signals:
	void sig_addElem(QString, QString);

private:
	Ui::AddElemWidgetClass ui;
};
