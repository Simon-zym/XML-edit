#include "XmlParser.h"
#include <QFile>
#include <QMessageBox>
#include <QStack>
#include <QTextStream>
#include <QLabel>
#include "CharacterDataWatcher.h"
#include "AddElemWidget.h"
#include <QAction>
#include <QSpinBox>
#include "QDoubleSpinBox"
#include <QCheckBox>
#include <QComboBox>
enum  EDITTYPE
{
	STR = 0,  NUM = 1, NUMD = 2, CHECK = 3, COMBOBOX = 4
};
XmlParser::XmlParser(QWidget *parent):QWidget(parent)
{
	hLayout = new QHBoxLayout(this);
	hLayout->setSpacing(0);
	layout_edit = new QVBoxLayout();
	layout_edit->setContentsMargins(0, 0, 0, 0);
	layout_rBottom = new QVBoxLayout();
	label_comment = new QLabel(this);
	//layout_rBottom->addWidget(label_comment);
	label_comment->setMinimumHeight(80);
	label_comment->setAlignment(Qt::AlignLeft | Qt::AlignTop | Qt::AlignJustify);
	label_comment->setContentsMargins(0,10,0,10);
	label_comment->setWordWrap(true);
	label_comment->setIndent(-1);

	layout_operate = new QVBoxLayout();
	btn_reload = new QPushButton(this);
	btn_addElem = new QPushButton(this);
	btn_reload->setText(tr("更新"));
	btn_addElem->setText(tr("添加元素"));
	layout_btn = new QHBoxLayout();
	layout_btn->addWidget(btn_reload);
	layout_btn->addWidget(btn_addElem);
	//layout_operate->addStretch();
	//layout_operate->addLayout(layout_btn);
	layout_rBottom->addLayout(layout_btn);
	layout_rBottom->addWidget(label_comment);

	stackedWidget = new QStackedWidget(this);
	listWidget = new QListWidget(this);
	layout_edit->addWidget(stackedWidget);
	layout_edit->addLayout(layout_rBottom);
	hLayout->addWidget(listWidget);
	hLayout->addLayout(layout_edit);
	QAction *t = new QAction(this);
	t->setText("gggg");
	listWidget->addAction(t);

	hLayout->setContentsMargins(0, 0, 0, 0);
	connect(listWidget, &QListWidget::currentRowChanged, stackedWidget, &QStackedWidget::setCurrentIndex);
	connect(listWidget, &QListWidget::currentRowChanged, this, &XmlParser::slot_showComment);
	connect(btn_addElem, &QPushButton::clicked, this, [this]() 
		{
			if (!doc.isNull())
			{
				AddElemWidget elemDlg;
				connect(&elemDlg, &AddElemWidget::sig_addElem2, this, &XmlParser::slot_addElem);
				connect(&elemDlg, &AddElemWidget::sig_addQCombo, this, &XmlParser::slot_addCombo);
				elemDlg.exec();
			}	
		});
	connect(btn_reload, &QPushButton::clicked, this, [this]() 
		{
			if (!fileName.isEmpty())
			{
				QFile file(fileName);
				file.open(QIODevice::ReadOnly | QIODevice::Text);
				read(&file);
			}
			
		});
	
	setLayout(hLayout);
}

XmlParser::~XmlParser() {}

void XmlParser::slot_showComment(int index) 
{
	if (commentMap.contains(index))
	{
		label_comment->setText(commentMap[index]);
	}
	else 
	{
		label_comment->clear();
	}
}

XmlEdit::XmlEdit(QWidget*parent):QScrollArea(parent)
{
	setWidget(new QWidget());
	layoutAll = new QVBoxLayout(widget());
	layoutTop = new QVBoxLayout();
	layoutTop->setSpacing(0);
	layoutAll->addLayout(layoutTop);
	layoutAll->addStretch();
	widget()->setLayout(layoutAll);
	layoutAll->setContentsMargins(0, 0, 0, 0);
	setWidgetResizable(true);
}
XmlEdit::~XmlEdit() {}

