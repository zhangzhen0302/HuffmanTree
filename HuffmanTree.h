#pragma once

#include"Heap.h"

using namespace std;

//���������ڵ������
template<typename E> 
class HuffmanNode {
public:
	virtual ~HuffmanNode() {}
	virtual int getWeight() = 0;		//����Ȩ��
	virtual bool isLeaf() = 0;		//���ؽڵ�����
	virtual E val() = 0;
	virtual HuffmanNode<E>* left() const = 0;
	virtual HuffmanNode<E>* right() const = 0;
};

//�ڲ��ڵ��࣬���������ڵ������
template<typename E>
class intlNode :public HuffmanNode<E> {
private:
	HuffmanNode<E>* lc;			//������ָ��
	HuffmanNode<E>* rc;			//������ָ��
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


template<typename E>		//Ҷ�ڵ��࣬���������ڵ������
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

//����������
template<typename E>
class HuffmanTree {
private:
	HuffmanNode<E>* root;		//ȫ���ĸ��ڵ�
public:
	HuffmanTree(E& val, int freq) {			//Ҷ�ӹ��캯��
		root = new LeafNode<E>(val, freq);
	}
	//�ڲ��ڵ㹹�캯��
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

//���ıȽ���
//д�������Ϊ��С�������ͨ����
class HuffmanTreeComp {
public:
	/*
	* @brief �ж�������������������ĸ�Ȩ�ظ���
	* @param[in] a:HuffmanTree<wchar_t>	��Ҫ�Ƚϵĵ�һ����
				b:HuffmanTree<wchar_t> ��Ҫ�Ƚϵĵڶ�����
	* @return bool	���⡰��һ������Ȩ��>�ڶ�������Ȩ�ء���ֵ
	* @author Zhen Zhang
	*/
	static bool prior(HuffmanTree<char> a, HuffmanTree<char> b) {
		return a.getWeight() > b.getWeight() ? true : false;
	}
};

/*
* @brief ��ɭ�ֽ������������ĺ���
* @param[in] tree_array:vector<HuffmanTree<E>>	ɭ��
			count:int ɭ�ֵĴ�С
* @return HuffmanTree<E>*	���������ĸ��ڵ�
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