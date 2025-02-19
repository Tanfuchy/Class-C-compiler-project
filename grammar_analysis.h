#include "lexical_analysis.h"
#include <bits/stdc++.h>
using namespace std;

//LL(1)�﷨����
void get_wenfa(){
    freopen("grammar.txt", "r", stdin);//�ض��� 

    string line, s, foo;
    int cnt = 0;
    while(getline(cin, line)){//��ȡһ�����룬ֱ�����ﻻ�з� 
        cnt ++;//��¼���� 
        int len = line.size();//ȡ�ַ����ĳ��� 
        int i = 0;
        while(line[i] != ' ' && i < len) i ++;//��⵽��һ���ո�ֹͣ 
        foo = line.substr(0, i);//��0��ʼ��ȡline��iλ �����õ��Ƶ�ʽ  -> ��ߵ����� 
        if(cnt == 1) Start = foo;//��Ϊ��Ҫ��#����follow(Start)
        i++;//��ǰΪ�ո���������һ����Ϊ�ո�� 
        while(line[i] != ' ' && i < len) i ++;   //��ȥ ->   sΪ -> ��������� 
        for(; i<len; i++){
            if(line[i] != ' '){
                s = "";
                while(!(line[i] == '|' && line[i-1] == ' ' && line[i+1] == ' ') && i < len){
                    s += line[i++];
                }
                while(s[s.size()-1] == ' ') s.erase(s.size()-1, 1);//ɾ��ĩβ�Ŀո� 
                wenfa[foo].insert(s);//���뵽�ķ��� 
            }
        }
    }//���磺 <��������> -> <����> <����> ( <��������> ) { <������> }  foo: <��������>   s: <����> <����> ( <��������> ) { <������> }
    
//    for(map<string, Sets>::iterator it = wenfa.begin(); it != wenfa.end(); it ++){    //  ����ķ�
//        cout<<it->first<<"->";
//        for(Sets::iterator ti = (it->second).begin(); ti != (it->second).end(); ti ++){
//            if(ti == it->second.begin()) cout<<*ti;
//            else cout<<" | "<<*ti;
//        }
//        puts("");
//    }
    
    freopen("CON", "r", stdin);//�ر��ض���,�ָ���׼
}

void get_ch(){
    fstream in;
    in.open("grammar.txt", ios::in);//���ķ� 

    string str;
    while(in >> str){
        if(str == "->") continue;//���� -> 
        if(str[0] == '<' && str[str.size()-1] == '>') Nterm.insert(str);//�����<>��ʽ���Ƿ��ռ��� 
        else Term.insert(str);//��������ռ��� 
    }
    Term.insert("#");
    in.close();
}

