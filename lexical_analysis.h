#include "struct.h"
#include <bits/stdc++.h>
using namespace std;

//词法分析
bool is_oper(string s){//判断操作符 
    for(int i=0; i<n_oper; i++) if(s == oper[i]) return true;
    return false;
}

bool is_bound(char c){//判断分界符 
    for(int i=0; i<sizeof(bound); i++) if(c == bound[i]) return true;
    return false;
}

bool is_key(string s){//判断关键字（保留字） 
    for(int i=0; i<n_key; i++) if(s == key_word[i]) return true;
    return false;
}

int my_stoi(string s){//强制转换string to int 
    int ans = 0;
    for(int i=0; i<s.size(); i++) ans = ans * 10 + s[i] - '0';
    return ans;
}

double my_stof(string s){//强制转换string to double
    long long ans = 0;
    int fd = -1, fe = -1;
    for(int i=0; i<s.size(); i++){
        if(s[i] == '.'){//记录 . 出现的位置 
            fd = i;
            continue;
        }
        if(s[i] == 'e'){//记录 e 出现的位置 
            fe = i;
            continue;
        }
        ans = ans * 10 + s[i] - '0';//得到除去小数点和e的数据 
    }
    if(fd != -1 && fe == -1) return double(ans)/(pow(10, s.size() - fd - 1));//小数点出现，但是e没有出现，普通double类数据 
    else if(fd == -1 && fe != -1){//小数点未出现，e出现， 
        long long temp = ans % (long long)pow(10, s.size() - fe - 1);     //得到e后的数字
        ans /= pow(10, s.size() - fe - 1);                          //得到e前的数字
        return double(ans*pow(10, temp));
    }
    else{//小数点和e都出现的情况 
        long long temp = ans % (long long)pow(10, s.size() - fe - 1);     //得到e后的数字
        //cout<<ans<<" "<<s.size() - fe - 1<<" "<<temp<<endl;
        ans /= pow(10, s.size() - fe - 1);                                //得到e前的数字
        //cout<<ans<<endl;
        long long tt = (s.size() - fd - 1) - (s.size() - fe - 1) - 1;     //得到.后的数字
        //cout<<tt<<endl;
        return (double)ans/pow(10, tt) * (pow(10, temp));
    }
}

void getword(){
    //freopen("programe.txt", "r", stdin);//输入重定向，输入数据将从目录下的programe.txt文件中读取 
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
	            while(isspace(c) && i<nn){   //滤空格
	                c=s[i++];
	            }
	        }
	        if(isalpha(c) || c == '_'){                         //以字母或下划线开头
	            while(isalnum(c) || c == '_' || isalpha(c)){
	                str += c;
	                c=s[i++];
	            }
	            w[++f].value = str;//Word w[maxn]; word是结构体    int f = 0, ff = 0;这里是接收到第一个完整的单词  f记录单词 
	            w[f].line=line;
	            if(is_key(str)){//如果这个是保留字 
	                w[f].id = 1;//关键字的序号设置为1 
	                if(str == "int" || str == "float" || str == "long" || str == "double" || str == "char" || str == "bool")
	                    watest.push_back(pro{str, "type", 1,line});//vector<pro> watest; 词法 
	                else watest.push_back(pro{str, str, 1,line});
	            }     
	            else{//标识符
	                w[f].id = 2;                     
	                m[str] ++;
	                w[f].line=line;
	                watest.push_back(pro{str, "id", 2,line});//标识符序号设置为2 
	            }
	            str = "";
	            flag = 1;
	        }//设置词法结构体的几个值，控制f，ff的值，词法入栈，最后str清空 
	        if(isdigit(c)){  //数字
	            int fd = 0, fe = 0, fflag = 0;
	            while(isdigit(c) || c == '.' || c == 'e'){
	                if(c == '.') fd ++;
	                if(c == 'e') fe ++;
	                if(c == '.' && fe) fflag = 1;
	                str += c;
	                c=s[i++];
	            }
	            
	            if(str[str.size()-1] == '.' || str[str.size()-1] == 'e') fflag = 1;//最后一位是小数点或者e 
	            if(fflag){
	                cout<<"在第"<<line<<"行错误-->不合法数字："<<str<<endl;    //忽略不合法输入
	            }
	            else{
	                watest.push_back(pro{str, "number", 3,line});//数组的序号的设置为3 
	                if(!fd && !fe){//两个指针都没动的话，就是普通的int
	                    n[++ff].vi = my_stoi(str);//ff记录数字个数 
	                    n[ff].id = 1;//n是数字的数组，id=1 int 
	                    n[f].line=line;
	                }
	                else{
	                    n[++ff].vd = my_stof(str);//否则就是是科学计数法 
	                    n[ff].id = 2;//id=2 科学计数 
	                    n[f].line=line;
	                }
	                w[++f].id = 3;  w[f].value = str;//记录单词 
	                w[f].line=line;
	            }
	            str = "";
	            flag = 1;
	            //cout<<s<<"\t"<<c<<endl;
	        }
	        if(is_bound(c)){ //分界符    ,  ; ( ) [ ] { } 
	            str += c;
	            w[++f].value = str;
	            w[f].line=line;
	            w[f].id = 4;// 分界符id=4 
	
	            watest.push_back(pro{str, str, 4,line});//词法记录 
	
	            //c=s[i++];
	            str = "";
	            flag = 1;
	            
	        }
	        
	        string ss = "";
		    ss += c;
		    if(is_oper(ss)){//操作符 
		        while(is_oper(ss)){//操作符可能不是一位的 
		            str += c;
		            c=s[i++];
		            ss += c;
		        }
		        i--;
		        if(is_oper(str)){
		            w[++f].value = str;
		            w[f].line=line;
		            w[f].id = 5;//操作符的id=5 
		            watest.push_back(pro{str, str, 5,line});
		        }
		        str = "";
		        flag = 1;
		    }
		    
		    
	        if(!flag && i<nn){//其他 
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
    //freopen("CON", "r", stdin);//关闭重定向,恢复标准
}

void to_file1(){ //主表
//    char filename[20];
//    puts("请输入词法分析主表要保存到的地址：");
//    scanf("%s", filename);
    fstream out;
    out.open("main_table.txt", ios::out);
    out<<"1.保留字   2.标识符   3.数字   4.分界符   5.操作符   6.其他"<<endl<<endl;
    for(int i=1; i<=f; i++) out<<w[i].id<<"  "<<w[i].value<<endl;
    out.close();
}

void to_file2(){ //标识符表
//    char filename[20];
//    puts("请输入词法分析标识符表要保存到的地址：");
//    scanf("%s", filename);
    ofstream out;
    out.open("id_table.txt", ios::out);
    map<string, int>::iterator it;
    for(it=m.begin(); it!=m.end(); it++) out<<it->first<<endl;
    out.close();
}

void to_file3(){ //数字表
//    char filename[20];
//    puts("请输入词法分析数字表要保存到的地址：");
//    scanf("%s", filename);
    ofstream out;
    out.open("number_table.txt", ios::out);
    out<<"1.int   2.double"<<endl<<endl;
    for(int i=1; i<=ff; i++) out<<n[i].id<<"  "<<(n[i].id == 1 ? n[i].vi : n[i].vd)<<endl;
    out.close();
}

void to_file4(){//词法分析结果（三元组） 例如： int  type  1   		n  id  2 	  0  number  3			while  while  1	 
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
//    puts("请输入您要打开的文件地址：");
//    scanf("%s", filename);
    getword();
    to_file1();
    to_file2();
    to_file3();
    to_file4();
	show_watest();
    return true;
}
