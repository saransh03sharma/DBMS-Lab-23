#include <iostream>
#include <cstdio>
#include <cstring>
#include<bits/stdc++.h>
#include "defs.h"

using namespace std;

struct Student_n {
    string roll;
    float ct_marks;
    float ms_marks;
};
struct Student_d {
    string roll;
    string name;
    float ct_marks;
    float ms_marks;
};

string remove_foreign_ascii(const string& input) {
    string output;
    for (char c : input) {
        if ((c >= 0 && c <= 127) || (c>=0 && c<=9) || c=='.') {
            output += c;
        }
    }
    return output;
}
string remove_space(string str)
{
  
    for(int i=0;i+1<str.size();++i)
    {
        if(str[i]==' ' && str[i+1]==' ')
        {

            str[i]='\0';
            break;
        }
    }
    int len = strlen(str.c_str());
    return str.c_str();
}
string toLowerCase(string str) {
    transform(str.begin(), str.end(), str.begin(),
                   [](unsigned char c) { return tolower(c); });
    return str;
}

int main() {
    int option, flag = 0, replace;
    cout << "Which replacement technique you want to run: Clock (0) or LRU (1) or MRU (2): ";
    cin>>replace;
    cout << "\nWhich query you want to run: Select (0) or Join (1): ";
    cin>>option;
    string file1="b.txt", file2="a.txt";
    FILE *f1, *f2;
    Page *p, *q;



    string input_string;
    if(option==0){
        cout << "\nEnter Name to search for: ";
        getline(cin, input_string);
        getline(cin, input_string);
        f1 = fopen(file1.c_str(), "rb");
        if (!f1) {
            cerr << "Error: could not open data file." << endl;
            return 1;
        }
    }
    
    if(option==1)
    {
        
        f1 = fopen(file1.c_str(), "rb");
        if (!f1) {
            cerr << "Error: could not open data file." << endl;
            return 1;
        }
        f2 = fopen(file2.c_str(), "rb");
        if (!f2) {
            cerr << "Error: could not open data file." << endl;
            return 1;
        }

    }
    

    // Create buffer managers
    int num_bufs = 5;
    clock_buffer_manager clock_mgr(num_bufs);
    lru_buffer_manager lru_mgr(num_bufs);

    int i=0;
    // Read pages using clock replacement algorithm
    while(1) 
    {
        cout << "outer loop" << endl;
        if(replace==0)p = clock_mgr.read_page(f1, i);
        else if (replace==1)p = lru_mgr.read_page(f1, i);
        else p = clock_mgr.read_page(f1, i);

        if (!p) {
            break;
        }

        string data =  reinterpret_cast<char *>(p->disk_block);
        //cout << "Read page " << i << " using clock replacement." << endl;
        cout << "Page content: \n" << data << endl;
        stringstream ss(data);
        string line;

        int num_rows = 0;
        while (getline(ss, line)) {
            if (!line.empty()) {
                num_rows++;
            }
        }
        
        ss.clear();
        ss.seekg(0, ios::beg);


        int j = 0;
        Student_d stud;
        while (getline(ss, line)) {
            if (!line.empty()) {
                if(line.size()==2)continue;
                stud.name = toLowerCase(remove_space(line.substr(22,30)));
                
                stud.roll = line.substr(8, 9);
                stud.ct_marks = stof(remove_space(line.substr(51, 7)));
                stud.ms_marks = stof(remove_space(line.substr(59, 5)));
                if(option ==0){
                    if(stud.name.compare(toLowerCase(input_string))==0)
                    {
                        
                        cout << "Name: " << stud.name << "\nRoll Number: "<<stud.roll <<"\nClass Test 1 ct_marks: " << stud.ct_marks << "\nMid-Sem ct_marks: " << stud.ms_marks << endl;
                        flag=1;
                        break;
                    }
                }
                if(option==1){
                    int k=0;
                    while(1)
                    {
                        if(replace==0) q = clock_mgr.read_page(f2, k);
                        else if (replace==1) q = lru_mgr.read_page(f2, k);
                        else q = clock_mgr.read_page(f2, k);
                        if (!q) {
                            break;
                        }

                        string q_data =  reinterpret_cast<char *>(q->disk_block);
                        stringstream ss(q_data);
                        string q_line;

                        int q_num_rows = 0;
                        while (getline(ss, q_line)) {
                            if (!q_line.empty()) {
                                q_num_rows++;
                            }
                        }

                        ss.clear();
                        ss.seekg(0, ios::beg);

                        Student_n q_stud;
                        while (getline(ss, q_line)) {
                            if (!q_line.empty()) {
                                q_stud.roll = q_line.substr(4, 9);
                                q_stud.ms_marks = stof(remove_space(q_line.substr(16, 5)));
                                q_stud.ct_marks = stof(remove_space(q_line.substr(25, 4)));
                                if(q_stud.roll.compare(stud.roll)==0)
                                {
                                    cout << "\nName: "<<stud.name<< "\nRoll Number: "<<q_stud.roll << "\nNetworks Mid-Sem Marks: "<<q_stud.ms_marks<< "\nDBMS Mid-Sem Marks: "<<stud.ms_marks<< "\nNetworks Class Test Marks: "<<q_stud.ct_marks<<"\nDBMS Class Test Marks: "<<stud.ct_marks<<endl;
                                    flag=1;
                                }
                                else{
                                    continue;
                                }
                            
                            }
                        }
                        if(replace == 0)clock_mgr.unpin_page(q);
                        else if(replace == 1)lru_mgr.unpin_page(q);
                        else clock_mgr.unpin_page(q);
                        k++;
                    }
                }
            }
        }
        if(replace==0)clock_mgr.unpin_page(p);
        else if(replace==1)lru_mgr.unpin_page(p);
        else clock_mgr.unpin_page(p);

        i++;
    }
    if (flag==0)cout <<"No matching entry found.";

    fclose(f1);

    return 0;
}
