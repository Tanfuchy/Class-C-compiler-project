#include "lexical_analysis.h"
#include <bits/stdc++.h>
using namespace std;

//LL(1)语法分析
void get_wenfa(){
    freopen("grammar.txt", "r", stdin);//重定向 

    string line, s, foo;
    int cnt = 0;
    while(getline(cin, line)){//读取一行输入，直到到达换行符 
        cnt ++;//记录行数 
        int len = line.size();//取字符串的长度 
        int i = 0;
        while(line[i] != ' ' && i < len) i ++;//检测到第一个空格停止 
        foo = line.substr(0, i);//从0开始截取line的i位 ，即得到推导式  -> 左边的内容 
        if(cnt == 1) Start = foo;//因为需要把#加入follow(Start)
        i++;//当前为空格，自增到下一个不为空格的 
        while(line[i] != ' ' && i < len) i ++;   //略去 ->   s为 -> 后面的内容 
        for(; i<len; i++){
            if(line[i] != ' '){
                s = "";
                while(!(line[i] == '|' && line[i-1] == ' ' && line[i+1] == ' ') && i < len){
                    s += line[i++];
                }
                while(s[s.size()-1] == ' ') s.erase(s.size()-1, 1);//删除末尾的空格 
                wenfa[foo].insert(s);//插入到文法中 
            }
        }
    }//例如： <函数定义> -> <类型> <变量> ( <参数声明> ) { <函数块> }  foo: <函数定义>   s: <类型> <变量> ( <参数声明> ) { <函数块> }
    
//    for(map<string, Sets>::iterator it = wenfa.begin(); it != wenfa.end(); it ++){    //  输出文法
//        cout<<it->first<<"->";
//        for(Sets::iterator ti = (it->second).begin(); ti != (it->second).end(); ti ++){
//            if(ti == it->second.begin()) cout<<*ti;
//            else cout<<" | "<<*ti;
//        }
//        puts("");
//    }
    
    freopen("CON", "r", stdin);//关闭重定向,恢复标准
}

void get_ch(){
    fstream in;
    in.open("grammar.txt", ios::in);//读文法 

    string str;
    while(in >> str){
        if(str == "->") continue;//忽略 -> 
        if(str[0] == '<' && str[str.size()-1] == '>') Nterm.insert(str);//如果是<>格式则是非终极符 
        else Term.insert(str);//否则就是终极符 
    }
    Term.insert("#");
    in.close();
}