void get_first(){
    int flag = 1;
    Sets tt;
    while(flag){
        flag = 0;
        for(map<string, Sets>::iterator it = wenfa.begin(); it != wenfa.end(); it ++){    //���з��ռ���
            string X = it->first;       //���ڷ��ռ���X

            if(wenfa[X].count("@") && !first[X].count("@")){   //count����ָ��Ԫ�س��ֵĴ���,����@ �գ�����FIRST
                first[X].insert("@");
                flag = 1;
            }

            for(Sets::iterator f = wenfa[X].begin(); f != wenfa[X].end(); f ++){    //����s���в���ʽ
                string foo = *f, str = "", last = "";                               //���ڲ���ʽX->Y1Y2Y3...

                int i = 0;
                while(foo[i] != ' ' && i < foo.size()) str += foo[i ++];    //�ҵ�Y1
                if(str == "") continue;

                if(!(str[0] == '<' && str[str.size()-1] == '>')){ //��ʽ����Ϊ��<Y1>  ���� Y1�����ռ���
                    if(!first[X].count(str)){//���first����û���������� 
                        first[X].insert(str);
                        flag = 1;
                    }
                    if(str != "@") continue;      //���ռ���������@
                }
                else{ //Y1�Ƿ��ռ���
                    for(Sets::iterator ii = first[str].begin(); ii != first[str].end(); ii ++){//��Y1��first����X 
                        if(*ii != "@" && !first[X].count(*ii)){
                            first[X].insert(*ii);
                            flag = 1;
                        }
                    }
                    if(!first[str].count("@")) continue;
                }

                for( ; i<foo.size(); i++){
                    if(foo[i] != ' '){
                        last = str;
                        str = "";
                        while(foo[i] != ' ' && i < foo.size()) str += foo[i ++];//��ȡY2 

                        if(last == "@" || first[last].count("@")){//���Y1Ϊ�ջ���Y1��first��Ϊ�� 
                            for(Sets::iterator ii = first[str].begin(); ii != first[str].end(); ii ++){
                                if(*ii != "@" && !first[X].count(*ii)){//���Y2��first�����ǿգ�����Y1��first����û�д�Ÿ��ռ������߷��ռ��� 
                                    first[X].insert(*ii);//���뵽��ǰ���ռ�����first���� 
                                    flag = 1;
                                }
                            }
                        }
                        else break;
                    }
                }

                if(i >= foo.size() && (str == "@" || first[str].count("@"))){  //Y1Y2Y3...Yn��FIRST���ɲ���@
                    if(!first[X].count("@")){
                        first[X].insert("@");
                        flag = 1;
                    }
                }
            }
        }
    }

//���FIRST��
    fstream out;
    out.open("FIRST.txt", ios::out);//�������FIRST
    for(map<string, Sets>::iterator t = first.begin(); t != first.end(); t ++){
        out<<"FIRST("<<t->first<<"): ";
        for(Sets::iterator it = t->second.begin(); it != t->second.end(); it ++)
            out<<" "<<*it;
        out<<endl;
    }
    out.close();
}

void get_follow(){
    int flag = 1;
    int cnt;
    string str, foo, s, last, tr;
    Sets tt;

    follow[Start].insert("#");//��#����follow(Start)

    while(flag){
        flag = 0;
        for(map<string, Sets>::iterator it = wenfa.begin(); it != wenfa.end(); it ++){
            str = it->first;//����FOLLOW(str)

            for(Sets::iterator ti = it->second.begin(); ti != it->second.end(); ti ++){
                foo = *ti;//���ڲ���ʽstr->foo������Ϊstr->ABC

                int len = foo.size(), i = 0;
                s = "";
                while(foo[i] != ' ' && i < len){ s += foo[i ++]; }//��ȡ�Ƶ�ʽ
                if(s == "") continue;

                for( ; i<len; i++){
                    if(foo[i] != ' '){
                        last = s;
                        s = "";
                        while(foo[i] != ' ' && i < len){ s += foo[i ++]; }//��ȡ�ڶ��� 
                        if(last[0] != '<' || last[last.size()-1] != '>') continue;//A���ս�������������ȡ 

                        if(s[0] != '<' || s[s.size()-1] != '>'){//B���ս��
                            if(s != "@" && !follow[last].count(s)){//B���ǿ���A��follow��û�����B 
                                follow[last].insert(s);//��B��Ӹ�A��follow��
                                flag = 1;
                            }
                            if(s != "@") continue;
                        }
                        else{//B�Ƿ��ս�����Ȱ�first(B) - {@}���뵽A��follow 
                            for(Sets::iterator ii = first[s].begin(); ii != first[s].end(); ii ++){
                                if(*ii != "@" && !follow[last].count(*ii)){
                                    follow[last].insert(*ii);
                                    flag = 1;
                                }
                            }
                        }

                        if(first[s].count("@") || s == "@"){//���B��first�����ڿգ�������� 
                            string next = s;
                            int k = i;//��¼��ȡλ�ã���ֹ�����������ȡ 
                            for( ; k < len; k++){
                                if(foo[k] != ' '){
                                    next = "";
                                    while(foo[k] != ' ' && k < len){ next += foo[k ++]; }//��������ȡ 

                                    if(next[0] != '<' || next[next.size()-1] != '>'){//next���ս��
                                        if(next != "@" && !follow[last].count(next)){//���ռ�����A��follow 
                                            follow[last].insert(next);
                                            flag = 1;
                                        }
                                        if(next != "@") break;
                                    }
                                    else{ //next�Ƿ��ս��
                                        for(Sets::iterator ii = first[next].begin(); ii != first[next].end(); ii ++){//��next��first��A��follow 
                                            if(*ii != "@" && !follow[last].count(*ii)){
                                                follow[last].insert(*ii);
                                                flag = 1;
                                            }
                                        }
                                        if(!first[next].count("@")) break;
                                    }
                                }
                            }
                            if(k >= len && (next == "@" || first[next].count("@"))){ //�������ķ��ռ��������Ƶ����գ���follow(str) - {@}�ӵ�follow(A)��
                                for(Sets::iterator ii = follow[str].begin(); ii != follow[str].end(); ii ++){
                                    if(!follow[last].count(*ii)){
                                        follow[last].insert(*ii);
                                        flag = 1;
                                    }
                                }
                            }
                        }
                    }
                }
                if(i >= len && (s[0] == '<' && s[s.size()-1] == '>')){//����str->ABC,�����follow(str) - {@}�ӵ�follow(C)��
                    for(Sets::iterator ii = follow[str].begin(); ii != follow[str].end(); ii ++){
                        if(!follow[s].count(*ii)){
                            follow[s].insert(*ii);
                            flag = 1;
                        }
                    }
                }
            }
        }
    }

//���FOLLOW��
    fstream out;
    out.open("FOLLOW.txt", ios::out);
    for(map<string, Sets>::iterator t = follow.begin(); t != follow.end(); t ++){
        out<<"FOLLOW("<<t->first<<"): ";
        for(Sets::iterator it = t->second.begin(); it != t->second.end(); it ++)
            out<<" "<<*it;
        out<<endl;
    }
    out.close();
}

void get_select(){
    int flag = 1;
    string str, foo, s, fxx;
    Sets tt;
    for(map<string, Sets>::iterator it = wenfa.begin(); it != wenfa.end(); it ++){
        str = it->first;//����str

        for(Sets::iterator ti = it->second.begin(); ti != it->second.end(); ti ++){
            foo = *ti;
            fxx = str + " " + foo;//����select(str->foo)

            tt.clear();
            int len = foo.size();
            for(int i=0; i<len; i++){
                if(foo[i] != ' '){
                    s = "";
                    while(foo[i] != ' ' && i < len){ s += foo[i ++]; }//��ȡ�ķ� 
                    if(s[0] != '<' || s[s.size()-1] != '>'){//s���ս��
                        if(s != "@"){
                            tt.insert(s);//��Ϊ�ղ���tt 
                            break;
                        }
                    }else{//s�Ƿ��ռ��� 
                        for(Sets::iterator ii = first[s].begin(); ii != first[s].end(); ii ++){
                            if(*ii != "@") tt.insert(*ii);//����s��first�� 
                        }
                        if(!first[s].count("@")) break;
                    }
                }
            }
            if(s == "@" || first[s].count("@")){//set_union:���������ϵĲ���
                set_union(select[fxx].begin(), select[fxx].end(), follow[str].begin(), follow[str].end(), inserter(select[fxx], select[fxx].begin()));
                set_union(select[fxx].begin(), select[fxx].end(), tt.begin(), tt.end(), inserter(select[fxx], select[fxx].begin()));
            }
            else set_union(tt.begin(), tt.end(), select[fxx].begin(), select[fxx].end(), inserter(select[fxx], select[fxx].begin()));
        }
    }

//���SELECT��
    fstream out;
    out.open("SELECT.txt", ios::out);
    for(map<string, Sets>::iterator t = select.begin(); t != select.end(); t ++){
        out<<"SELECT("<<t->first<<"): ";
        for(Sets::iterator it = t->second.begin(); it != t->second.end(); it ++)
            out<<" "<<*it;
        out<<endl;
    }
    out.close();
}

