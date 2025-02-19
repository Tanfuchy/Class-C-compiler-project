#include "grammar_analysis.h"
#include <bits/stdc++.h>
using namespace std;

//����
void init_pri(){    //���ȼ�����
    pri["("] = 10;
    pri["++"] = pri["--"] = 9;
    pri["*"] = pri["/"] = pri["%"] = 8;
    pri["+"] = pri["-"] = 7;
    pri[">"] = pri["<"] = pri[">="] = pri["<="] = 6;
    pri["!="] = pri["=="] = 5;
    pri["&&"] = 4;
    pri["||"] = 3;
    pri["="] = 2;
    pri[")"] = pri["@"] = 1;
}

void get_token(){//��ȡ��Ԫʽ
    last = now;
    now = watest[cnt ++];//��Ԫʽ������� 
}

bool expect(string Expect){   //ƥ�亯��
//	cout<<"���е������ˣ�expect: "<<Expect<<endl;
    if(now.type == Expect || now.value == Expect){
        get_token();
        return true;
    }
    else return false;
}

string itos(int x){//int to string ǿ������ת�� 
    stringstream ss;
    ss<<x;
    return ss.str();
}

string Caculate(string stop){//���ʽ
	//cout<<"���е������ˣ�Caculate "<<endl;
    int             bra = 0;
    string          op;
    string          arg1, arg2, t = "";
    string          v;
    stack<string>   num;
    stack<string>   opr;
    opr.push("@");
    while(true) {
        v = now.value;
        //cout<<"v: "<<v<<endl;
        //������ʽ�Ƿ����
        if(v == "(")    bra--;
        if(v == ")")    bra++;
        if(bra > 0 || expect(stop))  v = "@";

        //���ʽ����
        if(v != "@" && (now.type == "id" || now.type == "number")) {//��Ϊ��������ʱ
            num.push(v);
            //cout<<"��ջv"<<v<<endl; 
            if(expect("id"))         t = "";
            else if(expect("number")) t = "";//���ƥ���ȡ��Ԫʽ 
        }
        else {//��Ϊ�����ʱ
            op = opr.top();
            //cout<<"op��"<<op<<"\t"<<"v:"<<v<<endl; 
            while(pri[v] <= pri[op] && !(op == "(" && v != ")")) {
                if(op == "@" && v == "@"){
                	//cout<<"����return"<<endl;
                    return num.top();
                }
                opr.pop();
				//cout<<"�����ջ"<<endl;
                if(op == "(" && v == ")") {
                    break;
                }
                else if(op == "++" || op == "--") {//��Ϊ��Ŀ�����  ��׺++ 
                    arg1 = num.top();   num.pop();
                    four_exp.push_back(Four_exp{op, arg1, "_", "T" + itos(cnt_temp ++)});
                }
                else {//��Ϊ˫Ŀ�����
                    arg2 = num.top();   num.pop();
                    arg1 = num.top();   num.pop();

                    four_exp.push_back(Four_exp{op, arg1, arg2, "T" + itos(cnt_temp ++)});
                }

                num.push("T" + itos(cnt_temp - 1));
                op = opr.top();
                //cout<<"num.top():"<<num.top()<<"\t"<<"op:"<<op<<endl;
            }
            if(v != ")" && v != "@")    opr.push(v);
            expect(v);
            //cout<<"v:"<<v<<endl;
        }
    }
}

void fuzhi(pro left, string stop){//��ֵ���
	//cout<<"���е������ˣ���ֵ���"<<endl; 
    expect("=");
    string tt = Caculate(stop);
    four_exp.push_back(Four_exp{"=", tt, "_", left.value});
}


