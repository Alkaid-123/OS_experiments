#include<bits/stdc++.h>
#include "windows.h"
using namespace std;

int main(int argc, char* argv[]){
    vector<vector<string>>a;
    vector<string>s;
    s.push_back("hello");
    s.push_back("world");
    a.push_back(s);
    a.push_back(s);
    cout<<a[0][0];
    	cout << "ԭɫtestCOLOR��û������������ɫ��" << endl;
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED |
		FOREGROUND_GREEN | FOREGROUND_BLUE);//������ɫ���
	cout << "��ɫtestCOLOR����ɫ��ɫ��ɫ��ӣ�" << endl;
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED);
	//���ú�ɫ
	cout << "��ɫtestCOLOR�����õ���ɫΪ��ɫ��" << endl;
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_GREEN)
		;//������ɫ
	cout << "��ɫtestCOLOR�����õ���ɫΪ��ɫ��" << endl;
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_BLUE);
	//������ɫ
	cout << "��ɫtestCOLOR�����õ���ɫΪ��ɫ��" << endl;
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED |
		FOREGROUND_GREEN);//���ú�ɫ����ɫ���
	cout << "��ɫtestCOLOR����ɫ����ɫ���ɫ��" << endl;
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED |
		FOREGROUND_BLUE);//���ú�ɫ����ɫ���
	cout << "��ɫtestCOLOR����ɫ����ɫ���ɫ��" << endl;
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_GREEN |
		FOREGROUND_BLUE);//������ɫ����ɫ���
	cout << "��ɫtestCOLOR����ɫ����ɫ���ɫ��" << endl;
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY);
	//������ɫ��û�������ɫ����Ϊԭɫ
	cout << endl;


	for(int i=0;i<=109;i++){
        cout  <<i<< " \033["<<i<<"mhello world\033[0m"<< endl ;
	}

	system("pause");

    return 0;
}
