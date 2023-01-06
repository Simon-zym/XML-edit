#include "CharacterDataWatcher.h"
#include <QDomText>
#include <QCheckBox>
#include <QComboBox>
#include <QSpinBox>
#include <QDoubleSpinBox>

CharacterDataWatcher::CharacterDataWatcher(QWidget *edit, const QDomElement &d)
	: QObject(edit), widgetEdit(edit),elem(d)
{
	QString obName = widgetEdit->metaObject()->className();
	if (obName=="QLineEdit")
	{
		connect(dynamic_cast<QLineEdit*>(widgetEdit), &QLineEdit::textChanged, this, &CharacterDataWatcher::changed);
	}else if (obName == "QCheckBox") 
	{
		connect(dynamic_cast<QCheckBox*>(widgetEdit), &QCheckBox::stateChanged, this, &CharacterDataWatcher::changed);
	}
	else if (obName == "QSpinBox") 
	{
		connect(dynamic_cast<QSpinBox*>(widgetEdit), qOverload<int>(&QSpinBox::valueChanged), this, &CharacterDataWatcher::changed);
	}
	else if (obName == "QDoubleSpinBox") 
	{
		connect(dynamic_cast<QDoubleSpinBox*>(widgetEdit), qOverload<double>(&QDoubleSpinBox::valueChanged), this, &CharacterDataWatcher::changed);
	}
	else if (obName == "QComboBox") 
	{
		connect(dynamic_cast<QComboBox*>(widgetEdit), qOverload<int>(&QComboBox::currentIndexChanged), this, &CharacterDataWatcher::changed);
	}
	
}

CharacterDataWatcher::~CharacterDataWatcher()
{}

void CharacterDataWatcher::changed() 
{
	QString obName = widgetEdit->metaObject()->className();
	if (obName == "QLineEdit")
	{
		QDomText t = elem.firstChild().toText();
		QLineEdit *edit = dynamic_cast<QLineEdit*>(widgetEdit);
		if (t.isNull() && !edit->text().isEmpty())
		{
			QDomDocument doc;
			QDomText t = doc.createTextNode(edit->text());
			elem.appendChild(t);
		}
		t.setData(edit->text());
	}
	else if (obName == "QCheckBox")
	{
		QCheckBox *checkBox = dynamic_cast<QCheckBox*>(widgetEdit);
		QDomText t = elem.firstChild().toText();
		if(checkBox->isChecked())
		{
			t.setData("1");
		}
		else {
			t.setData("0");
		}
	}
	else if (obName == "QSpinBox")
	{
		QSpinBox *spinBox = dynamic_cast<QSpinBox*>(widgetEdit);
		QDomText t = elem.firstChild().toText();
		t.setData(QString::number(spinBox->value()));
	}
	else if (obName == "QDoubleSpinBox")
	{
		QDoubleSpinBox *dSpinBox = dynamic_cast<QDoubleSpinBox*>(widgetEdit);
		QDomText t = elem.firstChild().toText();
		t.setData(QString::number(dSpinBox->value()));
	}
	else if (obName == "QComboBox")
	{
		QComboBox *comboBox = dynamic_cast<QComboBox*>(widgetEdit);
		elem.setAttribute("value", comboBox->currentIndex());
	}
	
}

