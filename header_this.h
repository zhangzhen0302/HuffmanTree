#pragma once

#pragma warning(disable : 4996)

#include<iostream>
#include<fstream>
#include<string>
#include<vector>

using namespace std;

// 宏定义引用
#include"macro.h"


//其它类的头文件引用
#include"FreqTable.h"

#include"HuffmanTree.h"

#include"Heap.h"

#include"CodeTable.h"


//函数声明

void compress();

void decompress();

char* text_input(char* path, size_file_t* size);

void binary_output(char* filename, int mark, size_file_t size_of_former_file, vector<record>* freq_table,
	vector<char>* compressed_data, int valid);

void binary_input(char* filename, record** freq_table, int* num_of_valid_digit,
	char** compressed_data, int* size_of_table, size_file_t* size_of_data, size_file_t* size_of_former_file);

void text_output(char* filename, char* out_file, size_file_t size);

vector<HuffmanTree<char>> buildForest(record* freq_table, int count);

void fillCodeTable(HuffmanNode<char>* root, string code, vector<record_CodeTable>* codeTable);

int coding(char* input_string, size_file_t size, vector<record_CodeTable>* codeTable, vector<char>* code);

char* decoding(char* data, size_file_t size, int valid, size_file_t length, HuffmanTree<char>* tree);