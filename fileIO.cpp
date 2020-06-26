#include"header_this.h"

/*
* @brief �ı��ļ����뺯��
* @param[in] path:char*	�ļ��ģ���Ի���ԣ�·��
			size:size_file_t* ָ���¼ԭ�ļ���С���ֽ����������ݵ�ָ��
* @return char* ָ���ļ��е������ı����ݵ�ָ��
* @author Zhen Zhang
*/
char* text_input(char* path, size_file_t* size) {
	ifstream file_in;
	file_in.open(path, ios::binary | ios::_Nocreate);
	//�쳣����
	//���ļ���ʧ�ܣ����Ȳ²��û�����Ĳ���·����ֻ���ļ�������������¸��ļ�Ӧ���ڳ����ͬһĿ¼�¡�
	//����Ϊ�ļ�����ȫ·�����ٴγ��Դ򿪡�����Ȼ��ʧ�����׳��쳣
	if (!file_in.is_open()) {
		string path_sub;
		string filename = path;
		path_sub = "./" + filename;
		file_in.open(path_sub, ios::binary | ios::_Nocreate);
		if (!file_in.is_open()) {
			throw "Error 01: File open fail!";
		}
	}
	//��ȡ�ļ��Ĵ�С
	file_in.seekg(0, file_in.end);
	size_file_t length = file_in.tellg();
	//���ļ���СΪ0�ֽڣ����׳��쳣
	if (length == 0)
		throw "Error 04: This file is empty!";
	file_in.seekg(0, file_in.beg);
	//���ļ��ж����ı�����
	char* buffer = new char[length + 1];	//���ڴ�Ŵ��ı��ļ��ж��������
	file_in.read(buffer, length);
	buffer[length] = '\0';
	//��¼ԭ�ļ����ֽ���
	*size = length;
	file_in.close();
	return buffer;
}

/*
* @brief ѹ���ļ��������ƣ�д�뺯��
* @param[in] filename:char*	ѹ���ļ���
			mark:int �ļ���ͷ���
			size_of_former_file:size_file_t ԭ�ļ���С���ֽ�����
			freq_table:vector<record>* ָ��Ƶ�����ָ��
			compressed_data:vector<bool>* ָ��ѹ�������ݵ�ָ��
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
	int size_of_table = sizeof((*freq_table)[0])*freq_table->size();	// Ƶ����Ĵ�С���ֽ�����
	size_file_t size_of_data = compressed_data->size();		// ѹ�������ݵĴ�С���ֽ�����
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
* @brief ѹ���ļ��������ƣ����뺯��
* @param[in] path:char*	�ļ��ģ���Ի���ԣ�·��
			freq_table:record** ָ��Ƶ�����¼�����ָ��
			num_of_valid_digit:int* ָ��ѹ�����������һ���ֽ��е���Чλ����ָ��
			compressed_data:char** ָ��ѹ���ļ���ָ���ָ��
			size_of_table:int* ָ��Ƶ�����С��ָ��
			size_of_data:size_file_t* ָ��ѹ�������ݴ�С��ָ��
			size_of_former_file:size_file_t* ָ��ԭ�ļ���С��ָ��
* @return void
* @author Zhen Zhang
*/
void binary_input(char* filename, record** freq_table, int* num_of_valid_digit, 
	char** compressed_data, int* size_of_table, size_file_t* size_of_data, size_file_t* size_of_former_file) {
	ifstream file_in;
	file_in.open(filename, ios::binary | ios::_Nocreate);
	// �쳣����
	// ���ļ���ʧ�ܣ����Ȳ²��û�����Ĳ���·����ֻ���ļ�������������¸��ļ�Ӧ���ڳ����ͬһĿ¼�¡�
	// ����Ϊ�ļ�����ȫ·�����ٴγ��Դ򿪡�����Ȼ��ʧ�����׳��쳣��"Error 01: File open fail!"
	if (!file_in.is_open()) {
		string path_sub;
		string filename_str = filename;
		path_sub = "./" + filename_str;
		file_in.open(path_sub, ios::binary | ios::_Nocreate);
		if (!file_in.is_open()) {
			throw "Error 01: File open fail!";
		}
	}
	//��ȡ�ļ��Ĵ�С
	file_in.seekg(0, file_in.end);
	size_file_t length = file_in.tellg();
	//���ļ���С��С�����׳��쳣
	if (length <= (3 * sizeof(int) + 2 * sizeof(size_file_t))) {
		throw "Error 03: This file is not a compressed file!";
	}
	file_in.seekg(0, file_in.beg);
	//��־��⡣���ļ���ͷ��������ΪMARK�����׳��쳣
	int mark = 0;
	file_in.read((char*)(&mark), sizeof(int));
	if (mark != MARK) {
		throw "Error 03: This file is not a compressed file!";
	}
	file_in.read((char*)size_of_table, sizeof(int));
	file_in.read((char*)size_of_data, sizeof(size_file_t));
	file_in.read((char*)size_of_former_file, sizeof(size_file_t));
	//���ļ���С���㲻��ȷ�����׳��쳣
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
* @brief ѹ���ļ��������ƣ�д�뺯��
* @param[in] filename:char*	Ŀ���ļ���
			out_file:char* ָ���ѹ���ַ�����ָ��
			size:size_file_t Ŀ���ļ���С���ֽ�����
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