void get_first(){
    int flag = 1;
    Sets tt;
    while(flag){
        flag = 0;
        for(map<string, Sets>::iterator it = wenfa.begin(); it != wenfa.end(); it ++){    //所有非终极符
            string X = it->first;       //对于非终极符X

            if(wenfa[X].count("@") && !first[X].count("@")){   //count返回指定元素出现的次数,包含@ 空，加入FIRST
                first[X].insert("@");
                flag = 1;
            }

            for(Sets::iterator f = wenfa[X].begin(); f != wenfa[X].end(); f ++){    //遍历s所有产生式
                string foo = *f, str = "", last = "";                               //对于产生式X->Y1Y2Y3...

                int i = 0;
                while(foo[i] != ' ' && i < foo.size()) str += foo[i ++];    //找到Y1
                if(str == "") continue;

                if(!(str[0] == '<' && str[str.size()-1] == '>')){ //格式不是为：<Y1>  而是 Y1则是终极符
                    if(!first[X].count(str)){//如果first集中没有添加则添加 
                        first[X].insert(str);
                        flag = 1;
                    }
                    if(str != "@") continue;      //是终极符但不是@
                }
                else{ //Y1是非终极符
                    for(Sets::iterator ii = first[str].begin(); ii != first[str].end(); ii ++){//将Y1的first集给X 
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
                        while(foo[i] != ' ' && i < foo.size()) str += foo[i ++];//读取Y2 

                        if(last == "@" || first[last].count("@")){//如果Y1为空或者Y1的first集为空 
                            for(Sets::iterator ii = first[str].begin(); ii != first[str].end(); ii ++){
                                if(*ii != "@" && !first[X].count(*ii)){//如果Y2的first集不是空，并且Y1的first集中没有存放该终极符或者非终极符 
                                    first[X].insert(*ii);//插入到当前非终极符的first集中 
                                    flag = 1;
                                }
                            }
                        }
                        else break;
                    }
                }

                if(i >= foo.size() && (str == "@" || first[str].count("@"))){  //Y1Y2Y3...Yn的FIRST都可产生@
                    if(!first[X].count("@")){
                        first[X].insert("@");
                        flag = 1;
                    }
                }
            }
        }
    }

//输出FIRST集
    fstream out;
    out.open("FIRST.txt", ios::out);//输出保存FIRST
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

    follow[Start].insert("#");//把#加入follow(Start)

    while(flag){
        flag = 0;
        for(map<string, Sets>::iterator it = wenfa.begin(); it != wenfa.end(); it ++){
            str = it->first;//对于FOLLOW(str)

            for(Sets::iterator ti = it->second.begin(); ti != it->second.end(); ti ++){
                foo = *ti;//对于产生式str->foo即举例为str->ABC

                int len = foo.size(), i = 0;
                s = "";
                while(foo[i] != ' ' && i < len){ s += foo[i ++]; }//读取推导式
                if(s == "") continue;

                for( ; i<len; i++){
                    if(foo[i] != ' '){
                        last = s;
                        s = "";
                        while(foo[i] != ' ' && i < len){ s += foo[i ++]; }//读取第二个 
                        if(last[0] != '<' || last[last.size()-1] != '>') continue;//A是终结符，继续往后读取 

                        if(s[0] != '<' || s[s.size()-1] != '>'){//B是终结符
                            if(s != "@" && !follow[last].count(s)){//B不是空且A的follow集没有添加B 
                                follow[last].insert(s);//把B添加给A的follow集
                                flag = 1;
                            }
                            if(s != "@") continue;
                        }
                        else{//B是非终结符，先把first(B) - {@}加入到A的follow 
                            for(Sets::iterator ii = first[s].begin(); ii != first[s].end(); ii ++){
                                if(*ii != "@" && !follow[last].count(*ii)){
                                    follow[last].insert(*ii);
                                    flag = 1;
                                }
                            }
                        }

                        if(first[s].count("@") || s == "@"){//如果B的first集存在空，看后面的 
                            string next = s;
                            int k = i;//记录读取位置，防止干扰主程序读取 
                            for( ; k < len; k++){
                                if(foo[k] != ' '){
                                    next = "";
                                    while(foo[k] != ' ' && k < len){ next += foo[k ++]; }//继续向后读取 

                                    if(next[0] != '<' || next[next.size()-1] != '>'){//next是终结符
                                        if(next != "@" && !follow[last].count(next)){//将终极符给A的follow 
                                            follow[last].insert(next);
                                            flag = 1;
                                        }
                                        if(next != "@") break;
                                    }
                                    else{ //next是非终结符
                                        for(Sets::iterator ii = first[next].begin(); ii != first[next].end(); ii ++){//把next的first给A的follow 
                                            if(*ii != "@" && !follow[last].count(*ii)){
                                                follow[last].insert(*ii);
                                                flag = 1;
                                            }
                                        }
                                        if(!first[next].count("@")) break;
                                    }
                                }
                            }
                            if(k >= len && (next == "@" || first[next].count("@"))){ //如果后面的非终极符都能推导出空，则将follow(str) - {@}加到follow(A)中
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
                if(i >= len && (s[0] == '<' && s[s.size()-1] == '>')){//对于str->ABC,最后则将follow(str) - {@}加到follow(C)中
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

//输出FOLLOW集
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
        str = it->first;//对于str

        for(Sets::iterator ti = it->second.begin(); ti != it->second.end(); ti ++){
            foo = *ti;
            fxx = str + " " + foo;//对于select(str->foo)

            tt.clear();
            int len = foo.size();
            for(int i=0; i<len; i++){
                if(foo[i] != ' '){
                    s = "";
                    while(foo[i] != ' ' && i < len){ s += foo[i ++]; }//读取文法 
                    if(s[0] != '<' || s[s.size()-1] != '>'){//s是终结符
                        if(s != "@"){
                            tt.insert(s);//不为空插入tt 
                            break;
                        }
                    }else{//s是非终极符 
                        for(Sets::iterator ii = first[s].begin(); ii != first[s].end(); ii ++){
                            if(*ii != "@") tt.insert(*ii);//插入s的first集 
                        }
                        if(!first[s].count("@")) break;
                    }
                }
            }
            if(s == "@" || first[s].count("@")){//set_union:求两个集合的并集
                set_union(select[fxx].begin(), select[fxx].end(), follow[str].begin(), follow[str].end(), inserter(select[fxx], select[fxx].begin()));
                set_union(select[fxx].begin(), select[fxx].end(), tt.begin(), tt.end(), inserter(select[fxx], select[fxx].begin()));
            }
            else set_union(tt.begin(), tt.end(), select[fxx].begin(), select[fxx].end(), inserter(select[fxx], select[fxx].begin()));
        }
    }

//输出SELECT集
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

void get_pre_list(){//预测表 
    get_ch();//读文法，加载Nterm和Term 
    pre_list.clear();//清空 
    string str, foo;
    for(map<string, Sets>::iterator it = select.begin(); it != select.end(); it ++){
        int i = 0;
        str = foo = "";
        while(it->first[i] != ' ' && i < it->first.size()) str += it->first[i ++];//读取select的map数据类型的string 
        foo = it->first.substr(i+1, it->first.size() - i - 1);//例如 str->foo 

        for(Sets::iterator ti = it->second.begin(); ti != it->second.end(); ti ++){
            pre_list[P(str ,*ti)] = foo;//写预测表 
        }
    }
    fstream out;//输出 
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

bool analysis(ofstream &out){//分析过程  存放在 process.txt文件中 
    vector<string> p;
    string t, foo, str, fxx;

    p.push_back("#");//预处理
    p.push_back(Start);
    watest.push_back(pro{"#", "#", 0});

    out<<"  步骤 |    当前栈顶    |     当前串首     |   推导所用产生式                 \n";
    out<<"------------------------------------------------------------------------------\n";
    int step = 1, pos = 0;
    while(!p.empty() && pos < watest.size()){//结束条件是栈为空或者词法识别完 
        out<<setw(6)<<(step ++)<<setw(18)<<p.back()<<setw(19)<<watest[pos].type;
        if(p.back() == "#" && watest[pos].type == "#"){//当前栈和串都是#说明识别完成了 
            out<<"    "<<"接受"<<endl;
            return true;
        }

        if(p.back() == watest[pos].type){
            out<<"    "<<"“"<<watest[pos].type<<"”"<<"匹配"<<endl;//类型相同则匹配 
            p.pop_back();//出栈 
            pos ++;
        }
        else{
            t = pre_list[P(p.back(), watest[pos].type)];
            if(t == "") {
				cout<<"栈的标识不同，出错! \t"<<"当前栈顶为："<< p.back()<<"\t"<<"当前串首为："<< watest[pos].type<<endl;
				cout<<"出错为第"<<watest[pos].line<<"行,该单词或符号为："<<watest[pos].value<<"\t 它的id_number: "<<watest[pos].number<<endl;
				cout<<"出错后的一个为"<<watest[pos+1].line<<"行,该单词或符号为："<<watest[pos+1].value<<"\t 它的id_number: "<<watest[pos+1].number<<endl;
				return false;//预测表中无，则出错 
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

//输出分析过程
    ofstream out;
    out.open("process.txt", ios::out);
    bool flag = analysis(out);
    out<<(flag ? "\n\n-->LL(1)合法句子\n\n" : "\n\n-->LL(1)非法句子\n\n");
    out.close();
    return flag;
}