#include <QDebug>
QLayout* XmlParser::generateWidgets(int type,QDomNode node) 
{
	QHBoxLayout *layout = new QHBoxLayout();
	QLabel *label = new QLabel((static_cast<XmlEdit*>(stackedWidget->currentWidget()))->widget());
	label->setText(node.nodeName());
	layout->addWidget(label);
	
	
	switch (type)
	{
	case STR:
	{
		QLineEdit *edit = new QLineEdit((static_cast<XmlEdit*>(stackedWidget->currentWidget()))->widget());
		edit->setText(node.toElement().text());
		new CharacterDataWatcher(edit, node.toElement());
		layout->addWidget(edit);
		break;
	}
	case NUM:
	{
		QSpinBox *spinBox = new QSpinBox((static_cast<XmlEdit*>(stackedWidget->currentWidget()))->widget());
		
		
		new CharacterDataWatcher(spinBox, node.toElement());
		QDomNamedNodeMap ma = node.attributes();
		if (ma.namedItem("max").isNull())
		{
			
			spinBox->setMaximum(INTMAX_MAX);
			
		}
		else 
		{
			spinBox->setMaximum(ma.namedItem("max").toAttr().value().toInt());
		}

		if (ma.namedItem("min").isNull())
		{
			spinBox->setMinimum(INT_MIN);
			
		} 
		else
		{
			spinBox->setMinimum(ma.namedItem("min").toAttr().value().toInt());
		}
		spinBox->setValue(node.toElement().text().toInt());
		layout->addWidget(spinBox);
		break;
	}
	case NUMD:
	{
		QDoubleSpinBox *dSpinBox = new QDoubleSpinBox((static_cast<XmlEdit*>(stackedWidget->currentWidget()))->widget());
		
		new CharacterDataWatcher(dSpinBox, node.toElement());
		QDomNamedNodeMap ma = node.attributes();
		if (ma.namedItem("max").isNull())
		{
			dSpinBox->setMaximum(INT_MAX);
		}
		else
		{
			dSpinBox->setMaximum(ma.namedItem("max").toAttr().value().toDouble());
			
		}

		if (ma.namedItem("min").isNull())
		{
			dSpinBox->setMinimum(INT_MIN);
		}
		else
		{
			dSpinBox->setMinimum(ma.namedItem("min").toAttr().value().toDouble());
			
		}
		dSpinBox->setValue(node.toElement().text().toDouble());
		dSpinBox->setSingleStep(0.01);
		layout->addWidget(dSpinBox);
		break;
	}
	case CHECK:
	{
		QCheckBox *checkBox = new QCheckBox((static_cast<XmlEdit*>(stackedWidget->currentWidget()))->widget());
		checkBox->setChecked(node.toElement().text().toInt());
		new CharacterDataWatcher(checkBox, node.toElement());
		layout->addWidget(checkBox);
		break;
	}
	case COMBOBOX:
	{
		QComboBox *comboBox = new QComboBox((static_cast<XmlEdit*>(stackedWidget->currentWidget()))->widget());
		QStringList strs;
		QDomNode n = node.firstChild();
		while (!n.isNull())
		{
			comboBox->addItem(n.toElement().text());
			
			n = n.nextSibling();
			
		}
		
		QDomNamedNodeMap ma = node.attributes();
		QDomNode cur = ma.namedItem("value");

		//qDebug() << strs.value(0).toStdU16String();
		
		comboBox->setCurrentIndex(cur.toAttr().value().toInt());
		new CharacterDataWatcher(comboBox, node.toElement());
		layout->addWidget(comboBox);
		break;
	}
	default:
		break;
	}
	QPushButton *btn_send = new QPushButton((static_cast<XmlEdit*>(stackedWidget->currentWidget()))->widget());
	btn_send->setText(tr("send"));
	
	
	layout->addWidget(btn_send);
	return layout;
}

void XmlParser::parseXml() 
{
	QFile xml("text.xml");
	if (!xml.open(QIODevice::ReadOnly))
	{
		return;
	}
	QByteArray byteStream;
	byteStream = xml.readAll();
	QDomDocument doc;
	doc.setContent(byteStream);
	QDomElement docElem = doc.documentElement();
	
	QDomNode node = docElem.firstChild();

	while(!node.isNull())
	{	
		
		QDomNode n1 = node.firstChild();
		
		while (!n1.isNull())
		{
			QDomElement e = n1.toElement();
			e.attribute("type");
			n1 = n1.nextSibling();
		}
		node = node.nextSibling();
	}
}
#include <iostream>
bool XmlParser::addNode(QDomNode node, int dep) 
{
	if (dep>1) 
	{
		QMessageBox::information(window(), "parser error", "xml format error,max allow 3 level node!");
		return false;
	}

	if (dep == 0)
	{
		nodeVec.emplace_back(node);
		listWidget->addItem(node.nodeName());
		//listWidget->item(listWidget->count() - 1)->setFlags(Qt::ItemIsEnabled | Qt::ItemIsEditable);
		XmlEdit *t = new XmlEdit();
		stackedWidget->addWidget(t);
		stackedWidget->setCurrentWidget(t);
		listWidget->setCurrentRow(listWidget->count()-1);
	}else if (dep == 1)
	{
		int type = getType(node);
		QLayout *layout =generateWidgets(type, node);
		QVBoxLayout *lay = (static_cast<XmlEdit*>(stackedWidget->currentWidget()))->layoutTop;
		lay->addLayout(layout);	
		/*new CharacterDataWatcher(edit, node.toElement());
		connect(btn_send, &QPushButton::clicked, this, [=]()
			{
				emit sig_send(label->text(),edit->text());
			});*/
	}
	return true;
}

