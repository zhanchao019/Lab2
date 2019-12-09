#include<bits/stdc++.h>

using namespace std;

struct AnlzNode {
    string left;
    string right[100];
};

class Analysis {

public:
    int T;
    AnlzNode analysis_str[100];

    set<string> first_set[100];//first集
    set<string> follow_set[100];//follow集
    vector<string> ter_withoutblack; //去空字终结符
    vector<string> terminal;//终结符
    vector<string> nonterminal;//非终结符

    Analysis() : T(0) {}

    bool isNotTerminal(string c);//判断是否为非终结符
    void remove();//左递归消除
    int get_index(string target);//获得在终结符集合中的下标
    int get_non_index(string target);//获得在非终结符集合中的下标
    void get_first(string target); //得到first集合
    void get_follow(string target);//得到follow集合
    void inputAndSolve(); //处理得到first和follow
    void display(); //显示

};

class TableStack : public Analysis {

    int tableMap[100][100];//预测表
public:
    TableStack() { memset(tableMap, -1, sizeof(tableMap)); }

    void get_table(); //得到预测表
    void print_out();//输出
    void manage(); //综合
};

int main() {
    TableStack res;
    res.manage();
    int a;
    cin >> a;//等待回车退出
}


bool Analysis::isNotTerminal(string c) {
    for (int i = 0; i < nonterminal.size(); i++)
        if (c == nonterminal[i])
            return true;
    return false;
}

int Analysis::get_index(string target) {
    for (int i = 0; i < nonterminal.size(); i++) {
        if (target == nonterminal[i])
            return i;
    }
    return -1;
}

int Analysis::get_non_index(string target) {
    for (int i = 0; i < ter_withoutblack.size(); i++) {
        if (target == ter_withoutblack[i])
            return i;
    }
    return -1;
}

void Analysis::get_first(string target) {
    int tag = 0;
    int flag = 0;
    for (int i = 0; i < T; i++) {
        if (analysis_str[i].left == target)//匹配产生式左部
        {
            if (!isNotTerminal(analysis_str[i].right[0]))//对于终结符，直接加入first
            {
                first_set[get_index(target)].insert(analysis_str[i].right[0]);
            } else {
                for (int j = 0; analysis_str[i].right[j] != ""; j++) {
                    if (!isNotTerminal(analysis_str[i].right[j]))//终结符结束
                    {
                        first_set[get_index(target)].insert(analysis_str[i].right[j]);
                        break;
                    }
                    get_first(analysis_str[i].right[j]);//递归
                    set<string>::iterator it;
                    for (it = first_set[get_index(analysis_str[i].right[j])].begin();
                         it != first_set[get_index(analysis_str[i].right[j])].end(); it++) {
                        if (*it == "#")
                            flag = 1;
                        else
                            first_set[get_index(target)].insert(*it);//将FIRST(Y)中的非$就加入FIRST(X)
                    }
                    if (flag == 0)
                        break;
                    else {
                        tag += flag;
                        flag = 0;
                    }
                }
                int len = 0;
                while (analysis_str[i].right[len] != "")
                    len++;
                if (tag == len)//所有右部first(Y)都有#,将@加入FIRST(X)中
                    first_set[get_index(target)].insert("#");
            }
        }
    }

}

void Analysis::get_follow(string target) {
    for (int i = 0; i < T; i++) {
        int index = -1;
        int len = 0;
        while (analysis_str[i].right[len] != "")
            len++;
        for (int j = 0; j < len; j++) {
            if (analysis_str[i].right[j] == target) {
                index = j;
                break;
            }
        }
        if (index != -1 && index < len - 1) {
            string nxt = analysis_str[i].right[index + 1];
            if (!isNotTerminal(nxt)) {
                follow_set[get_index(target)].insert(nxt);
            } else {
                int isExt = 0;
                set<string>::iterator it;
                for (it = first_set[get_index(nxt)].begin(); it != first_set[get_index(nxt)].end(); it++) {
                    if (*it == "#")
                        isExt = 1;
                    else
                        follow_set[get_index(target)].insert(*it);
                }

                if (isExt && analysis_str[i].left != target) {
                    get_follow(analysis_str[i].left);
                    set<string>::iterator it;
                    string tmp = analysis_str[i].left;
                    for (it = follow_set[get_index(tmp)].begin(); it != follow_set[get_index(tmp)].end(); it++)
                        follow_set[get_index(target)].insert(*it);
                }
            }
        } else if (index != -1 && index == len - 1 && target != analysis_str[i].left) {
            get_follow(analysis_str[i].left);
            set<string>::iterator it;
            string tmp = analysis_str[i].left;
            for (it = follow_set[get_index(tmp)].begin(); it != follow_set[get_index(tmp)].end(); it++)
                follow_set[get_index(target)].insert(*it);
        }

    }
}

