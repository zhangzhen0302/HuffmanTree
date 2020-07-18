# HuffmanTree
Huffman编码实现文件压缩和解码 
 
一、项目的目的与要求
实现Huffman编码，同时借助Huffman编码对文件进行一定的操作。其中包括一下几点：
a.	通过文件IO，判断文件是否存在；对文件进行读写操作
b.	借助二进制读写的方式，实现对多种格式文件（TXT、PDF、JPG……）的压缩
c.	根据使用者输入的文件名，将压缩文件输出
d.	对文件进行判断，是否为编码文件；对编码文件进行解码
二、工具/准备工作
所需知识：数据结构二叉数的实现；Huffman编码；C++基本知识；
编码工具：Visual studio 2017
三、分析
1.	压缩和解码文件过程
a)	编码和压缩文件过程
i.	将文件以二进制读入
ii.	建立频数表
iii.	建Huffman森林
iv.	建堆（最小堆）
v.	建Huffman树
vi.	建码表
vii.	根据码表对文件进行编码压缩
b)	解码和解压缩文件过程
i.	判断文件是否为编码文件
ii.	从文件头中获取频数表
iii.	根据频数表建Huffman树
iv.	解码
2.	重要算法实现
a)	最小堆算法实现
i.	借助siftdown方法对获得的Huffman森林进行建堆，建立最小堆的类。具体siftdown代码实现如下：
 
ii.	最小堆类中其他函数声明：
 
b)	Huffman树算法实现
由于上述过程中建立了由Huffman森林组成的最小堆，最小堆中的元素对应频率表中的元素，同时也即是n个初始的Huffman树。通过取最小堆的根节点，可以依次获得权重最小的两棵树，再把他们标记为Huffman树的叶节点，把这两个叶节点标记为一个分支结点的两个子结点，而这个结点的权重即为两个叶结点的权重。再将获得的新树放回最小堆，siftdown获得新的最小堆，重复上述过程，直到最小堆中只有一个元素，则Huffman树建立完毕。
 
 
c)	通过栈遍历Huffman树，获取码表
 
3.	编码和压缩结果
由上述几步，我们获得了码表和频率表，故根据每个元素的Huffman编码将原文件进行编码，最后我们得到编码后的数据。最终压缩文件由以下组成：
	Mark(int)：标志，用来判断是否为压缩文件
	Size(int):用来记录频数表的大小
	Size_file_t:数据大小
	Size_file_t:原文件字节数
	Vaild_int:有效位
	频数表
	数据
4.	声明
由于解码和解压缩是根据压缩文件中存储的频数表重构建Huffman树，再通过数据对Huffman树进行找寻，所以实际上并没有额外的算法实现，故在这里不在描述解码和解压缩的过程。
四、完整程序代码
1.	代码文件总览
 
2.	具体代码
a)	头文件代码
i.	CodeTable.h: 定义码表的结构体
 
ii.	FreqTable.h:定义频率表类以及频率表中的记录
  
iii.	header_this.h:函数声明
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

iv.	Heap.h:定义最小堆类及类的实现
#pragma once

using namespace std;

//"小顶"堆类
template<typename E,typename Comp>
class Heap {
private:
	E* heap;		//指向堆的指针
	int max_size;	//堆的最大尺寸
	int size;		//当前堆中的元素总数

	//厉害的siftdown函数，用于将元素放到正确的位置上
	void siftdown(int pos) {
		while (!isLeaf(pos)) {
			int j = leftChild(pos);
			int rc = rightChild(pos);
			if ((rc < size) && Comp::prior(heap[j], heap[rc]))
				j = rc;		//将pos的左右孩子中最小的那个的赋给j
			if (Comp::prior(heap[j], heap[pos])) return;	//若pos小于j，则siftdown完成
			swap(pos, j);
			pos = j;
		}
	}

public:
	Heap(E* array, int size, int max_size) {
		heap = array;
		this->size = size;
		this->max_size = max_size;
	}
	int getSize() const {
		return size;
	}
	bool isLeaf(int pos) const {
		return (pos >= size / 2) && (pos < size);
	}
	int leftChild(int pos) const {
		return 2 * pos + 1;
	}
	int rightChild(int pos) const {
		return 2 * pos + 2;
	}
	int parent(int pos) const {
		return (pos - 1) / 2;
	}
	void buildHeap() {
		for (int iter = size / 2 - 1;iter >= 0;iter--) {
			siftdown(iter);
		}
	}

