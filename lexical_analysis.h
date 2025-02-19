#include "struct.h"
#include <bits/stdc++.h>
using namespace std;

//�ʷ�����
bool is_oper(string s){//�жϲ����� 
    for(int i=0; i<n_oper; i++) if(s == oper[i]) return true;
    return false;
}

bool is_bound(char c){//�жϷֽ�� 
    for(int i=0; i<sizeof(bound); i++) if(c == bound[i]) return true;
    return false;
}

bool is_key(string s){//�жϹؼ��֣������֣� 
    for(int i=0; i<n_key; i++) if(s == key_word[i]) return true;
    return false;
}

int my_stoi(string s){//ǿ��ת��string to int 
    int ans = 0;
    for(int i=0; i<s.size(); i++) ans = ans * 10 + s[i] - '0';
    return ans;
}

double my_stof(string s){//ǿ��ת��string to double
    long long ans = 0;
    int fd = -1, fe = -1;
    for(int i=0; i<s.size(); i++){
        if(s[i] == '.'){//��¼ . ���ֵ�λ�� 
            fd = i;
            continue;
        }
        if(s[i] == 'e'){//��¼ e ���ֵ�λ�� 
            fe = i;
            continue;
        }
        ans = ans * 10 + s[i] - '0';//�õ���ȥС�����e������ 
    }
    if(fd != -1 && fe == -1) return double(ans)/(pow(10, s.size() - fd - 1));//С������֣�����eû�г��֣���ͨdouble������ 
    else if(fd == -1 && fe != -1){//С����δ���֣�e���֣� 
        long long temp = ans % (long long)pow(10, s.size() - fe - 1);     //�õ�e�������
        ans /= pow(10, s.size() - fe - 1);                          //�õ�eǰ������
        return double(ans*pow(10, temp));
    }
    else{//С�����e�����ֵ���� 
        long long temp = ans % (long long)pow(10, s.size() - fe - 1);     //�õ�e�������
        //cout<<ans<<" "<<s.size() - fe - 1<<" "<<temp<<endl;
        ans /= pow(10, s.size() - fe - 1);                                //�õ�eǰ������
        //cout<<ans<<endl;
        long long tt = (s.size() - fd - 1) - (s.size() - fe - 1) - 1;     //�õ�.�������
        //cout<<tt<<endl;
        return (double)ans/pow(10, tt) * (pow(10, temp));
    }
}

void getword(){
    //freopen("programe.txt", "r", stdin);//�����ض����������ݽ���Ŀ¼�µ�programe.txt�ļ��ж�ȡ 
	ifstream in("programe.txt");
    string str = "";
    int flag;
    //is_end = scanf("%c", &c);
    string s;
	int line=0;
	int i=0;
	int nn=0;
    while(getline(in, s)){
    	nn=s.size();
    	line++;
    	i=0;
    	while(i<nn){	
			c=s[i++];
	        flag = 0;
	        if(isspace(c)&&i<nn){
	            while(isspace(c) && i<nn){   //�˿ո�
	                c=s[i++];
	            }
	        }
	        if(isalpha(c) || c == '_'){                         //����ĸ���»��߿�ͷ
	            while(isalnum(c) || c == '_' || isalpha(c)){
	                str += c;
	                c=s[i++];
	            }
	            w[++f].value = str;//Word w[maxn]; word�ǽṹ��    int f = 0, ff = 0;�����ǽ��յ���һ�������ĵ���  f��¼���� 
	            w[f].line=line;
	            if(is_key(str)){//�������Ǳ����� 
	                w[f].id = 1;//�ؼ��ֵ��������Ϊ1 
	                if(str == "int" || str == "float" || str == "long" || str == "double" || str == "char" || str == "bool")
	                    watest.push_back(pro{str, "type", 1,line});//vector<pro> watest; �ʷ� 
	                else watest.push_back(pro{str, str, 1,line});
	            }     
	            else{//��ʶ��
	                w[f].id = 2;                     
	                m[str] ++;
	                w[f].line=line;
	                watest.push_back(pro{str, "id", 2,line});//��ʶ���������Ϊ2 
	            }
	            str = "";
	            flag = 1;
	        }//���ôʷ��ṹ��ļ���ֵ������f��ff��ֵ���ʷ���ջ�����str��� 
	        if(isdigit(c)){  //����
	            int fd = 0, fe = 0, fflag = 0;
	            while(isdigit(c) || c == '.' || c == 'e'){
	                if(c == '.') fd ++;
	                if(c == 'e') fe ++;
	                if(c == '.' && fe) fflag = 1;
	                str += c;
	                c=s[i++];
	            }
	            
	            if(str[str.size()-1] == '.' || str[str.size()-1] == 'e') fflag = 1;//���һλ��С�������e 
	            if(fflag){
	                cout<<"�ڵ�"<<line<<"�д���-->���Ϸ����֣�"<<str<<endl;    //���Բ��Ϸ�����
	            }
	            else{
	                watest.push_back(pro{str, "number", 3,line});//�������ŵ�����Ϊ3 
	                if(!fd && !fe){//����ָ�붼û���Ļ���������ͨ��int
	                    n[++ff].vi = my_stoi(str);//ff��¼���ָ��� 
	                    n[ff].id = 1;//n�����ֵ����飬id=1 int 
	                    n[f].line=line;
	                }
	                else{
	                    n[++ff].vd = my_stof(str);//��������ǿ�ѧ������ 
	                    n[ff].id = 2;//id=2 ��ѧ���� 
	                    n[f].line=line;
	                }
	                w[++f].id = 3;  w[f].value = str;//��¼���� 
	                w[f].line=line;
	            }
	            str = "";
	            flag = 1;
	            //cout<<s<<"\t"<<c<<endl;
	        }
	        if(is_bound(c)){ //�ֽ��    ,  ; ( ) [ ] { } 
	            str += c;
	            w[++f].value = str;
	            w[f].line=line;
	            w[f].id = 4;// �ֽ��id=4 
	
	            watest.push_back(pro{str, str, 4,line});//�ʷ���¼ 
	
	            //c=s[i++];
	            str = "";
	            flag = 1;
	            
	        }
	        
	        string ss = "";
		    ss += c;
		    if(is_oper(ss)){//������ 
		        while(is_oper(ss)){//���������ܲ���һλ�� 
		            str += c;
		            c=s[i++];
		            ss += c;
		        }
		        i--;
		        if(is_oper(str)){
		            w[++f].value = str;
		            w[f].line=line;
		            w[f].id = 5;//��������id=5 
		            watest.push_back(pro{str, str, 5,line});
		        }
		        str = "";
		        flag = 1;
		    }
		    
		    
	        if(!flag && i<nn){//���� 
	            str += c;
	            w[++f].value = str;
	            w[f].line=line;
	            w[f].id = 6;
	            watest.push_back(pro{str, str, 6,line});
	            c=s[i++];
	            str = "";
	        }
	        
	    }
    }
    //freopen("CON", "r", stdin);//�ر��ض���,�ָ���׼
}

