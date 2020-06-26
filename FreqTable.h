#pragma once
#include<string>
#include<vector>
#include"macro.h"
typedef struct Record_FreqTable record;

using namespace std;

//定义频数表类
class FreqTable {
public:
	FreqTable(char* input_string,size_file_t length);
	vector<record> getAllRecords();
	int size();
private:
	vector<record> records;
	void buildTableHelp(char* input_string, size_file_t length);
};

//定义频数表中的记录
struct Record_FreqTable {
	int freq = 0;	//字符出现的次数。在排序时是关键字
	char val;		//字符的值
};