#pragma once

#include <QObject>
#include <QLineEdit>
#include <QDomElement>

class CharacterDataWatcher  : public QObject
{
	Q_OBJECT

public:
	CharacterDataWatcher(QLineEdit *edit, const QDomElement &data);
	~CharacterDataWatcher();

protected:
	QLineEdit* lineEdit;
	QDomElement elem;

private slots:
	void changed();
};
