#include "translation.h"
#include <bits/stdc++.h>
using namespace std;

int main(){
    if(Word_analysis()){
        puts("�ʷ��������ɹ�!");
        cout<<endl; 
        if(grammar_analysis()){
            puts("�﷨�������ɹ�!");
            cout<<endl; 
            if(translation()) puts("���룺�ɹ���");
            else puts("���룺ʧ�ܣ�");
        }
        else puts("�﷨������ʧ�ܣ�");
    }
    else puts("�ʷ�������ʧ�ܣ�");

    return 0;
}
