#pragma execution_character_set("utf-8")  // ������ı�������
#include "qtguiapplication1.h"
#include <QFileDialog>

QtGuiApplication1::QtGuiApplication1(QWidget *parent)
	: QMainWindow(parent)
{
	// װ��UI
	ui.setupUi(this);
	this->resize(1200, 840);
	this->setWindowTitle("JCodeCounter");

	QIcon* qicon = new QIcon(".\\Resources\\cc_icon.png");
	this->setWindowIcon(*qicon);

	// ������ֵ
	QString ig[6] = { tr(".git"), tr(".vscode"), tr("dist"), tr("node_modules"), tr(".vs"), tr("venv") };
	ignoreList.assign(ig, ig + 6);

	QString hv[4] = { tr("file"), tr("code"), tr("comment"), tr("blank") };
	headerV.assign(hv, hv + 4);

	// ��ȡС����
	langSelectLayout = this->findChild<QLayout*>("langSelectLayout");
	pathSelectLayout = this->findChild<QLayout*>("pathSelectLayout");
	startLayout = this->findChild<QLayout*>("startLayout");

	selectAllCkBox = this->findChild<QCheckBox*>("selectAllCkBox");
	selectPathLine = this->findChild<QLineEdit*>("selectPathLine");
	selectPathBtn = this->findChild<QPushButton*>("selectPathBtn");

	ignoreSysCkBox = this->findChild<QCheckBox*>("ignoreSysCkBox");
	startBtn = this->findChild<QPushButton*>("startBtn");

	resultTable = this->findChild<QTableWidget*>("resultTable");
	logTextArea = this->findChild<QTextEdit*>("logTextArea");

	selectPathAction = this->findChild<QAction*>("selectPathAction");
	exitAction = this->findChild<QAction*>("exitAction");

	// �����������ѡ��
	for (int i = 0; i < lang.supportLang().size(); ++i)
	{
		QCheckBox* cb = new QCheckBox(tr(lang.supportLang()[i].c_str()), this);
		connect(cb, &QCheckBox::stateChanged, this, &QtGuiApplication1::onSelectLangStateChanged);
		langSelectLayout->addWidget(cb);
	}

	//���¼�
	connect(startBtn, &QPushButton::clicked, this, &QtGuiApplication1::onStartClick);
	connect(selectPathBtn, &QPushButton::clicked, this, &QtGuiApplication1::onSelectPathClick);
	connect(ignoreSysCkBox, &QCheckBox::stateChanged, this, &QtGuiApplication1::onIgnoreStateChanged);
	connect(selectAllCkBox, &QCheckBox::stateChanged, this, &QtGuiApplication1::onSelectAllLangStateChanged);
	connect(selectPathAction, &QAction::triggered, this, &QtGuiApplication1::onSelectPathTriggered);
	connect(exitAction, &QAction::triggered, this, &QtGuiApplication1::onExitTriggered);

	// ���ó�ʼ�ı�
	logTextArea->setText(logText);
}

void QtGuiApplication1::clear()
{
	// ��ձ��
	resultTable->setRowCount(0);

	// ���counter
	counter.clear();

	// ���log
	logText = START_LOG_TEXT;
	logTextArea->clear();
	logTextArea->setText(logText);
}

void QtGuiApplication1::openPath()
{
	QString dir = QFileDialog::getExistingDirectory(this, tr("ѡ��·��..."), root, QFileDialog::ShowDirsOnly);
	if (dir != "")
	{
		root = dir;
	}

	// ��ʾ��·��չʾ����
	selectPathLine->setText(root);
}

void QtGuiApplication1::close()
{
	updateLogText(tr("exit"));
	exit(0);
}

void QtGuiApplication1::updateLogText(QString text)
{
	logTextArea->append(text);
}

void QtGuiApplication1::updateExt()
{
	for (std::vector<QString>::iterator it = extList.begin(); it < extList.end(); ++it)
	{
		// C/Cpp��Ҫ���ͷ�ļ���չ��
		if ((*it) == "c" || (*it) == "cpp")
		{
			extList.push_back(tr("h"));
			break;
		}
	}
}

