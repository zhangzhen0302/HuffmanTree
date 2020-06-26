#include"header_this.h"

/*
* @brief 用频数表建立森林的函数
* @param[in] freq_table:record*	频数表
			count:int 频数表的长度
* @return vector<HuffmanTree<wchar_t>>	哈弗曼树组成的森林
* @author Zhen Zhang
*/
vector<HuffmanTree<char>> buildForest(record* freq_table, int count) {
	int size = count;
	vector<HuffmanTree<char>> forest;
	HuffmanTree<char>* in = NULL;
	//遍历频数表，产生森林
	for (int iter = 0;iter < size;iter++) {
		in = new HuffmanTree<char>(freq_table[iter].val, freq_table[iter].freq);
		forest.push_back(*in);
		delete in;
	}
	return forest;
}