#include <bits/stdc++.h>
using namespace std;

const int maxn = 1002;
const int n_key = 41; //��������Ŀ
const int n_oper = 22;//��������Ŀ
char c;

//������
string key_word[n_key] = {"auto", "enum", "restrict", "unsigned", "break",
                    "extern", "return", "void", "case", "float",
                    "short", "volatile", "char", "for", "signed",
                    "while", "const", "goto", "sizeof", "_Bool",
                    "continue", "if", "static", "_Complex", "default",
                    "inline", "struct", "_Imaginary", "do", "int",
                    "switch", "double", "long", "typedef", "else",
                    "register", "union", "scanf", "printf", "cin",
                    "cout"
};

//������
string oper[] = {"+", "-", "*", "/", "^",
                "<", ">", "++", "--", "==",
                "*=", "/=", ">=", "<=", "<<",
                ">>", ">>=", "<<=", "%", "&",
                "^","="
};

char bound[] = {',', ';', '(', ')', '[', ']', '{', '}'};

//�ʽṹ��
struct Word{
    int id;
    string value;
    int line;
};

//���ֽṹ��
struct Num{
    int id;
    int vi;
    double vd;
    int line;
};

Num n[maxn];            //����
Word w[maxn];           //��
map<string, int> m;     //��ʶ��
int f = 0, ff = 0;		//��¼ ������|��ʶ��   ��  ���� 

//�ʷ�
typedef struct node{
    string value;
    string type;
    int number;
    int line;
}pro;
vector<pro> watest;

//�﷨
typedef pair<string, string> P;
map<P, string> pre_list;//Ԥ��� 
typedef set<string> Sets;//���� 
map<string, Sets> first, follow, wenfa, select; //first, follow
Sets Term, Nterm;//�ռ��������ռ���
string Start;//�ʷ�������һ�� 

//����
int cnt = 0;//��Ԫʽ��� 
pro now, last;//��ǰ��֮ǰ�Ĵʷ� 
vector<string> var;//�������ı� 
struct Four_exp{   //��Ԫʽ�ṹ��
    string op;
    string r1, r2;
    string left;
};
vector<Four_exp> four_exp, assemble;//��Ԫʽ�����ָ���
int cnt_temp = 0;//
map<string, int> pri;//���ȼ�����
