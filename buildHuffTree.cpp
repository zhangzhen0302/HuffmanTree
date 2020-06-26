#include"header_this.h"

/*
* @brief ��Ƶ������ɭ�ֵĺ���
* @param[in] freq_table:record*	Ƶ����
			count:int Ƶ����ĳ���
* @return vector<HuffmanTree<wchar_t>>	����������ɵ�ɭ��
* @author Zhen Zhang
*/
vector<HuffmanTree<char>> buildForest(record* freq_table, int count) {
	int size = count;
	vector<HuffmanTree<char>> forest;
	HuffmanTree<char>* in = NULL;
	//����Ƶ��������ɭ��
	for (int iter = 0;iter < size;iter++) {
		in = new HuffmanTree<char>(freq_table[iter].val, freq_table[iter].freq);
		forest.push_back(*in);
		delete in;
	}
	return forest;
}