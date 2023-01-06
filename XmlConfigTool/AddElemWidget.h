#pragma once

#include <QDialog>
#include "ui_AddElemWidget.h"
#include <QMap>
#include <QStringListModel>

class AddElemWidget : public QDialog
{
	Q_OBJECT

public:
	AddElemWidget(QWidget *parent = nullptr);
	~AddElemWidget();
	
private slots:

	void slot_addData();
	void slot_addComItem();
	void slot_delComItem();
	void slot_enableBtn(int);

signals:
	void sig_addElem(QString, QString);
	void sig_addElem1(QWidget*);
	void sig_addElem2(int type,const QMap<QString,QString> map);
	void sig_addQCombo(QString, const QStringList);

private:
	Ui::AddElemWidgetClass ui;
	QStringListModel *model;

	void connectStr();
};
