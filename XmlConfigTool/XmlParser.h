#pragma once
#include <QDomDocument>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QIODevice>
#include <QWidget>
#include <QListWidget>
#include <QStackedWidget>
#include <QScrollArea>
#include <QPushButton>
#include <vector>

/************************************************************************/
/*xml 格式
	<root>
		<tab1>
			<groupbox1 type="groupbox">
				<lineedit1 type="QLineEdit">1</lineedit1>
				<lineedit2 type="QLineEdit"></lineedit2>
			</groupbox1>
			<groupbox2>
				<param0 type="QComboBox">1<param0>
				<param1 type="QLineEdit">10.2.0.1</param1>
			</groupbox2>
		</tab1>
		<tab2>
			<omega0 type="groupbox">
				<param1 type="QLineEdit">1</param1>
				<param2 type="QLineEdit">2</param2>
			</omega0>
			<omega2>
				<param3 type="QComboBox">1<param3>
				<param4 type="QLineEdit">10.2.0.1</param4>
			</omega2>
		</tab2>
	</root>
*/
/************************************************************************/
#include <QMap>
#include <QLabel>
class XmlEdit:public QScrollArea
{
public:
	XmlEdit(QWidget*parent = nullptr);
	virtual ~XmlEdit();
	QVBoxLayout *layoutTop;
	
protected:
private:
	
	QVBoxLayout *layoutAll;
	QHBoxLayout *layoutBottom;

	
};

class XmlParser:public QWidget
{
	Q_OBJECT
public:
	explicit XmlParser(QWidget *parent=nullptr);
	~XmlParser();
	void parseXml();
	QLayout* generateWidgets(int type, QDomNode node);
private slots:
	void slot_showComment(int );
	void slot_reload();
	void slot_addElem(int, const QMap<QString,QString>);
	void slot_addCombo(QString, const QStringList);
protected:
	QDomDocument doc;
	std::vector<QDomNode> nodeVec;
	QString fileName;
	QHBoxLayout *hLayout;
	QVBoxLayout *layout_edit;
	QStackedWidget *stackedWidget = nullptr;
	QListWidget *listWidget = nullptr;
	QMap<int, QString> commentMap;
	QVBoxLayout *layout_rBottom;
	QVBoxLayout *layout_operate;
	QHBoxLayout *layout_btn;
	QPushButton *btn_reload;
	QPushButton *btn_addElem;
	QLabel *label_comment;
	
	bool addNode(QDomNode node, int dep);
	void clear();
	int getType(QDomNode);
	
public:
	bool write(QIODevice *device)const;
	bool read(QIODevice *device);

signals:
	void sig_send(QString, QVariant);

	
};

