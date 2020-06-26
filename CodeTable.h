#pragma once
#include<iostream>
#include<string>

using namespace std;

//定义码表中的记录
struct Record_CodeTable {
	string code;	//字符的压缩编码，储存在字符串中
	char val;	//字符的值
};
typedef struct Record_CodeTable record_CodeTable;