	//把“it”插入到堆中
	void insert(E it) {
		if (size >= max_size) throw "Heap is full!";
		int curr = size++;
		heap[curr] = it;
		//memcpy_s(heap + curr, 4, &it, 4);
		//将it向上移动
		while ((curr != 0) &&
			(Comp::prior(heap[parent(curr)], heap[curr]))) {
			swap(curr, parent(curr));
			curr = parent(curr);
		}
	}
	//移除第一个元素
	E removeFirst() {
		if (size <= 0) throw "Heap is empty!";
		swap(0, --size);		//用最后一个元素siftdown
		if (size != 0) siftdown(0);		//siftdown以产生新的根节点
		return heap[size];
	}
	//交换两个元素的位置
	void swap(int pos_a,int pos_b) {
		E temp;
		temp = heap[pos_a];
		heap[pos_a] = heap[pos_b];
		heap[pos_b] = temp;
	}
};
v.	HuffmanTree.h:定义Huffman树类、内部节点类、比较类等
#pragma once

#include"Heap.h"

using namespace std;

//哈弗曼树节点抽象类
template<typename E> 
class HuffmanNode {
public:
	virtual ~HuffmanNode() {}
	virtual int getWeight() = 0;		//返回权重
	virtual bool isLeaf() = 0;		//返回节点类型
	virtual E val() = 0;
	virtual HuffmanNode<E>* left() const = 0;
	virtual HuffmanNode<E>* right() const = 0;
};

//内部节点类，哈弗曼树节点的子类
template<typename E>
class intlNode :public HuffmanNode<E> {
private:
	HuffmanNode<E>* lc;			//左子树指针
	HuffmanNode<E>* rc;			//右子树指针
	int weight;
public:
	intlNode(HuffmanNode<E>* l, HuffmanNode<E>* r) {
		weight = l->getWeight() + r->getWeight();
		lc = l;
		rc = r;
	}
	~intlNode() {}
	int getWeight() { 
		return weight; 
	}
	bool isLeaf() { 
		return false; 
	}
	HuffmanNode<E>* left() const {
		return lc; 
	}
	void setLeft(HuffmanNode<E>* l) {
		lc = l;
	}
	HuffmanNode<E>* right() const { return rc; }
	void setRight(HuffmanNode<E>* r) {
		rc = r;
	}
	E val() { 
		return NULL; 
	}
};


template<typename E>		//叶节点类，哈弗曼树节点的子类
class LeafNode :public HuffmanNode<E> {
private:
	E item;
	int weight;
public:
	LeafNode(const E& val, int freq) {
		item = val; 
		weight = freq;
	}
	~LeafNode(){}
	int getWeight() { return weight; }
	E val() { return item; }
	bool isLeaf() { return true; }
	HuffmanNode<E>* left() const { return NULL; }
	HuffmanNode<E>* right() const { return NULL; }
};

//哈弗曼树类
template<typename E>
class HuffmanTree {
private:
	HuffmanNode<E>* root;		//全树的根节点
public:
	HuffmanTree(E& val, int freq) {			//叶子构造函数
		root = new LeafNode<E>(val, freq);
	}
	//内部节点构造函数
	HuffmanTree(HuffmanTree<E>* l, HuffmanTree<E>* r) {
		root = new intlNode<E>(l->getRoot(), r->getRoot());
	}
	HuffmanTree() {
		root = NULL;
	}
	~HuffmanTree() {}
	HuffmanNode<E>* getRoot() { return root; }
	int getWeight() { return root->getWeight(); }
};

//树的比较类
//写这个类是为了小顶堆类的通用性
class HuffmanTreeComp {
public:
	/*
	* @brief 判断输入的两个哈弗曼树哪个权重更大
	* @param[in] a:HuffmanTree<wchar_t>	需要比较的第一棵树
				b:HuffmanTree<wchar_t> 需要比较的第二课树
	* @return bool	命题“第一棵树的权重>第二棵树的权重”的值
	* @author Zhen Zhang
	*/
	static bool prior(HuffmanTree<char> a, HuffmanTree<char> b) {
		return a.getWeight() > b.getWeight() ? true : false;
	}
};

