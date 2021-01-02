#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <string>
#include <set>
#include <cassert>

#include "ID3.hpp"

using namespace std;

vector<vector<int>> read_data_file(set<int> &classes, string input)
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

double test(vector<vector<int>> &data, ID3 &id3) {
    int valid_predictions = 0;
    for (auto test_case : data) {
        if(id3.predict(test_case) == test_case.back())
            valid_predictions++;
    }

    return (double)valid_predictions/(double)data.size() * 100.0;
}

int main()
{
    set<int> atributes;
    vector<vector<int>> data = read_data_file(atributes, "divorce.csv");

    ID3 id3(atributes, data);

    cout << "correct predictions percentage: " << test(data, id3) << " %" << endl << endl;

    id3.print();

    return 0;
}
