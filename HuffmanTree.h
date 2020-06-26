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