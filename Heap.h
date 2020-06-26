#pragma once

using namespace std;

//"С��"����
template<typename E,typename Comp>
class Heap {
private:
	E* heap;		//ָ��ѵ�ָ��
	int max_size;	//�ѵ����ߴ�
	int size;		//��ǰ���е�Ԫ������

	//������siftdown���������ڽ�Ԫ�طŵ���ȷ��λ����
	void siftdown(int pos) {
		while (!isLeaf(pos)) {
			int j = leftChild(pos);
			int rc = rightChild(pos);
			if ((rc < size) && Comp::prior(heap[j], heap[rc]))
				j = rc;		//��pos�����Һ�������С���Ǹ��ĸ���j
			if (Comp::prior(heap[j], heap[pos])) return;	//��posС��j����siftdown���
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

	//�ѡ�it�����뵽����
	void insert(E it) {
		if (size >= max_size) throw "Heap is full!";
		int curr = size++;
		heap[curr] = it;
		//memcpy_s(heap + curr, 4, &it, 4);
		//��it�����ƶ�
		while ((curr != 0) &&
			(Comp::prior(heap[parent(curr)], heap[curr]))) {
			swap(curr, parent(curr));
			curr = parent(curr);
		}
	}
	//�Ƴ���һ��Ԫ��
	E removeFirst() {
		if (size <= 0) throw "Heap is empty!";
		swap(0, --size);		//�����һ��Ԫ��siftdown
		if (size != 0) siftdown(0);		//siftdown�Բ����µĸ��ڵ�
		return heap[size];
	}
	//��������Ԫ�ص�λ��
	void swap(int pos_a,int pos_b) {
		E temp;
		temp = heap[pos_a];
		heap[pos_a] = heap[pos_b];
		heap[pos_b] = temp;
	}
}; 