void Analysis::inputAndSolve() {
    ifstream Open("C:\\Users\\13431\\Desktop\\CLionProjects\\Lab2\\Input.txt");
    string input[100];
    T = 0;
    while (1) {
        string s;
        getline(Open, s);
        if (s == "")
            break;
        for (int i = 0; i < s.length(); i++) {
            if (s[i + 1] != '|') {
                input[T] += s[i];
            } else {
                i++;
                T++;
                for (int j = 0; j < s.length(); j++) {
                    if (s[j] != '-' && s[j + 1] != '>')
                        input[T] += s[j];
                    else break;
                }
                input[T] += "->";
            }
        }
        T++;
    }
    int index = 0;
    while (index < T) {
        string s = input[index];
        string temp[100];
        int len = 0;
        for (int i = 0; i < s.length(); i++) {
            if (s[i] != ' ')
                temp[len] += s[i];
            else len++;
        }
        analysis_str[index].left = temp[0];
        int j = -1;
        for (int i = 2; i <= len; i++) {
            j++;
            analysis_str[index].right[j] = temp[i];
        }
        int flag = 0;
        for (int j = 0; j < nonterminal.size(); j++) {
            if (nonterminal[j] == temp[0]) {
                flag = 1;
                break;
            }
        }
        if (!flag)
            nonterminal.push_back(temp[0]);
        index++;
    }
    for (int i = 0; i < T; i++) {
        for (int k = 0; analysis_str[i].right[k] != ""; k++) {
            if (!isNotTerminal(analysis_str[i].right[k])) {
                int flag = 0;
                for (int j = 0; j < terminal.size(); j++) {
                    if (terminal[j] == analysis_str[i].right[k]) {
                        flag = 1;
                        break;
                    }
                }
                if (!flag)
                    terminal.push_back(analysis_str[i].right[k]);
            }
        }
    }
    remove();
    terminal.push_back("@");
    //first
    for (int i = 0; i < nonterminal.size(); i++) {
        get_first(nonterminal[i]);
    }

    for (int i = 0; i < nonterminal.size(); i++) {
        if (i == 0)
            follow_set[0].insert("@");
        get_follow(nonterminal[i]);
    }

    for (int i = 0; i < terminal.size(); i++) {
        if (terminal[i] != "#")
            ter_withoutblack.push_back(terminal[i]);
    }
}

void Analysis::remove() {
    string rep[100];
    int index = 0;
    for (int i = 0; i < T; i++) {
        if (analysis_str[i].right[0] == analysis_str[i].left) {
            bool flag = true;
            if (index > 0)
                for (int j = 0; j < index; j++) {
                    if (analysis_str[i].left == rep[j])
                        flag = false;
                }
            if (flag)
                rep[index++] = analysis_str[i].left;
        }
    }
    for (int i = 0; i < index; i++) {
        for (int j = 0; j < T; j++) {
            if (analysis_str[j].left == rep[i]) {
                if (analysis_str[j].right[0] != rep[i] && analysis_str[j].right[0] != "#") {
                    int k;
                    for (k = 0; k < 100; k++)
                        if (analysis_str[j].right[k] == "")
                            break;
                    analysis_str[j].right[k] = rep[i] + "'";
                    int flag2 = 0;
                    for (int j = 0; j < nonterminal.size(); j++) {
                        if (nonterminal[j] == rep[i] + "'") {
                            flag2 = 1;
                            break;
                        }
                    }
                    if (!flag2)
                        nonterminal.push_back(rep[i] + "'");
                }
                if (analysis_str[j].right[0] == rep[i]) {
                    analysis_str[j].left += "'";
                    for (int k = 0; k < 99; k++)
                        analysis_str[j].right[k] = analysis_str[j].right[k + 1];
                    int k;
                    for (k = 0; k < 100; k++)
                        if (analysis_str[j].right[k] == "")
                            break;
                    analysis_str[j].right[k] = rep[i] + "'";
                    bool flag = true;
                    for (int t = 0; t < T; t++)
                        if ((analysis_str[t].left == rep[i] + "'") && analysis_str[t].right[0] == "#")
                            flag = false;
                    if (flag) {
                        analysis_str[T].left = rep[i] + "'";
                        analysis_str[T++].right[0] = "#";
                    }
                    int flag2 = 0;
                    for (int j = 0; j < nonterminal.size(); j++) {
                        if (nonterminal[j] == rep[i] + "'") {
                            flag2 = 1;
                            break;
                        }
                    }
                    if (!flag2)
                        nonterminal.push_back(rep[i] + "'");
                    int flag3 = 0;
                    for (int j = 0; j < terminal.size(); j++) {
                        if (terminal[j] == "#") {
                            flag3 = 1;
                            break;
                        }
                    }
                    if (!flag3)
                        terminal.push_back("#");
                }
            }
        }
    }
}

