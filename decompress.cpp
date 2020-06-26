#include"header_this.h"

/*
* @brief 解压缩函数，将所有解压缩过程都打包在这个函数中
* @return void
* @author Zhen Zhang
*/
void decompress() {
	try {
		// 压缩文件读入
		string path, filename;
		cout << "Please enter the path of the compressed file:" << endl;
		cin >> path;
		cout << "Please enter the name of output file:" << endl;
		cin >> filename;
		cout << "Processing..." << endl;
		record* freq_table = 0;		// 频数表
		char* compressed_data = 0;		// 压缩后数据
		int num_of_valid_digit = 0;		// 压缩后数据最后一个字节中的有效位数
		int size_of_table = 0;		// 频数表的大小（字节数）
		size_file_t size_of_data = 0;	// 压缩后数据的大小（字节数）
		size_file_t size_of_former_file = 0;	// 原文件的大小（字节数）
		// 压缩后数据读入
		binary_input((char*)path.c_str(), &freq_table, &num_of_valid_digit, &compressed_data, &size_of_table, &size_of_data, &size_of_former_file);
		// 构建哈夫曼树的森林
		int length_of_table = size_of_table / sizeof(record);	// 频数表的长度（即元素个数）
		vector<HuffmanTree<char>> forest = buildForest(&freq_table[0], length_of_table);
		delete[] freq_table;
		// 构建哈夫曼树
		HuffmanTree<char>* huff_tree = buildHuff(&forest[0], forest.size());
		// 解码
		char* decompressed_data;
		decompressed_data = decoding(compressed_data, size_of_data, num_of_valid_digit, size_of_former_file, huff_tree);
		// 将解压后文件进行输出
		text_output((char*)filename.c_str(), decompressed_data, size_of_former_file);
		// 内存垃圾回收
		delete[] compressed_data;
		delete[] decompressed_data;
		cout << "Done!" << endl << endl;
	}
	catch(const char* e) {
		cout << e << endl;
		cout << "The program is unable to handle this task." << endl << endl;
	}
	catch (exception e) {
		cout << "Unknown error! Check if the compressed file is damaged." << endl;
		cout << "The program is unable to handle this task." << endl << endl;
	}
	return;
}

/*
* @brief 将压缩数据还原的函数
* @param[in] data:char* 指向压缩数据的指针
			size:size_file_t 压缩数据的大小（字节数）
			valid:int 压缩后数据最后一个字节中的有效位数
			length:size_file_t 压缩前文件的大小（字节数）
			tree:HuffmanTree<char>* 指向哈弗曼编码树的指针
* @return char* 指向解压后的字符数组的指针
* @author Zhen Zhang
*/
char* decoding(char* data, size_file_t size, int valid, size_file_t length, HuffmanTree<char>* tree) {
	char* decoded = new char[length];		// 为解码数据定义出空间
	size_file_t write_pos = 0;		// 此变量记录当前在解码字符数组中准备写入的位置
	HuffmanNode<char>* root = tree->getRoot();	// 指向树根的指针，用于解码时归位
	HuffmanNode<char>* curr_node = root;	// 当前节点指针
	// 处理除了最后一个字节外的所有压缩数据
	// 最后一个字节只有部分位是有效的，放置于最后单独处理可以节省判断开支
	char mask = 0x01;	// 掩码
	for (size_file_t iter_data = 0;iter_data < size - 1;iter_data++) {
		char curr_char = data[iter_data];
		mask = 0x01;	// 重置掩码
		for (int i = 0;i < 8;i++) {
			char curr_bit = (curr_char & mask) >> i;
			// 沿着树向下查找
			if (curr_bit == 0x00)
				curr_node = curr_node->left();
			else
				curr_node = curr_node->right();
			// 判断是否抵达树根。若是则在解码字符串里添加新的字符，并将当前节点指针重置
			if (curr_node->isLeaf()) {
				decoded[write_pos] = curr_node->val();
				write_pos++;
				curr_node = root;
			}
			mask = mask << 1;
		}
	}
	// 对最后一个字节进行解码
	mask = 0x01;	// 重置掩码
	for (int j = 0;j < valid;j++) {
		char curr_bit = (data[size - 1] & mask) >> j;
		// 沿着树向下查找
		if(curr_bit == 0x00)
			curr_node = curr_node->left();
		else
			curr_node = curr_node->right();
		// 判断是否抵达树根。若是则在解码字符串里添加新的字符，并将当前节点指针重置
		if (curr_node->isLeaf()) {
			decoded[write_pos] = curr_node->val();
			write_pos++;
			curr_node = root;
		}
		mask = mask << 1;
	}
	return decoded;
}