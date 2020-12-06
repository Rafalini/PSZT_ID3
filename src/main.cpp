#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <string>
#include <utility>

#include "Node.hpp"

using namespace std;

vector<vector<int>> read_data_file(map<int,string> &classes, string input)
{
    fstream f; f.open(input, ios::in);

    string line; getline(f, line);

    stringstream class_stream(line);

    int i=0;
    while(getline(class_stream, line, ';'))   //get class names
      if(line != "Class")
        classes.insert(pair<int,string>(i++,line));
    classes.erase(54);                  //last string is 'Class', remove it

    vector<vector<int>> cases;          //vector for case vectors

    while(getline(f, line)){
        stringstream data_stream(line);
        vector<int> divorce_case;

        while(getline(data_stream, line, ';'))       //extract atributes from line
            divorce_case.push_back(stoi(line));

        cases.push_back(divorce_case);
    }
    return cases;
}


int main()
{
    //vector<int> classes = {0,1};
    map<int,string> atributes;
    vector<vector<int>> data = read_data_file(atributes, "divorce.csv");

    Node root(atributes, data);
    root.print(0);

    return 0;
}