void Analysis::display() {
    cout << "FIRST SET:" << endl;
    for (int i = 0; i < nonterminal.size(); i++) {
        cout << nonterminal[i] << ": ";
        set<string>::iterator it;
        for (it = first_set[i].begin(); it != first_set[i].end(); it++)
            cout << *it << "  ";
        cout << endl;
    }

    cout << "FOLLOW SET:" << endl;
    for (int i = 0; i < nonterminal.size(); i++) {
        cout << nonterminal[i] << ": ";
        set<string>::iterator it1;
        for (it1 = follow_set[i].begin(); it1 != follow_set[i].end(); it1++)
            cout << *it1 << " ";
        cout << endl;
    }
}


void TableStack::get_table() {
    for (int i = 0; i < T; i++) {
        string tmp = analysis_str[i].right[0];
        if (!isNotTerminal(tmp)) {
            if (tmp != "#")
                tableMap[get_index(analysis_str[i].left)][get_non_index(tmp)] = i;
            if (tmp == "#") {
                set<string>::iterator it;
                for (it = follow_set[get_index(analysis_str[i].left)].begin();
                     it != follow_set[get_index(analysis_str[i].left)].end(); it++) {
                    tableMap[get_index(analysis_str[i].left)][get_non_index(*it)] = i;
                }
            }
        } else {
            set<string>::iterator ti;
            for (ti = first_set[get_index(tmp)].begin(); ti != first_set[get_index(tmp)].end(); ti++) {
                tableMap[get_index(analysis_str[i].left)][get_non_index(*ti)] = i;
            }
            if (first_set[get_index(tmp)].count("#") != 0) {
                set<string>::iterator it;
                for (it = follow_set[get_index(analysis_str[i].left)].begin();
                     it != follow_set[get_index(analysis_str[i].left)].end(); it++) {
                    tableMap[get_index(analysis_str[i].left)][get_non_index(*it)] = i;
                }
            }
        }
    }
}

void TableStack::print_out() {
    ofstream Out("Table_Output.txt");
    Out << setw(10) << '\0';
    for (int i = 0; i < ter_withoutblack.size(); i++) {
        string tercopy;
        tercopy += "  ";
        tercopy += ter_withoutblack[i];
        Out << left << setw(15) << tercopy;
    }
    Out << endl;
    for (int i = 0; i < nonterminal.size(); i++) {
        string nonter;
        nonter = nonterminal[i] + ": ";
        Out << setw(10) << nonter;
        for (int j = 0; j < ter_withoutblack.size(); j++) {
            if (tableMap[i][j] == -1)
                Out << setw(15) << '\0';
            else {
                int k = 0;
                string ss;
                ss += nonterminal[i];
                ss += "->";
                while (analysis_str[tableMap[i][j]].right[k] != "") {
                    ss += analysis_str[tableMap[i][j]].right[k];
                    k++;
                }
                Out << left << setw(15) << ss;
            }
        }
        Out << endl;
    }
    Out.close();
}

void TableStack::manage() {
    inputAndSolve();
    display();
    printf("---------------------\n\n");
    get_table();
    print_out();
    return;
}