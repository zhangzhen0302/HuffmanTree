#include"header_this.h"

/*
* @brief 文本文件读入函数
* @param[in] path:char*	文件的（相对或绝对）路径
			size:size_file_t* 指向记录原文件大小（字节数）的数据的指针
* @return char* 指向文件中的所有文本内容的指针
* @author Zhen Zhang
*/
char* text_input(char* path, size_file_t* size) {
	ifstream file_in;
	file_in.open(path, ios::binary | ios::_Nocreate);
	//异常处理
	//若文件打开失败，则先猜测用户输入的不是路径而只有文件名，这种情况下该文件应处于程序的同一目录下。
	//所以为文件名补全路径并再次尝试打开。若仍然打开失败则抛出异常
	if (!file_in.is_open()) {
		string path_sub;
		string filename = path;
		path_sub = "./" + filename;
		file_in.open(path_sub, ios::binary | ios::_Nocreate);
		if (!file_in.is_open()) {
			throw "Error 01: File open fail!";
		}
	}
	//获取文件的大小
	file_in.seekg(0, file_in.end);
	size_file_t length = file_in.tellg();
	//若文件大小为0字节，则抛出异常
	if (length == 0)
		throw "Error 04: This file is empty!";
	file_in.seekg(0, file_in.beg);
	//从文件中读入文本数据
	char* buffer = new char[length + 1];	//用于存放从文本文件中读入的内容
	file_in.read(buffer, length);
	buffer[length] = '\0';
	//记录原文件的字节数
	*size = length;
	file_in.close();
	return buffer;
}

/*
* @brief 压缩文件（二进制）写入函数
* @param[in] filename:char*	压缩文件名
			mark:int 文件开头标记
			size_of_former_file:size_file_t 原文件大小（字节数）
			freq_table:vector<record>* 指向频数表的指针
			compressed_data:vector<bool>* 指向压缩后数据的指针
* @return void
* @author Zhen Zhang
*/
void binary_output(char* filename, int mark, size_file_t size_of_former_file, vector<record>* freq_table,
	vector<char>* compressed_data, int valid) {
	ofstream file_out;
	file_out.open(filename, ios::binary | ios::_Noreplace);
	if (!file_out.is_open()) {
		throw "Error 02: File write fail! Check if there is a file with the same name!";
	}
	int size_of_table = sizeof((*freq_table)[0])*freq_table->size();	// 频数表的大小（字节数）
	size_file_t size_of_data = compressed_data->size();		// 压缩后数据的大小（字节数）
	file_out.write((char*)&mark, sizeof(int));
	file_out.write((char*)&size_of_table, sizeof(int));
	file_out.write((char*)&size_of_data, sizeof(size_file_t));
	file_out.write((char*)&size_of_former_file, sizeof(size_file_t));
	file_out.write((char*)&valid, sizeof(int));
	file_out.write((char*)&((*freq_table)[0]), size_of_table);
	file_out.write((char*)&((*compressed_data)[0]), size_of_data);
	file_out.close();
}

/*
* @brief 压缩文件（二进制）读入函数
* @param[in] path:char*	文件的（相对或绝对）路径
			freq_table:record** 指向频数表记录数组的指针
			num_of_valid_digit:int* 指向压缩后数据最后一个字节中的有效位数的指针
			compressed_data:char** 指向压缩文件块指针的指针
			size_of_table:int* 指向频数表大小的指针
			size_of_data:size_file_t* 指向压缩后数据大小的指针
			size_of_former_file:size_file_t* 指向原文件大小的指针
* @return void
* @author Zhen Zhang
*/
void binary_input(char* filename, record** freq_table, int* num_of_valid_digit, 
	char** compressed_data, int* size_of_table, size_file_t* size_of_data, size_file_t* size_of_former_file) {
	ifstream file_in;
	file_in.open(filename, ios::binary | ios::_Nocreate);
	// 异常处理
	// 若文件打开失败，则先猜测用户输入的不是路径而只有文件名，这种情况下该文件应处于程序的同一目录下。
	// 所以为文件名补全路径并再次尝试打开。若仍然打开失败则抛出异常："Error 01: File open fail!"
	if (!file_in.is_open()) {
		string path_sub;
		string filename_str = filename;
		path_sub = "./" + filename_str;
		file_in.open(path_sub, ios::binary | ios::_Nocreate);
		if (!file_in.is_open()) {
			throw "Error 01: File open fail!";
		}
	}
	//获取文件的大小
	file_in.seekg(0, file_in.end);
	size_file_t length = file_in.tellg();
	//若文件大小过小，则抛出异常
	if (length <= (3 * sizeof(int) + 2 * sizeof(size_file_t))) {
		throw "Error 03: This file is not a compressed file!";
	}
	file_in.seekg(0, file_in.beg);
	//标志检测。若文件开头的整数不为MARK，则抛出异常
	int mark = 0;
	file_in.read((char*)(&mark), sizeof(int));
	if (mark != MARK) {
		throw "Error 03: This file is not a compressed file!";
	}
	file_in.read((char*)size_of_table, sizeof(int));
	file_in.read((char*)size_of_data, sizeof(size_file_t));
	file_in.read((char*)size_of_former_file, sizeof(size_file_t));
	//若文件大小计算不正确，则抛出异常
	if (length != ((3 * sizeof(int) + 2 * sizeof(size_file_t)) + *size_of_table + *size_of_data)) {
		throw "Error 03: This file is not a compressed file!";
	}
	file_in.read((char*)num_of_valid_digit, sizeof(int));
	*freq_table = new record[*size_of_table/sizeof(record)];
	file_in.read((char*)(*freq_table), *size_of_table);
	*compressed_data = new char[*size_of_data];
	file_in.read(*compressed_data, *size_of_data);
}

/*
* @brief 压缩文件（二进制）写入函数
* @param[in] filename:char*	目标文件名
			out_file:char* 指向解压后字符串的指针
			size:size_file_t 目标文件大小（字节数）
* @return void
* @author Zhen Zhang
*/
void text_output(char* filename, char* out_file, size_file_t size) {
	ofstream file_out;
	file_out.open(filename, ios::binary | ios::_Noreplace);
	if (!file_out.is_open()) {
		throw "Error 02: File write fail! Check if there is a file with the same name!";
	}
	file_out.write(out_file, size);
	file_out.close();
}