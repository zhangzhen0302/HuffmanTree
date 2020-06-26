#include"header_this.h"

/*
* @brief 压缩函数，将所有压缩过程都打包在这个函数中
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
		// 文本文件读入
		size_file_t length = 0;		//原文件的字节数
		char* file_content = text_input((char*)path.c_str(), &length);
		// 构建频数表
		FreqTable* freq_table_obj = new FreqTable(file_content, length);
		vector<record> freq_table = freq_table_obj->getAllRecords();
		delete freq_table_obj;
		// 构建哈夫曼树的森林
		vector<HuffmanTree<char>> forest = buildForest(&freq_table[0], freq_table.size());
		// 构建哈夫曼树
		HuffmanTree<char>* huff_tree = buildHuff(&forest[0], forest.size());
		// 构建码表
		HuffmanNode<char>* root = huff_tree->getRoot();
		vector<record_CodeTable> code_table;
		fillCodeTable(root, "", &code_table);
		// 编码
		vector<char> code;
		int valid_bit = coding(file_content, length, &code_table, &code);			// 最后一个字节的有效位
		// 将压缩文件进行输出
		binary_output((char*)filename.c_str(), MARK, length, &freq_table, &code, valid_bit);
		// 内存垃圾回收
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
* @brief 用哈弗曼树的根节点填充空码表的函数
* @param[in] root:HuffmanNode<wchar_t>*	哈弗曼树的根节点
			code:string	编码字符串。使用时应输入空字符串""
			codeTable:vector<record_CodeTable>*	指向码表的指针
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
* @brief 将原文件内容压缩的函数
* @param[in] input_string:char*	指向原文件全部内容的指针
			size:size_file_t 原文件大小
			codeTable:vector<record_CodeTable>*	指向码表的指针
			code:vector<bool>*	指向空压缩数据的指针
* @return int 压缩数据最后一个字节的有效位数
* @author Zhen Zhang
*/
int coding(char* input_string, size_file_t size, vector<record_CodeTable>* codeTable, vector<char>* code) {
	int size_table = codeTable->size();		//记录码表的大小，用于之后的查找循环
	int bit_pos = 0;	// 记录当前的bit
	char buffer_char = (char)0x00;		// 暂存压缩码的buffer
	//遍历原文件内容的每一个字符
	for (size_file_t iter_string = 0;iter_string < size;iter_string++) {
		char curr = input_string[iter_string];	//此变量用来存放当前的字符
		string curr_code;	//此变量用来存放当前字符的对应压缩码
		//在码表查找当前字符对应的压缩码
		for (int iter_table = 0;iter_table < size_table;iter_table++) {
			if (curr == (*codeTable)[iter_table].val) {
				curr_code = (*codeTable)[iter_table].code;		//找到了压缩码，进行记录
				break;
			}
		}
		int size_curr_code = curr_code.size();		//记录当前压缩码的大小
		//对当前压缩码进行遍历，将每一位存进按位存储的code中
		for (int iter_curr = 0;iter_curr < size_curr_code;iter_curr++) {
			char temp = curr_code[iter_curr];	//此变量用来存放当前位置的“1”或“0”
			if (bit_pos == 8) {
				code->push_back(buffer_char);
				// 将完整编码的一个字节数据存入vector后，重置buffer和当前位
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