/*
* @brief 用森林建立哈弗曼树的函数
* @param[in] tree_array:vector<HuffmanTree<E>>	森林
			count:int 森林的大小
* @return HuffmanTree<E>*	哈弗曼树的根节点
* @author Zhen Zhang
*/
template<typename E>
HuffmanTree<E>* buildHuff(HuffmanTree<E>* tree_array, int count) {
	Heap<HuffmanTree<E>, HuffmanTreeComp>* forest =
		new Heap<HuffmanTree<E>, HuffmanTreeComp>(tree_array, count, count);
	//new heap<HuffTree<E>*, minTreeComp>(TreeArray, count, count);
	HuffmanTree<E> temp1;
	HuffmanTree<E> temp2;
	HuffmanTree<E> *temp3 = NULL;
	while (forest->getSize() > 1) {
		temp1 = forest->removeFirst();
		temp2 = forest->removeFirst();
		temp3 = new HuffmanTree<E>(&temp1, &temp2);
		forest->insert(*temp3);
	}
	return temp3;
}
vi.	macro.h:宏定义
#pragma once
#define size_file_t unsigned int	//用于表示文件大小的数据类型，单位是字节
#define MARK 0xDEADBEEF				//这是由这个程序压缩出来的压缩文件的文件标识头
b)	源文件
i.	fileIO.cpp:主要涉及文件的读写
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
ii.	FreqTable.cpp:通过读入的数据建立频数表
#include"FreqTable.h"

/*构造函数：输入字符串产生频数表并储存在类内*/
FreqTable::FreqTable(char* input_string, size_file_t length) {
	buildTableHelp(input_string, length);
}

int FreqTable::size() {
	return records.size();
}

/*私有的产生根据字符串频数表的函数*/
void FreqTable::buildTableHelp(char* input_string, size_file_t length) {
	//遍历整个字符串，构建频数表
	for (size_file_t iter_str = 0;iter_str < length;iter_str++) {
		char curr = input_string[iter_str];
		int length_vec = records.size();
		bool inTable = false;		//用来记录当前字符是否在频数表中，初始化为false
		//遍历当前的频数表，若当前字符在频数表中则对应记录的频数+1
		for (int iter_vec = 0;iter_vec < length_vec;iter_vec++) {
			if (curr == records[iter_vec].val) {
				inTable = true;
				records[iter_vec].freq += 1;
				break;
			}
		}
		//若遍历全表都未找到，则创建新的纪录并添加到表的末尾
		if (inTable == false) {
			record new_record;		//新的记录
			new_record.val = curr;
			new_record.freq = 1;
			records.push_back(new_record);
		}
	}
}

/*返回产生的码表中的所有记录（以vector形式组织起来）*/
vector<record> FreqTable::getAllRecords() {
	return records;
}
iii.	buildHuffTree.cpp:根据最小堆建Huffman树
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
iv.	compress.cpp:实现compress()函数，负责编码文件
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
v.	Decompress.cpp:实现decompress()函数，负责解码文件
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
vi.	main.cpp:主函数
#include"header_this.h"