// ****************************
// ***** �������źŲۺ��� *****
// ****************************
void QtGuiApplication1::onSelectLangStateChanged(int state)
{
	QCheckBox* cb = qobject_cast<QCheckBox*>(sender());
	QString key = lang.getExtByLang((cb->text()).toStdString()).c_str();
	
	if (key == "")
	{
		return;
	}

	if (state == Qt::Checked)
	{
		// ѡ����ӣ�ѭ��Ϊ�˱�֤���ظ����
		bool r = false;
		for (std::vector<QString>::iterator it = extList.begin(); it < extList.end(); ++it)
		{
			if (*it == key)
			{
				r = true;
				break;
			}
		}

		if (!r)
		{
			extList.push_back(key);
		}
	}
	else if(state == Qt::Unchecked)
	{
		// ȡ����ɾ��ָ��Ԫ��
		for (std::vector<QString>::iterator it = extList.begin(); it < extList.end(); ++it)
		{
			if (*it == key)
			{
				extList.erase(it);
				break;
			}
		}
	}

	// �Զ����������ǵ��ȫѡʱ�Ĳ���������Ҫִ������Ĳ���
	if (selectAllFlag)
	{
		return;
	}

	// �����ǵ�������
	selectOneFlag = true;
	// ���������ȣ���ʾ�Ѿ�ȫѡ����Ҫ��ѡȫѡ�򣬷���ȡ����ѡ
	if (lang.supportLang().size() == extList.size())
	{
		selectAllCkBox->setCheckState(Qt::CheckState::Checked);
	}
	else
	{
		selectAllCkBox->setCheckState(Qt::CheckState::Unchecked);
	}
	// ������ɣ��ָ�flag��״̬
	selectOneFlag = false;
}

void QtGuiApplication1::onStartClick()
{
	// �ж��Ƿ��������
	if (root == "")
	{
		updateLogText(tr("ѡ��һ��·��..."));
		return;
	}

	if (extList.size() == 0)
	{
		updateLogText(tr("ѡ����Ҫ���ܵ�����..."));
		return;
	}

	// ���
	clear();

	// ��ʼִ��
	updateLogText(tr("��ʼ..."));

	// �������ѡ����ϵͳ�ļ�������Ҫһ�������������߳�
	std::vector<QString>* _ignoreList;
	if (ignore == false)
	{
		_ignoreList = new std::vector<QString>;
	}
	else {
		_ignoreList = &ignoreList;
	}

	// ����Ҫ��һЩ����ļ���չ���ĸ����Թ��������ѡ����ĳ�����ԣ������Բ���һ����ȫֻ�б�ѡ���һ����չ��������C���Ի�Ӧ�ð���ͷ�ļ�.h
	updateExt();

	// �����̶߳�ȡ�ļ�
	readThread = new ReadThread(root.toStdString(), &counter, &extList, _ignoreList);
	connect(readThread, &ReadThread::updateLogTextSignal, this, &QtGuiApplication1::onUpdateLogText);
	connect(readThread, &ReadThread::finished, this, &QtGuiApplication1::onUpdateResTable);

	readThread->start();
}

void QtGuiApplication1::onSelectPathClick()
{
	openPath();
}

void QtGuiApplication1::onIgnoreStateChanged(int state)
{
	if (state)
	{
		ignore = true;
	}
	else
	{
		ignore = false;
	}
}

void QtGuiApplication1::onSelectAllLangStateChanged(int state)
{
	// ����ѡ�񴥷���ȫѡ��ִ���κβ���
	if (selectOneFlag)
	{
		return;
	}

	Qt::CheckState cs;
	if (state)
	{
		cs = Qt::CheckState::Checked;
	}
	else
	{
		cs = Qt::CheckState::Unchecked;
	}

	// ȫѡģʽ
	selectAllFlag = true;

	for (int i = 1; i < langSelectLayout->count(); i++)
	{
		QCheckBox* cb = qobject_cast<QCheckBox*>(langSelectLayout->itemAt(i)->widget());
		cb->setCheckState(cs);
	}

	selectAllFlag = false;
}

void QtGuiApplication1::onSelectPathTriggered()
{
	openPath();
}

void QtGuiApplication1::onExitTriggered()
{
	close();
}

void QtGuiApplication1::onUpdateLogText(QString text)
{
	updateLogText(text);
}

void QtGuiApplication1::onUpdateResTable()
{
	// ��֪��Ϊʲôֱ����for�ڲ�ʹ��counter.getCounter()һֱ������ֵһ�������ͽ���ˣ�������C++����ѧ��
	std::map<QString, std::map<QString, int>> c = counter.getCounter();
	for (auto it = c.begin(); it != c.end(); ++it)
	{
		// ��ȡ����Ӧ���ļ����ͣ����Ϊ�գ�˵�����ǵ�ǰ������֧�ֵ����ݣ�ֱ�ӷ���
		std::string n = lang.getLang((it->first).toStdString());

		if (n == "") continue;

		resultTable->insertRow(0);

		// ����ļ����͵���һ��
		QTableWidgetItem *headerItem = new QTableWidgetItem(tr(n.c_str()));
		resultTable->setItem(0, 0, headerItem);

		// ������ݵ����������
		for (int i = 0; i < headerV.size(); i++)
		{
			char buffer[10];
			_itoa((it->second)[headerV[i]], buffer, 10);
			QTableWidgetItem *contentItem = new QTableWidgetItem(tr(buffer));
			resultTable->setItem(0, i + 1, contentItem);
		}
	}

	// ������
	updateLogText(tr("��ɣ�"));
}
