#include"header_this.h"

//������
int main() {
	//�������
	cout << "APBOHCA" << endl;
	cout << "--A Program Based On Huffman Coding Algorithm" << endl;
	cout << "1. Compress" << endl;
	cout << "2. Decompress" << endl;
	cout << "3. Quit" << endl;
	bool cont = true;	//�˱�����¼�Ƿ�������г���
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
