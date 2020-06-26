#pragma once
#include<string>
#include<vector>
#include"macro.h"
typedef struct Record_FreqTable record;

using namespace std;

//����Ƶ������
class FreqTable {
public:
	FreqTable(char* input_string,size_file_t length);
	vector<record> getAllRecords();
	int size();
private:
	vector<record> records;
	void buildTableHelp(char* input_string, size_file_t length);
};

//����Ƶ�����еļ�¼
struct Record_FreqTable {
	int freq = 0;	//�ַ����ֵĴ�����������ʱ�ǹؼ���
	char val;		//�ַ���ֵ
};