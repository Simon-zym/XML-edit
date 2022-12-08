#include "CharacterDataWatcher.h"
#include <QDomText>

CharacterDataWatcher::CharacterDataWatcher(QLineEdit *edit, const QDomElement &d)
	: QObject(edit),lineEdit(edit),elem(d)
{
	connect(lineEdit, &QLineEdit::textChanged, this, &CharacterDataWatcher::changed);
}

CharacterDataWatcher::~CharacterDataWatcher()
{}

void CharacterDataWatcher::changed() 
{
	QDomText t = elem.firstChild().toText();
	if (t.isNull()&&!lineEdit->text().isEmpty())
	{
		QDomDocument doc;
		QDomText t = doc.createTextNode(lineEdit->text());
		elem.appendChild(t);
	}
	t.setData(lineEdit->text());
}