void to_file1(){ //����
//    char filename[20];
//    puts("������ʷ���������Ҫ���浽�ĵ�ַ��");
//    scanf("%s", filename);
    fstream out;
    out.open("main_table.txt", ios::out);
    out<<"1.������   2.��ʶ��   3.����   4.�ֽ��   5.������   6.����"<<endl<<endl;
    for(int i=1; i<=f; i++) out<<w[i].id<<"  "<<w[i].value<<endl;
    out.close();
}

void to_file2(){ //��ʶ����
//    char filename[20];
//    puts("������ʷ�������ʶ����Ҫ���浽�ĵ�ַ��");
//    scanf("%s", filename);
    ofstream out;
    out.open("id_table.txt", ios::out);
    map<string, int>::iterator it;
    for(it=m.begin(); it!=m.end(); it++) out<<it->first<<endl;
    out.close();
}

void to_file3(){ //���ֱ�
//    char filename[20];
//    puts("������ʷ��������ֱ�Ҫ���浽�ĵ�ַ��");
//    scanf("%s", filename);
    ofstream out;
    out.open("number_table.txt", ios::out);
    out<<"1.int   2.double"<<endl<<endl;
    for(int i=1; i<=ff; i++) out<<n[i].id<<"  "<<(n[i].id == 1 ? n[i].vi : n[i].vd)<<endl;
    out.close();
}

void to_file4(){//�ʷ������������Ԫ�飩 ���磺 int  type  1   		n  id  2 	  0  number  3			while  while  1	 
    ofstream out;
    out.open("twox.txt", ios::out); 
    int len = watest.size();
    for(int i=0; i<len-1; i++) out<<watest[i].value<<"  "<<watest[i].type<<"  "<<watest[i].number<<endl;
    out<<watest[len-1].value<<"  "<<watest[len-1].type<<"  "<<watest[len-1].number<<endl;
    out.close();
}

void show_watest(){
	ofstream out;
    out.open("watest.txt", ios::out); 
    out<<"value"<<"\t"<<"type"<<"\t"<<"number"<<"\t"<<"line"<<endl;
	for(auto it=watest.begin();it!=watest.end();it++){
		out<<(*it).value<<"\t"<<(*it).type<<"\t"<<(*it).number<<"\t"<<(*it).line<<endl;
	}
	out.close();
}

bool Word_analysis(){
//    char filename[20];
//    puts("��������Ҫ�򿪵��ļ���ַ��");
//    scanf("%s", filename);
    getword();
    to_file1();
    to_file2();
    to_file3();
    to_file4();
	show_watest();
    return true;
}
