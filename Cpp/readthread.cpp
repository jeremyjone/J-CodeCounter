#include "readthread.h"

ReadThread::ReadThread(std::string path, CodeCounter* counter, std::vector<QString> *extList, std::vector<QString>* ignoreList, QObject* parent)
{
	this->rootPath = path;
	this->counter = counter;
	this->extList = extList;
	this->ignoreList = ignoreList;
}

void ReadThread::run()
{
	// �������в��ҵ����ļ�·��
	std::vector<std::string> files;
	recPath_Win(rootPath, files);

	readFiles(files);
}

void ReadThread::recPath_Win(std::string path, std::vector<std::string>& files)
{
	//�ļ����
	/**
	* �����и��ӣ���win10��vs2019�����������vs2017���϶���Ҫ�������ҵ�sv2017�������У��У�
	* ��� hFile ������long long��ʽ������intptr_t��ʽ�������һֱ��
	* _findnext(hFile, &fileinfo) == 0 ��һ���� ntdll.dllд���ͻ �Ĵ�
	* �����vs2015�������£����Ҳ���win10����ôlong��ʽ�Ϳ���ʹ�á�
	*/
	long long hFile = 0;

	//�ļ���Ϣ  
	struct _finddata_t fileinfo;  //�����õ��ļ���Ϣ��ȡ�ṹ
	std::string p;  //string�������˼��һ����ֵ����:assign()���кܶ����ذ汾
	if ((hFile = _findfirst(p.assign(path).append("\\*").c_str(), &fileinfo)) != -1)
	{
		do
		{
			if ((fileinfo.attrib & _A_SUBDIR))  //�Ƚ��ļ������Ƿ����ļ���
			{
				// ����ļ�����'.'�㿪ͷ��Ĭ�ϲ���ȡ����Ϊ���������ļ����ܿ�����ϵͳ�ļ�
				if (std::regex_search(fileinfo.name, std::regex("^\\.")))
				{
					continue;
				}

				// �ж��ļ����Ƿ�Ϊ��Ҫ���Ե��ļ��У�����ǣ�ֱ������
				std::string lastDir = "";
				size_t last_slash_idx = path.rfind('\\');
				if (last_slash_idx == std::string::npos)
				{
					// Ŀ¼���'\\'�Ҳ�����ô����'/'��������Ҳ������������ж�
					last_slash_idx = path.rfind('/');
				}
				if (std::string::npos != last_slash_idx)
				{
					lastDir = path.substr(last_slash_idx + 1, path.size());
				}

				// �жϵ�ǰĿ¼�Ƿ�����Ҫ���Ե��ļ���
				bool con = false;
				for (std::vector<QString>::iterator it = ignoreList->begin(); it < ignoreList->end(); ++it)
				{
					if ((*it) == tr(lastDir.c_str()))
					{
						con = true;
						break;
					}
				}

				if (con)
				{
					// ��ǰ�ļ�������Ҫ���Եģ�ֱ����һ��ѭ��
					continue;
				}

				if (strcmp(fileinfo.name, ".") != 0 && strcmp(fileinfo.name, "..") != 0)
				{
					// �ݹ���һ���ļ���
					recPath_Win(p.assign(path).append("/").append(fileinfo.name), files);
				}
			}
			else
			{
				// ����ļ�����Ҫ�������б��У����ļ�·����ӵ�files��
				QString _ext = tr(getExt(fileinfo.name).c_str());
				for (std::vector<QString>::iterator it = extList->begin(); it < extList->end(); ++it)
				{
					if ((*it) == _ext)
					{
						files.push_back(p.assign(path).append("/").append(fileinfo.name));
					}
				}
			}
		} while (_findnext(hFile, &fileinfo) == 0);  //Ѱ����һ�����ɹ�����0������-1
		
		_findclose(hFile);
	}
}

void ReadThread::readFiles(std::vector<std::string>& files)
{
	// ѭ����ȡ�ļ�
	for (int i = 0; i < files.size(); i++)
	{
		// ��ӡ�ļ���Ϣ��log��
		emit updateLogTextSignal(tr(files[i].c_str()));

		// ���ļ�
		std::fstream f(files[i]);
		std::vector<std::string> words;
		std::string line;

		// ��ȡ�ļ���ÿһ�У������浽words��
		while (std::getline(f, line))
		{
			words.push_back(line);
		}

		// ����ÿһ��
		// �Ȼ�ȡ�ļ���չ��
		std::string ext = getExt(files[i]);

		// ����һ��Readʵ���������������
		Read read(ext, &words);

		counter->setCounter(tr(ext.c_str()), "file", 1);
		counter->setCounter(tr(ext.c_str()), "code", read.getCode());
		counter->setCounter(tr(ext.c_str()), "blank", read.getBlank());
		counter->setCounter(tr(ext.c_str()), "comment", read.getComment());
	}
}

std::string ReadThread::getExt(std::string path, bool eraseDot)
{
	char _drive[_MAX_DRIVE];
	char _dir[_MAX_DIR];
	char _fname[_MAX_FNAME];
	char _ext[_MAX_EXT];
	_splitpath(path.c_str(), _drive, _dir, _fname, _ext);
	std::string ext(_ext);

	if (eraseDot)
	{
		ext.erase(0, 1); // ɾ����ȡ��չ���ĵ�һ����(.)
	}

	return ext;
}
