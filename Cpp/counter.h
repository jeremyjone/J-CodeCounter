#pragma once
#include <iostream>
#include <map>
#include <QString>

class CodeCounter
{
private:
	std::map<QString, std::map<QString, int>> counter;

public:
	CodeCounter();
	~CodeCounter();

	// ��ȡcounter
	std::map<QString, std::map<QString, int>> getCounter();
	// ���counter
	void setCounter(QString lang, QString key, int value);
	// ���counter
	void clear();
};

