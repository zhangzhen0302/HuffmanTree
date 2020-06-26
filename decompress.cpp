#include"header_this.h"

/*
* @brief ��ѹ�������������н�ѹ�����̶���������������
* @return void
* @author Zhen Zhang
*/
void decompress() {
	try {
		// ѹ���ļ�����
		string path, filename;
		cout << "Please enter the path of the compressed file:" << endl;
		cin >> path;
		cout << "Please enter the name of output file:" << endl;
		cin >> filename;
		cout << "Processing..." << endl;
		record* freq_table = 0;		// Ƶ����
		char* compressed_data = 0;		// ѹ��������
		int num_of_valid_digit = 0;		// ѹ�����������һ���ֽ��е���Чλ��
		int size_of_table = 0;		// Ƶ����Ĵ�С���ֽ�����
		size_file_t size_of_data = 0;	// ѹ�������ݵĴ�С���ֽ�����
		size_file_t size_of_former_file = 0;	// ԭ�ļ��Ĵ�С���ֽ�����
		// ѹ�������ݶ���
		binary_input((char*)path.c_str(), &freq_table, &num_of_valid_digit, &compressed_data, &size_of_table, &size_of_data, &size_of_former_file);
		// ��������������ɭ��
		int length_of_table = size_of_table / sizeof(record);	// Ƶ����ĳ��ȣ���Ԫ�ظ�����
		vector<HuffmanTree<char>> forest = buildForest(&freq_table[0], length_of_table);
		delete[] freq_table;
		// ������������
		HuffmanTree<char>* huff_tree = buildHuff(&forest[0], forest.size());
		// ����
		char* decompressed_data;
		decompressed_data = decoding(compressed_data, size_of_data, num_of_valid_digit, size_of_former_file, huff_tree);
		// ����ѹ���ļ��������
		text_output((char*)filename.c_str(), decompressed_data, size_of_former_file);
		// �ڴ���������
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
* @brief ��ѹ�����ݻ�ԭ�ĺ���
* @param[in] data:char* ָ��ѹ�����ݵ�ָ��
			size:size_file_t ѹ�����ݵĴ�С���ֽ�����
			valid:int ѹ�����������һ���ֽ��е���Чλ��
			length:size_file_t ѹ��ǰ�ļ��Ĵ�С���ֽ�����
			tree:HuffmanTree<char>* ָ���������������ָ��
* @return char* ָ���ѹ����ַ������ָ��
* @author Zhen Zhang
*/
char* decoding(char* data, size_file_t size, int valid, size_file_t length, HuffmanTree<char>* tree) {
	char* decoded = new char[length];		// Ϊ�������ݶ�����ռ�
	size_file_t write_pos = 0;		// �˱�����¼��ǰ�ڽ����ַ�������׼��д���λ��
	HuffmanNode<char>* root = tree->getRoot();	// ָ��������ָ�룬���ڽ���ʱ��λ
	HuffmanNode<char>* curr_node = root;	// ��ǰ�ڵ�ָ��
	// ����������һ���ֽ��������ѹ������
	// ���һ���ֽ�ֻ�в���λ����Ч�ģ���������󵥶�������Խ�ʡ�жϿ�֧
	char mask = 0x01;	// ����
	for (size_file_t iter_data = 0;iter_data < size - 1;iter_data++) {
		char curr_char = data[iter_data];
		mask = 0x01;	// ��������
		for (int i = 0;i < 8;i++) {
			char curr_bit = (curr_char & mask) >> i;
			// ���������²���
			if (curr_bit == 0x00)
				curr_node = curr_node->left();
			else
				curr_node = curr_node->right();
			// �ж��Ƿ�ִ��������������ڽ����ַ���������µ��ַ���������ǰ�ڵ�ָ������
			if (curr_node->isLeaf()) {
				decoded[write_pos] = curr_node->val();
				write_pos++;
				curr_node = root;
			}
			mask = mask << 1;
		}
	}
	// �����һ���ֽڽ��н���
	mask = 0x01;	// ��������
	for (int j = 0;j < valid;j++) {
		char curr_bit = (data[size - 1] & mask) >> j;
		// ���������²���
		if(curr_bit == 0x00)
			curr_node = curr_node->left();
		else
			curr_node = curr_node->right();
		// �ж��Ƿ�ִ��������������ڽ����ַ���������µ��ַ���������ǰ�ڵ�ָ������
		if (curr_node->isLeaf()) {
			decoded[write_pos] = curr_node->val();
			write_pos++;
			curr_node = root;
		}
		mask = mask << 1;
	}
	return decoded;
}