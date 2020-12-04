#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <string>

#include "Node.hpp"

using namespace std;

vector<vector<int>> read_data_file(vector<string> &classes, string input)
{
    fstream f; f.open(input, ios::in);

    string line; getline(f, line);

    stringstream class_stream(line);

    while(getline(class_stream, line, ';'))   //get class names
        classes.push_back(line);
    classes.pop_back();                 //last string is 'Class', remove it

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
    vector<int> classes = {0,1};
    vector<string> atributes;
    vector<vector<int>> data = read_data_file(atributes, "divorce.csv");

    Node root(classes, atributes, data);

    return 0;
}