void get_pre_list(){//Ԥ��� 
    get_ch();//���ķ�������Nterm��Term 
    pre_list.clear();//��� 
    string str, foo;
    for(map<string, Sets>::iterator it = select.begin(); it != select.end(); it ++){
        int i = 0;
        str = foo = "";
        while(it->first[i] != ' ' && i < it->first.size()) str += it->first[i ++];//��ȡselect��map�������͵�string 
        foo = it->first.substr(i+1, it->first.size() - i - 1);//���� str->foo 

        for(Sets::iterator ti = it->second.begin(); ti != it->second.end(); ti ++){
            pre_list[P(str ,*ti)] = foo;//дԤ��� 
        }
    }
    fstream out;//��� 
    out.open("pre_list.txt", ios::out);

    out<<"        ";
    for(Sets::iterator ti = Term.begin(); ti != Term.end(); ti ++){
        out<<setw(8)<<*ti;
    }
    out<<endl;
    for(Sets::iterator it = Nterm.begin(); it != Nterm.end(); it ++){
        out<<setw(8)<<*it;
        for(Sets::iterator ti = Term.begin(); ti != Term.end(); ti ++){
            out<<setw(8)<<(pre_list[P(*it, *ti)] == "" ? "$" : pre_list[P(*it, *ti)]);
        }
        out<<endl;
    }
    out.close();
}

bool analysis(ofstream &out){//��������  ����� process.txt�ļ��� 
    vector<string> p;
    string t, foo, str, fxx;

    p.push_back("#");//Ԥ����
    p.push_back(Start);
    watest.push_back(pro{"#", "#", 0});

    out<<"  ���� |    ��ǰջ��    |     ��ǰ����     |   �Ƶ����ò���ʽ                 \n";
    out<<"------------------------------------------------------------------------------\n";
    int step = 1, pos = 0;
    while(!p.empty() && pos < watest.size()){//����������ջΪ�ջ��ߴʷ�ʶ���� 
        out<<setw(6)<<(step ++)<<setw(18)<<p.back()<<setw(19)<<watest[pos].type;
        if(p.back() == "#" && watest[pos].type == "#"){//��ǰջ�ʹ�����#˵��ʶ������� 
            out<<"    "<<"����"<<endl;
            return true;
        }

        if(p.back() == watest[pos].type){
            out<<"    "<<"��"<<watest[pos].type<<"��"<<"ƥ��"<<endl;//������ͬ��ƥ�� 
            p.pop_back();//��ջ 
            pos ++;
        }
        else{
            t = pre_list[P(p.back(), watest[pos].type)];
            if(t == "") {
				cout<<"ջ�ı�ʶ��ͬ������! \t"<<"��ǰջ��Ϊ��"<< p.back()<<"\t"<<"��ǰ����Ϊ��"<< watest[pos].type<<endl;
				cout<<"����Ϊ��"<<watest[pos].line<<"��,�õ��ʻ����Ϊ��"<<watest[pos].value<<"\t ����id_number: "<<watest[pos].number<<endl;
				cout<<"������һ��Ϊ"<<watest[pos+1].line<<"��,�õ��ʻ����Ϊ��"<<watest[pos+1].value<<"\t ����id_number: "<<watest[pos+1].number<<endl;
				return false;//Ԥ������ޣ������ 
			}
            out<<"    "<<p.back()<<" -> "<<t<<endl;
            if(t == "@") t = "";

            p.pop_back();

            int i = t.size()-1;
            for( ; i >= 0; i--){
                if(t[i] != ' '){
                    foo = "";
                    while(t[i] != ' ' && i >= 0) foo += t[i --];
                    reverse(foo.rbegin(), foo.rend());
                    p.push_back(foo);
                }
            }
        }
    }
    return false;
}

bool grammar_analysis(){
    get_wenfa();
    get_first();
    get_follow();
    get_select();
    get_pre_list();

//�����������
    ofstream out;
    out.open("process.txt", ios::out);
    bool flag = analysis(out);
    out<<(flag ? "\n\n-->LL(1)�Ϸ�����\n\n" : "\n\n-->LL(1)�Ƿ�����\n\n");
    out.close();
    return flag;
}
