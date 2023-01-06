#pragma once

#include <QObject>
#include <QLineEdit>
#include <QDomElement>

class CharacterDataWatcher  : public QObject
{
	Q_OBJECT

public:
	CharacterDataWatcher(QWidget *edit, const QDomElement &data);
	~CharacterDataWatcher();

protected:
	QWidget* widgetEdit;
	QDomElement elem;

private slots:
	void changed();
};
