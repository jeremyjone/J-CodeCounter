#include "read.h"

void strip(std::string& str)
{
	std::string blanks("\f\v\r\t\n ");
	str.erase(0, str.find_first_not_of(blanks));
	str.erase(str.find_last_not_of(blanks) + 1);
}

Read::Read(std::string ext, std::vector<std::string>* lines)
{
	this->lines = lines;

	// ����extʹ�õ��ò�ִͬ�к���
	if (ext == "c" || ext == "cpp" || ext == "cs")
	{
		cpp();
	}
	else if (ext == "html")
	{
		html();
	}
	else if (ext == "java" || ext == "js")
	{
		java();
	}
	else if (ext == "py")
	{
		python();
	}
	else if (ext == "vue")
	{
		vue();
	}
}

int Read::getCode()
{
	return code;
}

int Read::getBlank()
{
	return blank;
}

int Read::getComment()
{
	return comment;
}

void Read::cpp()
{
	std::string cmt = "";

	for (int i = 0; i < (*lines).size(); i++)
	{
		std::string line = (*lines)[i];
		strip(line); // ȥ���հ��ַ�
		transform(line.begin(), line.end(), line.begin(), ::tolower); // תΪСд

		// ����
		if (line == "")
		{
			blank++;
		}
		else
		{
			if (cmt == "")
			{
				if (std::regex_search(line, std::regex("^#if")))
				{
					comment++;
					cmt = "^#endif";
				}
				else if (std::regex_search(line, std::regex("^/\\*")))
				{
					comment++;
					// �����ж�
					if (!(std::regex_search(line, std::regex("\\*/$"))))
					{
						cmt = "\\*/$";
					}
				}
				else if (std::regex_search(line, std::regex("^/")))
				{
					comment++;
				}
				else
				{
					// �����ǣ����Ǵ���
					code++;
				}
			}
			else
			{
				comment++;

				if (std::regex_search(line, std::regex(cmt)))
				{
					cmt = "";
				}
			}
		}
	}
}

void Read::html()
{
	std::string cmt = "";

	for (int i = 0; i < (*lines).size(); i++)
	{
		std::string line = (*lines)[i];
		strip(line); // ȥ���հ��ַ�
		transform(line.begin(), line.end(), line.begin(), ::tolower); // תΪСд

		// ����
		if (line == "")
		{
			blank++;
		}
		else
		{
			if (cmt == "")
			{
				if (std::regex_search(line, std::regex("^<!--")))
				{
					comment++;
					// �����ж�
					if (!(std::regex_search(line, std::regex("-->$"))))
					{
						cmt = "-->";
					}
				}
				else
				{
					// �����ǣ����Ǵ���
					code++;
				}
			}
			else
			{
				comment++;

				if (std::regex_search(line, std::regex(cmt)))
				{
					cmt = "";
				}
			}
		}
	}
}

void Read::java()
{
	std::string cmt = "";

	for (int i = 0; i < (*lines).size(); i++)
	{
		std::string line = (*lines)[i];
		strip(line); // ȥ���հ��ַ�
		transform(line.begin(), line.end(), line.begin(), ::tolower); // תΪСд

		// ����
		if (line == "")
		{
			blank++;
		}
		else
		{
			if (cmt == "")
			{
				if (std::regex_search(line, std::regex("^/\\*")))
				{
					comment++;
					// �����ж�
					if (!(std::regex_search(line, std::regex("\\*/$"))))
					{
						cmt = "\\*/$";
					}
				}
				else if (std::regex_search(line, std::regex("^/")))
				{
					comment++;
				}
				else
				{
					// �����ǣ����Ǵ���
					code++;
				}
			}
			else
			{
				comment++;

				if (std::regex_search(line, std::regex(cmt)))
				{
					cmt = "";
				}
			}
		}
	}
}

void Read::python()
{
	std::string cmt = "";

	for (int i = 0; i < (*lines).size(); i++)
	{
		std::string line = (*lines)[i];
		strip(line); // ȥ���հ��ַ�
		transform(line.begin(), line.end(), line.begin(), ::tolower); // תΪСд

		// ����
		if (line == "")
		{
			blank++;
		}
		else
		{
			if (cmt == "")
			{
				if (std::regex_search(line, std::regex("^#")))
				{
					comment++;
				}
				else if (std::regex_search(line, std::regex("^\"\"\"")))
				{
					comment++;
					// �����ж�
					if (!(std::regex_search(line, std::regex("\"\"\"$"))))
					{
						cmt = "\"\"\"$";
					}
				}
				else if (std::regex_search(line, std::regex("^\'\'\'")))
				{
					comment++;
					// �����ж�
					if (!(std::regex_search(line, std::regex("\'\'\'$"))))
					{
						cmt = "\'\'\'$";
					}
				}
				else
				{
					// �����ǣ����Ǵ���
					code++;
				}
			}
			else
			{
				comment++;

				if (std::regex_search(line, std::regex(cmt)))
				{
					cmt = "";
				}
			}
		}
	}
}

void Read::vue()
{
	std::string cmt = "";

	for (int i = 0; i < (*lines).size(); i++)
	{
		std::string line = (*lines)[i];
		strip(line); // ȥ���հ��ַ�
		transform(line.begin(), line.end(), line.begin(), ::tolower); // תΪСд

		// ����
		if (line == "")
		{
			blank++;
		}
		else
		{
			if (cmt == "")
			{
				if (std::regex_search(line, std::regex("^<!--")))
				{
					comment++;
					// �����ж�
					if (!(std::regex_search(line, std::regex("-->$"))))
					{
						cmt = "-->";
					}
				}
				else if (std::regex_search(line, std::regex("^/\\*")))
				{
					comment++;
					// �����ж�
					if (!(std::regex_search(line, std::regex("\\*/$"))))
					{
						cmt = "\\*/$";
					}
				}
				else if (std::regex_search(line, std::regex("^/")))
				{
					comment++;
				}
				else
				{
					// �����ǣ����Ǵ���
					code++;
				}
			}
			else
			{
				comment++;

				if (std::regex_search(line, std::regex(cmt)))
				{
					cmt = "";
				}
			}
		}
	}
}
