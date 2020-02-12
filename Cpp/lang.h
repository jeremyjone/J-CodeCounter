#pragma once
#include <iostream>
#include <map>
#include <vector>
#include <string>

class LangEnum
{
private:
	// {Language, Ext}
	std::map<std::string, std::string> langMap;

public:
	LangEnum();
	~LangEnum();

	// ������չ����ȡ��������
	std::string getLang(std::string ext);
	// ��ȡ����֧�ֵ����Ե��б�
	std::vector<std::string> supportLang();
	// �������Բ�����չ��
	std::string getExtByLang(std::string value);
};