//主函数
int main() {
	//程序介绍
	cout << "APBOHCA" << endl;
	cout << "--A Program Based On Huffman Coding Algorithm" << endl;
	cout << "1. Compress" << endl;
	cout << "2. Decompress" << endl;
	cout << "3. Quit" << endl;
	bool cont = true;	//此变量记录是否继续运行程序
	while (cont) {
		string input;
		cout << "Function number:";
		cin >> input;
		if (input == "1") {
			compress();
		}
		else if (input == "2") {
			decompress();
		}
		else if (input == "3") {
			cont = false;
		}
		else {
			cout << "Illegal input! Please type again." << endl;
		}
	}
	return 0;
}
3.	源代码地址
在以上的部分中，虽然已经将源代码展示出来，但是不方便观看，故将该项目工程存储在百度网盘中，可以通过下方链接获取。
链接：https://pan.baidu.com/s/1OwoXEj08hhL-MgxG9mBM7g 
提取码：toqy
五、测试与结论
测试条目	测试主题	测试具体操作	完成情况	备注	测试结果
1	文件的压缩	1.运行程序，选择压缩功能                2.选择代测文件life.txt                         3.进行压缩操作，等待程序完成	将原先的95kb的文档成功压缩成53kb，符合要求		成功
2	文件的解压	1.运行程序，选择解压功能               2.选择代测文件test1.txt                        3.进行解压操作，等待程序完成	将压缩的53kb的文件成功解压为原来文件，文件内容及大小没有任何变化		成功
3	特殊字符处理	1.运行程序，选择压缩功能                2.选择代测文件demo1.txt，此文件包含int，char等特殊字符                                    3.进行相关操作，等待程序完成	没有出现任何异常报错，解压后文件即为所需文件		成功
4	输入异常功能选项	1.运行程序在功能选项中输入之前没有的选项                                            2.等待程序完成	程序出现了相应报错	llegal input	成功
5	输入异常文件选项	1.运行程序在解压/压缩选项               2. 输入不存在的文件或文件路径                                    3.等待程序完成	程序出现了相应报错	file not found	成功
6	在解压中输入未压缩文件	1.运行程序，选择解压功能                2.选择未解压文件life.txt                     3.进行解压操作，等待程序完成	程序出现了相应报错	not a compressed file	成功
7	选择退出	1.运行程序在功能选项中选择退出                                               2.等待程序完成	程序退出		成功
8	解压字符较少的文件	1.运行程序，选择压缩功能                2.选择代测文件demo.txt                         3.进行压缩操作，等待程序完成	程序完成相关操作，但是压缩后文件更大	从58字节到229字节	半成功
9	压缩PDF格式文件	1.运行程序，选择压缩功能                2.选择代测文件demo.pdf                         3.进行压缩操作，等待程序完成	程序完成相关操作，但是压缩后文件更大	从77kb到79kb	半成功
10	解压PDF格式文件	1.运行程序，选择解压功能               2.选择代测文件test9.txt                        3.进行解压操作，等待程序完成
	程序完成，且与原文件相同		成功
11	压缩含中文字符文件	1.运行程序，选择压缩功能                2.选择代测文件demo.pdf                         3.进行压缩操作，等待程序完成	程序完成相关操作		成功
12	解压/压缩文件输出	1.运行程序在解压/压缩选项               2. 输入生成文件名为已存在的文件或文件路径                                            3.等待程序完成	程序报错	file write wrong	成功
项目运行截图：
 
文件压缩截图：
 
解压缩截图：
 
初始文件与压缩后解压缩文件对比：
 
 
六、项目总结
1.	列出项目具体实现的功能。
a)	实现了最小堆类、Huffman树类、内部节点类、比较类等
b)	实现了文件IO，能够判断文件是否存在
c)	能够对文件进行读写操作
d)	可以处理多种类型文件，实现对多种格式文件（TXT、PDF、JPG……）的压缩
e)	根据使用者输入的文件名，将压缩文件输出
f)	对文件进行判断，是否为编码文件；对编码文件进行解码
g)	相关报错全面，不会出现error
h)	处理了内存泄漏问题
i)	处理了中文输入，支持中文文档
2.	列出项目中你认为 “具有创新性”或“具有拓展性”的功能实现或实现思想。
a)	算法思想介绍
i.	通过二进制读写文件
ii.	根据读写的文件建立频数表
iii.	根据频数表建立Huffman森林
iv.	借助最小堆从Huffman森林中取出最小的两个数值
v.	根据取出的数值进行建立Huffman
vi.	利用栈实现对Huffman树的每支遍历，从而建立码表，借助码表实现文件压缩
vii.	根据压缩文件中的频率表重新建立Huffman树，通过文件中的数据对文件进行解码、解压缩
b)	整个项目具有原创性，基于数据结构与算法课程的二叉树、堆等数据结构知识和本门课程的C++基础知识编写的。而且整个项目都可以作为一个创新功能——加密，一般来说，没有该项目的源代码，是无法解压压缩文件的，因此整个项目可以作为加密功能使用。
3.	如果项目的功能有不足之处，请加以列出。
a)	当处理较小文件时压缩后文件较原来大，没有做到压缩的效果
b)	处理PDF文件是压缩后文件更大，详见测试9