bool XmlParser::write(QIODevice *device) const
{
	const int indent = 4;
	QTextStream out(device);
	doc.save(out, indent);
	return true;
}

bool XmlParser::read(QIODevice *device) 
{
	QString errStr;
	int errLine;
	int errCol;
	clear();
	if (!doc.setContent(device, &errStr, &errLine, &errCol)) 
	{
		QMessageBox::information(window(), tr("XML parser"), tr("parse error at line %1,column %2:\n%3").arg(errLine).arg(errCol).arg(errStr));
		return false;
	}
	fileName = dynamic_cast<QFile*>(device)->fileName();
	QDomElement elem = doc.documentElement();
	QStack<QDomNode> stack;
	QDomNode current = elem.firstChild();
	while (1)
	{
		if (!current.isNull())
		{
			
			
			//QDomElement childElem = node.do
			if (current.isComment())
			{
				if (stack.count() < 1)
				{
				
					commentMap[listWidget->count()] = current.toComment().data();
				}
				current = current.nextSibling(); 
				continue;
			}
			if (!addNode(current, stack.count())) return false;
			if (stack.count() < 1) 
			{
				stack.push(current);
				current = current.firstChild();
			}
			else 
			{
				current = current.nextSibling();
			}
			
		}else if (!stack.count())
		{
			break;
		}
		else 
		{
			current = stack.pop().nextSibling();
		}
	}
	return true;
}

void XmlParser::clear() 
{
	fileName.clear();
	doc.clear();
	nodeVec.clear();
	commentMap.clear();
	if (listWidget != Q_NULLPTR)
	{
		listWidget->clear();

	}

	if (stackedWidget != Q_NULLPTR)
	{
		int count = stackedWidget->count();
		for (int i = 0; i < count; i++)
		{
			QWidget *t = stackedWidget->widget(count-i-1);
			stackedWidget->removeWidget(t);
			t->deleteLater();
		}
	}
}

void XmlParser::slot_reload() {}
#include <QDebug>
void XmlParser::slot_addElem(int type,QMap<QString,QString> data) 
{
	QDomNode node = nodeVec[listWidget->currentRow()];
	if (!node.isNull())
	{
		QString nodeName = data["name"];
		QDomElement elem = doc.createElement(nodeName);
		QDomText text = doc.createTextNode(data["value"]);
		elem.appendChild(text);
		
		
		switch (type)
		{
		case STR:
		{
			elem.setAttribute("type","STR");
			break;
		}
		case NUM:
		{
			elem.setAttribute("type","NUM");
			if (data.contains("min"))
			{
				elem.setAttribute("min", data.value("min"));
			}
			if (data.contains("max"))
			{
				elem.setAttribute("max", data.value("max"));
			}
			break;
		}
		case NUMD:
		{
			elem.setAttribute("type", "NUMD");
			if (data.contains("min"))
			{
				elem.setAttribute("min", data.value("min"));
			}
			if (data.contains("max"))
			{
				elem.setAttribute("max", data.value("max"));
			}
			break;
		}
		case CHECK:
		{
			elem.setAttribute("type", "CHECK");
			break;
		}
		}
		node.appendChild(elem);
		addNode(elem, 1);
	}
}

void XmlParser::slot_addCombo(QString name,const QStringList list)
{
	QDomNode node = nodeVec[listWidget->currentRow()];
	if (!node.isNull())
	{
		QDomElement elem = doc.createElement(name);
		elem.setAttribute("type", "COMBOBOX");
		elem.setAttribute("value", "0");
		for (auto str : list)
		{
			QDomNode n = doc.createElement(str);
			QDomText text = doc.createTextNode(str);
			n.appendChild(text);
			elem.appendChild(n);
		}
		node.appendChild(elem);
		addNode(elem, 1);
	}
}

int XmlParser::getType(QDomNode node) 
{
	if (node.isNull())
	{
		return -1;
	}
	else if (!node.hasAttributes()) 
	{
		return STR;
	}
	else 
	{
		QDomNamedNodeMap attrMap =node.attributes();
		QString typeName = attrMap.namedItem("type").toAttr().value();

		if (typeName == "str")
		{
			return STR;
		}
		else if (typeName == "CHECK") 
		{
			return CHECK;
		}
		else if (typeName == "NUM") 
		{
			return NUM;
		}
		else if (typeName == "NUMD") 
		{
			return NUMD;
		}
		else if(typeName == "COMBOBOX"){
			return COMBOBOX;
		}
		else 
		{
			return STR;
		}
	}
}
