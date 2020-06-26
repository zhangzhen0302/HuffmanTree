#include"FreqTable.h"

/*���캯���������ַ�������Ƶ��������������*/
FreqTable::FreqTable(char* input_string, size_file_t length) {
	buildTableHelp(input_string, length);
}

int FreqTable::size() {
	return records.size();
}

/*˽�еĲ��������ַ���Ƶ����ĺ���*/
void FreqTable::buildTableHelp(char* input_string, size_file_t length) {
	//���������ַ���������Ƶ����
	for (size_file_t iter_str = 0;iter_str < length;iter_str++) {
		char curr = input_string[iter_str];
		int length_vec = records.size();
		bool inTable = false;		//������¼��ǰ�ַ��Ƿ���Ƶ�����У���ʼ��Ϊfalse
		//������ǰ��Ƶ��������ǰ�ַ���Ƶ���������Ӧ��¼��Ƶ��+1
		for (int iter_vec = 0;iter_vec < length_vec;iter_vec++) {
			if (curr == records[iter_vec].val) {
				inTable = true;
				records[iter_vec].freq += 1;
				break;
			}
		}
		//������ȫ��δ�ҵ����򴴽��µļ�¼����ӵ����ĩβ
		if (inTable == false) {
			record new_record;		//�µļ�¼
			new_record.val = curr;
			new_record.freq = 1;
			records.push_back(new_record);
		}
	}
}

/*���ز���������е����м�¼����vector��ʽ��֯������*/
vector<record> FreqTable::getAllRecords() {
	return records;
}