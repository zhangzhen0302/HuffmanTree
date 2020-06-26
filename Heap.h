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