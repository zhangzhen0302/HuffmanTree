#include"header_this.h"

/*
* @brief ѹ��������������ѹ�����̶���������������
* @return void
* @author Zhen Zhang
*/
void compress() {
	try {
		string path, filename;
		cout << "Please enter the path of the file:" << endl;
		cin >> path;
		cout << "Please enter the name of compressed file:" << endl;
		cin >> filename;
		cout << "Processing..." << endl;
		// �ı��ļ�����
		size_file_t length = 0;		//ԭ�ļ����ֽ���
		char* file_content = text_input((char*)path.c_str(), &length);
		// ����Ƶ����
		FreqTable* freq_table_obj = new FreqTable(file_content, length);
		vector<record> freq_table = freq_table_obj->getAllRecords();
		delete freq_table_obj;
		// ��������������ɭ��
		vector<HuffmanTree<char>> forest = buildForest(&freq_table[0], freq_table.size());
		// ������������
		HuffmanTree<char>* huff_tree = buildHuff(&forest[0], forest.size());
		// �������
		HuffmanNode<char>* root = huff_tree->getRoot();
		vector<record_CodeTable> code_table;
		fillCodeTable(root, "", &code_table);
		// ����
		vector<char> code;
		int valid_bit = coding(file_content, length, &code_table, &code);			// ���һ���ֽڵ���Чλ
		// ��ѹ���ļ��������
		binary_output((char*)filename.c_str(), MARK, length, &freq_table, &code, valid_bit);
		// �ڴ���������
		delete file_content;
		cout << "Done!" << endl << endl;
	}
	catch (const char* e) {
		cout << e << endl;
		cout << "The program is unable to handle this task." << endl << endl;
	}
	catch (exception e) {
		cout << "Unknown error!" << endl;
		cout << "The program is unable to handle this task." << endl << endl;
	}
	return;
}

/*
* @brief �ù��������ĸ��ڵ��������ĺ���
* @param[in] root:HuffmanNode<wchar_t>*	���������ĸ��ڵ�
			code:string	�����ַ�����ʹ��ʱӦ������ַ���""
			codeTable:vector<record_CodeTable>*	ָ������ָ��
* @return void
* @author Zhen Zhang
*/
void fillCodeTable(HuffmanNode<char>* root, string code, vector<record_CodeTable>* codeTable) {
	if (root->isLeaf()) {
		char ch = root->val();
		record_CodeTable new_record;
		new_record.val = ch;
		new_record.code = code;
		codeTable->push_back(new_record);
		return;
	}
	else {
		fillCodeTable(root->left(), code + "0", codeTable);
		fillCodeTable(root->right(), code + "1", codeTable);
	}
}

/*
* @brief ��ԭ�ļ�����ѹ���ĺ���
* @param[in] input_string:char*	ָ��ԭ�ļ�ȫ�����ݵ�ָ��
			size:size_file_t ԭ�ļ���С
			codeTable:vector<record_CodeTable>*	ָ������ָ��
			code:vector<bool>*	ָ���ѹ�����ݵ�ָ��
* @return int ѹ���������һ���ֽڵ���Чλ��
* @author Zhen Zhang
*/
int coding(char* input_string, size_file_t size, vector<record_CodeTable>* codeTable, vector<char>* code) {
	int size_table = codeTable->size();		//��¼���Ĵ�С������֮��Ĳ���ѭ��
	int bit_pos = 0;	// ��¼��ǰ��bit
	char buffer_char = (char)0x00;		// �ݴ�ѹ�����buffer
	//����ԭ�ļ����ݵ�ÿһ���ַ�
	for (size_file_t iter_string = 0;iter_string < size;iter_string++) {
		char curr = input_string[iter_string];	//�˱���������ŵ�ǰ���ַ�
		string curr_code;	//�˱���������ŵ�ǰ�ַ��Ķ�Ӧѹ����
		//�������ҵ�ǰ�ַ���Ӧ��ѹ����
		for (int iter_table = 0;iter_table < size_table;iter_table++) {
			if (curr == (*codeTable)[iter_table].val) {
				curr_code = (*codeTable)[iter_table].code;		//�ҵ���ѹ���룬���м�¼
				break;
			}
		}
		int size_curr_code = curr_code.size();		//��¼��ǰѹ����Ĵ�С
		//�Ե�ǰѹ������б�������ÿһλ�����λ�洢��code��
		for (int iter_curr = 0;iter_curr < size_curr_code;iter_curr++) {
			char temp = curr_code[iter_curr];	//�˱���������ŵ�ǰλ�õġ�1����0��
			if (bit_pos == 8) {
				code->push_back(buffer_char);
				// �����������һ���ֽ����ݴ���vector������buffer�͵�ǰλ
				buffer_char = (char)0x00;
				bit_pos = 0;
			}
			if (temp == '1') {
				buffer_char += (char)(0x01 << bit_pos);
			}
			bit_pos++;
		}
	}
	code->push_back(buffer_char);
	return bit_pos;
}