bool body(){//������
    while(true){
    	//cout<<"���е������ˣ�body"<<endl; 
        if(expect("type")){//�������
            if(expect("id")){
            	//cout<<"���е������ˣ�push"<<endl; 
                var.push_back(last.value);
                expect(";");
            }
        }
        else if(expect("cin")){//cin�������ʽ 
        	//cout<<"���е������ˣ�cin"<<endl;
            while(expect(">>") && expect("id")) four_exp.push_back(Four_exp{"cin", "_", "_", last.value});
            expect(";");
        }
        else if(expect("cout")){//cout�������ʽ 
        	//cout<<"���е������ˣ�cout"<<endl;
            while(expect("<<") && expect("id")) four_exp.push_back(Four_exp{"cout", "_", "_", last.value});
            expect(";");
        }
        else if(expect("id")){//��ֵ���
        	//cout<<"���е������ˣ�="<<endl;
            if(find(var.begin(), var.end(), last.value) == var.end()){
                cout<<"Variable��"<<last.value<<" is not declare in here��"<<endl;
                return false;
            }
            fuzhi(last, ";");
        } 
        else if(expect("if") && expect("(")){//if��� 
        //cout<<"���е������ˣ�if"<<endl; 
            string tt = Caculate(")");//��Ҫ�����߼����ʽ��ֵ
			expect(")"); 
            expect("{");
            //cout<<"���е������ˣ�4"<<endl;
            four_exp.push_back(Four_exp{"J=", tt, "1", "(" + itos(four_exp.size()+2) + ")"});//������Ԫʽ 
            four_exp.push_back(Four_exp{"J", "_", "_", ""});
            int ad = four_exp.size() - 1;
            //cout<<"���е������ˣ�5"<<endl;
            body();
            if(expect("else") && expect("{")){//else�� 
            	//cout<<"���е������ˣ�else"<<endl;
                four_exp.push_back(Four_exp{"J", "_", "_", ""});
                int sd = four_exp.size() - 1;
                four_exp[ad].left += "(" + itos(four_exp.size()) + ")";
                body();
                four_exp[sd].left += "(" + itos(four_exp.size()) + ")";
            }
            else four_exp[ad].left += "(" + itos(four_exp.size()) + ")";
        }
        else if(expect("while") && expect("(")){//while��� 
        	//cout<<"���е������ˣ�while"<<endl;
            string tt = Caculate(")");
            expect(")");
            expect("{");
            four_exp.push_back(Four_exp{"J=", tt, "1", "(" + itos(four_exp.size()+2) + ")"});
            four_exp.push_back(Four_exp{"J", "_", "_", ""});//�ٳ��� 
            int ad = four_exp.size() - 1;
            body();
            four_exp.push_back(Four_exp{"J", "_", "_", "(" + itos(ad - 2) + ")"});//ִ���꺯�����ڣ�����while���ж���� 
            four_exp[ad].left += "(" + itos(four_exp.size()) + ")";//���ϼٳ��� 
        }
        else if(expect("for") && (expect("("))){//for��� 
     		//cout<<"���е������ˣ�for"<<endl; 
            expect("id");
//            if(find(var.begin(), var.end(), now.value) == var.end()){
//                cout<<"Variable"<<now.value<<"is not declare in here��"<<endl;
//                return false;
//            }
            fuzhi(last, ";");
            string tt = Caculate(";");//��Ҫ�����߼����ʽ��ֵ 
            four_exp.push_back(Four_exp{"J=", tt, "1", "(" + itos(four_exp.size()+2) + ")"});
			int ad = four_exp.size() - 1;
			four_exp.push_back(Four_exp{"J", "_", "_", ""});
            int ft = four_exp.size() - 1;
//            expect("id");
//            if(find(var.begin(), var.end(), now.value) == var.end()){
//                cout<<"Variable"<<now.value<<"is not declare in here��"<<endl;
//                return false;
//            }
			pro id_temp = now;
			string res_temp = Caculate(")");//�ȼ����׺���ʽ��ֵ�����ǲ���������Ԫʽ 
			expect(")");
            expect("{");
            body();
            four_exp.push_back(Four_exp{"=", res_temp, "_", id_temp.value});//ִ���꺯�����ڣ�����for�ĺ�׺���ʽ 
            four_exp.push_back(Four_exp{"J", "_", "_", "(" + itos(ad-1) + ")"});//ִ���꺯�����ڣ�����for���߼��ж���� 
			four_exp[ft].left += "(" + itos(four_exp.size()) + ")";//���ϼٳ��� 

        }
        else{
            expect("}");
            return true;
        }
    }
    return false;
}

void four_exp_to_file(){//�����Ԫʽ
    fstream out;
    out.open("four_exp.txt", ios::out);

    int n = four_exp.size();
    out<<"		Four_expression"<<endl;
    out<<"============================="<<endl;
    for(int i=0; i<n; i++){
        out<<"("<<i<<")"<<"  ( "<<four_exp[i].op<<" , "<<four_exp[i].r1<<" , "<<four_exp[i].r2<<" , "<<four_exp[i].left<<" )"<<endl;
    }
}

