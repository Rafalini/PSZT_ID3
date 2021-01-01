#include "Node.hpp"
#include <vector>
#include <string>
#include <algorithm>
#include <utility>
#include <map>
#include <unordered_set>
#include <math.h>
#include <iostream>

Node::Node(std::map<int,std::string> atributes, std::vector<std::vector<int>> learning_data){
  //ID3 algorithm
  //if only one class occurs in learning data return that class
  std::cout <<"tree level: "<<55-atributes.size() <<std::endl;
  if(how_many_classes_in_data_set(learning_data)==1){
    return_class = learning_data[0].back();
    return;
  }

  //if atributes.size() == 0 return most oftem class
  if(atributes.size() == 0){
    return_class = most_oftem_occurring_class(learning_data);
    return;
  }

    //select best atribute
  this->atribute = atribute_with_max_entropy(atributes, learning_data);
  atributes.erase(this->atribute.first);

  std::map<int,std::vector<std::vector<int>>> sets_divided = sets_divided_by_atribute(this->atribute.first, learning_data);
    //for all possible values of atribute add child
  for(auto child : sets_divided)
    children.insert(std::pair<int,Node>(child.first, Node(atributes, child.second)));
    //std::cout <<"atrib size: "<<atributes.size() <<" dataset size:"<< child.second.size() <<std::endl;
}

double Node::calculate_entropy(std::vector<std::vector<int>> learning_data){
  std::map<int,int> class_occurences; //class, occurences

  for (auto single_case : learning_data) {
      int curr_class = single_case.back();
      ++class_occurences[curr_class];
  }

  int all_cases = learning_data.size();

  double entropy = 0;
  for(auto single_class : class_occurences){
      double frequency = (double)single_class.second / (double)all_cases;
      entropy -= frequency * log(frequency);
  }
  return entropy;
}

double Node::entropy_for_division(int atribute, std::vector<std::vector<int>> learning_data){

  std::map<int,std::vector<std::vector<int>>> sets_divided = sets_divided_by_atribute(atribute, learning_data);
  double entropy=0;

  for(auto single_set : sets_divided)
      entropy += (double)single_set.second.size() / (double) learning_data.size() * calculate_entropy(single_set.second);
  return entropy;
}

std::map<int,std::vector<std::vector<int>>> Node::sets_divided_by_atribute(int atribute, std::vector<std::vector<int>> learning_data){
  std::map<int,std::vector<std::vector<int>>> sets_divided_by_atribute; //class, occurences

  for(auto single_case : learning_data){
    int curr_attrib = single_case[atribute];
    sets_divided_by_atribute[curr_attrib].push_back(single_case);
  }
  return sets_divided_by_atribute;
}

double Node::inf_gain(int atribute, std::vector<std::vector<int>> learning_data){
  return calculate_entropy(learning_data) - entropy_for_division(atribute,learning_data);
}

int Node::how_many_classes_in_data_set(std::vector<std::vector<int>> learning_data){
  std::unordered_set<int> classes; //classes that occur in learning data

  for(auto single_case : learning_data) {
    int curr_class = single_case.back();
    if (classes.find(curr_class) == classes.end())
      classes.insert(curr_class);
  }

  return classes.size();
}

int Node::most_oftem_occurring_class(std::vector<std::vector<int>> learning_data){
  std::map<int,int> classes; //classes that occur in learning data

  int most_common_class = learning_data[0].back();
  for (auto single_case : learning_data) {
    int curr_class = single_case.back();
    int curr_class_occurence = ++classes[curr_class];

    if (curr_class_occurence > classes[most_common_class])
      most_common_class = curr_class;
  }

  return most_common_class;
}

std::pair<int,std::string> Node::atribute_with_max_entropy(std::map<int,std::string> atributes, std::vector<std::vector<int>> learning_data){
  double max_entropy_gain = 0.0;
  std::pair<int,std::string> max_atribute = std::pair<int,std::string>(atributes.begin()->first, atributes.begin()->second);
  for(auto atribute : atributes){
      double information_gain_from_division = inf_gain(atribute.first, learning_data);
      if(information_gain_from_division>max_entropy_gain)
      {
          max_entropy_gain = information_gain_from_division;
          max_atribute = atribute;
      }
  }
  return max_atribute;
}

void Node::print(int level){
  for(auto child : children)
    child.second.print(level+1);
  std::cout << atribute.second << " on lvl: "<<level<< std::endl;
}

int Node::predict(std::vector<int> input_case){

  return 0;
}
