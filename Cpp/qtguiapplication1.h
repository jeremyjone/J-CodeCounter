#pragma once
#define START_LOG_TEXT tr("׼��...")

#include <QMainWindow>
#include "ui_qtguiapplication1.h"
#include "counter.h"
#include "lang.h"
#include "readthread.h"
#include <QTextEdit>
#include <QLayout>
#include <QCheckBox>
#include <QLineEdit>
#include <QPushButton>
#include <QTableWidget>
#include <QAction>

class QtGuiApplication1 : public QMainWindow
{
	Q_OBJECT

public:
	QtGuiApplication1(QWidget *parent = Q_NULLPTR);

private slots:
	void onSelectLangStateChanged(int state);
	void onStartClick();
	void onSelectPathClick();
	void onIgnoreStateChanged(int state);
	void onSelectAllLangStateChanged(int state);
	void onSelectPathTriggered();
	void onExitTriggered();
	void onUpdateLogText(QString text);
	void onUpdateResTable();

private:
	void clear();
	void close();
	void openPath();
	void updateLogText(QString text);
	void updateExt();

private:
	Ui::QtGuiApplication1Class ui;

	// ****************************
	// ***** �������ͣ������� *****
	// ****************************
	LangEnum lang;
	CodeCounter counter;

	// ********************
	// ***** һЩ���� *****
	// ********************
	// ��������
	std::vector<QString> ignoreList;
	bool ignore = true;
	// ��Ŀ¼
	QString root = "";
	// ����������Ҫչʾ������ͷ
	std::vector<QString> headerV;
	// ��չ���б�
	std::vector<QString> extList;
	// �·�����ʾ����
	QString logText = START_LOG_TEXT;
	// �߳�
	ReadThread *readThread;

	// ****************************
	// ***** ����С������flag *****
	// ****************************
	// ȫѡ���Ե�flag�������ڲ�����������ʹ��
	bool selectAllFlag = false;
	// ����ѡ�����Ե�flag�������ڲ�����������ʹ��
	bool selectOneFlag = false;

	// ********************
	// ***** С������ *****
	// ********************
	// ��ʽ����
	QLayout* langSelectLayout = Q_NULLPTR;
	QLayout* pathSelectLayout = Q_NULLPTR;
	QLayout* startLayout = Q_NULLPTR;
	// ѡ������
	QCheckBox* selectAllCkBox = Q_NULLPTR;
	// ѡ���·��
	QLineEdit* selectPathLine = Q_NULLPTR;
	QPushButton* selectPathBtn = Q_NULLPTR;
	// ����ѡ��
	QCheckBox* ignoreSysCkBox = Q_NULLPTR;
	QPushButton* startBtn = Q_NULLPTR;
	// �������ʾ���
	QTableWidget* resultTable = Q_NULLPTR;
	// ��־�ı�����
	QTextEdit* logTextArea = Q_NULLPTR;
	//�˵�
	QAction* selectPathAction = Q_NULLPTR;
	QAction* exitAction = Q_NULLPTR;
};