void to_assemble(){//��Ԫʽ�����
    int fxx = 0;
    int n = four_exp.size();
    int cnt = 0, temp = 0;
    for(int i=0; i<n; i++){
        if(four_exp[i].op == "="){
            assemble.push_back(Four_exp{"MOV", four_exp[i].left, four_exp[i].r1, "_"});// =
        }
        else if(four_exp[i].op == "cin"){
            assemble.push_back(Four_exp{"IN", four_exp[i].left, "_", "_"});//  cin 
        }
        else if(four_exp[i].op == "cout"){
            assemble.push_back(Four_exp{"OUT", four_exp[i].left, "_", "_"});//	cout 
        }
        else if(four_exp[i].op == "+"){
            assemble.push_back(Four_exp{"MOV", "R" + itos(temp), four_exp[i].r1, "_"});//	+
            assemble.push_back(Four_exp{"ADD", "R" + itos(temp), four_exp[i].r2, "_"});
            assemble.push_back(Four_exp{"MOV", four_exp[i].left, "R" + itos(temp), "_"});
            temp ++;//�Ĵ�������� 
            cnt += 2;
        }
        else if(four_exp[i].op == "-"){
            assemble.push_back(Four_exp{"MOV", "R" + itos(temp), four_exp[i].r1, "_"});	//	-
            assemble.push_back(Four_exp{"SUB", "R" + itos(temp), four_exp[i].r2, "_"});
            assemble.push_back(Four_exp{"MOV", four_exp[i].left, "R" + itos(temp), "_"});
            temp ++;
            cnt += 2;
        }
        else if(four_exp[i].op == "*"){
            assemble.push_back(Four_exp{"MOV", "R" + itos(temp), four_exp[i].r1, "_"});//	*
            assemble.push_back(Four_exp{"MUL", "R" + itos(temp), four_exp[i].r2, "_"});
            assemble.push_back(Four_exp{"MOV", four_exp[i].left, "R" + itos(temp), "_"});
            temp ++;
            cnt += 2;
        }
        else if(four_exp[i].op == "/"){
            assemble.push_back(Four_exp{"MOV", "R" + itos(temp), four_exp[i].r1, "_"});//	/���� 
            assemble.push_back(Four_exp{"DIV", "R" + itos(temp), four_exp[i].r2, "_"});
            assemble.push_back(Four_exp{"MOV", four_exp[i].left, "R" + itos(temp), "_"});
            temp ++;
            cnt += 2;
        }
        else if(four_exp[i].op == "%"){
            assemble.push_back(Four_exp{"MOV", "R" + itos(temp), four_exp[i].r1, "_"});//	%ȡ�� 
            assemble.push_back(Four_exp{"MOD", "R" + itos(temp), four_exp[i].r2, "_"});
            assemble.push_back(Four_exp{"MOV", four_exp[i].left, "R" + itos(temp), "_"});
            temp ++;
            cnt += 2;
        }
        else if(four_exp[i].op == ">"){
            assemble.push_back(Four_exp{"CMP", four_exp[i].r1, four_exp[i].r2, "_"});//		>���� 
            assemble.push_back(Four_exp{"JG", "(" + itos(assemble.size() + 3) + ")", "_", "_"});
            assemble.push_back(Four_exp{"MOV", four_exp[i].left, "0", "_"});
            assemble.push_back(Four_exp{"J", "(" + itos(assemble.size() + 2) + ")", "_", "_"});
            assemble.push_back(Four_exp{"MOV", four_exp[i].left, "1", "_"});
            temp ++;
            cnt += 4;
        }
        else if(four_exp[i].op == "<"){
            assemble.push_back(Four_exp{"CMP", four_exp[i].r1, four_exp[i].r2, "_"});//		<С�� 
            assemble.push_back(Four_exp{"JNG", "(" + itos(assemble.size() + 3) + ")", "_", "_"});
            assemble.push_back(Four_exp{"MOV", four_exp[i].left, "0", "_"});
            assemble.push_back(Four_exp{"J", "(" + itos(assemble.size() + 2) + ")", "_", "_"});
            assemble.push_back(Four_exp{"MOV", four_exp[i].left, "1", "_"});
            temp ++;
            cnt += 4;
        }
        else if(four_exp[i].op == "J"){
            string s = four_exp[i].left;
            s.erase(0, 1);
            s.erase(s.size()-1, 1);//������ͷ��ĩβ��һλ�ģ��� 
            int foo = my_stoi(s) + cnt;
            if(++fxx == 1) foo += 8;
            else foo -= cnt;

            assemble.push_back(Four_exp{"JMP", "(" + itos(foo) + ")", "_", "_"});
        }
        else if(four_exp[i].op == "J="){
            string s = four_exp[i].left;
            s.erase(0, 1);
            s.erase(s.size()-1, 1);
            int foo = my_stoi(s) + cnt;

            assemble.push_back(Four_exp{"CMP", four_exp[i].r1, four_exp[i].r2, "_"});
            assemble.push_back(Four_exp{"JZ", "(" + itos(foo) + ")", "_", "_"});
            cnt ++;
        }
    }

//������ָ��
    fstream out;
    out.open("assemble.txt", ios::out);

    int len = assemble.size();
    out<<"                            Assemble_code                          "<<endl;
    out<<"==================================================================="<<endl;
    for(int i=0; i<len; i++){
        if(assemble[i].op[0] == 'J' || assemble[i].op == "IN" || assemble[i].op == "OUT") out<<"("<<i<<")"<<"   "<<assemble[i].op<<"   "<<assemble[i].r1<<endl;
        else out<<"("<<i<<")"<<"   "<<assemble[i].op<<"   "<<assemble[i].r1<<" , "<<assemble[i].r2<<endl;
        
		}
    
    out.close();
}

bool translation(){
    init_pri();//��ʼ���ȼ� 
    //var.push_back("=");
    int tt = 0;
    get_token();
    if(expect("int") && expect("main") && expect("(") && expect(")") && expect("{")){
        if(body()){
            four_exp_to_file();
            to_assemble();
            return true;
        }
    }
    four_exp_to_file();
//    cout<<var.size();
//    for(auto it=var.begin();it!=var.end();it++){
//    	cout<<*it<<endl;
//	}
    return false;
}
