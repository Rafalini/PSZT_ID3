#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <string>
#include <set>

#include "ID3.hpp"
#include "Tests.hpp"

using namespace std;

vector<vector<int>> read_data_file(set<int> &classes, const string& input)
{
    fstream f; f.open(input, ios::in);

    string line; getline(f, line);

    stringstream class_stream(line);

    for(int i = 0; getline(class_stream, line, ';'); i++) {  //get classes
        classes.insert(i);
    }
    classes.erase(--classes.end());     //last string is 'Class', remove it

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
    set<int> attributes;
    vector<vector<int>> data = read_data_file(attributes, "divorce.csv");

    mean_stats(10, attributes, data);

//    exemplary decision tree
//    ID3 id3(attributes, data);
//    id3.print();

    return 0;
}
