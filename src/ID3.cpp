#include "ID3.hpp"
#include "Node.hpp"

#include <vector>
#include <string>
#include <memory>
#include <map>
#include <tuple>
#include <set>
#include <math.h>
#include <iostream>

ID3::ID3(std::set<int> attributes, std::vector<std::vector<int>> learning_data){
  root = build_ID3(attributes, learning_data);
}

std::shared_ptr<Node> ID3::build_ID3(std::set<int> attributes, std::vector<std::vector<int>> learning_data) {
  //ID3 algorithm
  //if only one class occurs in learning data return that class
  if(is_one_class_in_data_set(learning_data)){
    int return_class = learning_data[0].back();
    return std::shared_ptr<Node>(new Node(return_class, -1));
  }

  //if atributes.size() == 0 return most oftem class
  if(attributes.size() == 0) {
    int return_class = most_oftem_occurring_class(learning_data);
    return std::shared_ptr<Node>(new Node(return_class, -1));
  }

  //select best atribute
  int division_attribute = atribute_with_max_entropy(attributes, learning_data);
  attributes.erase(division_attribute);

  std::map<int,std::vector<std::vector<int>>> sets_divided = sets_divided_by_atribute(division_attribute, learning_data);

  // call recursively for sets divided by attribute
  std::shared_ptr<Node> root(new Node(-1, division_attribute));
  for (auto data_subset : sets_divided) {
    std::shared_ptr<Node> child = build_ID3(attributes, data_subset.second);
    root->children[data_subset.first] = child;
  }

  return root;
}

double ID3::calculate_entropy(std::vector<std::vector<int>> learning_data){
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

double ID3::entropy_for_division(int atribute, std::vector<std::vector<int>> learning_data){

  std::map<int,std::vector<std::vector<int>>> sets_divided = sets_divided_by_atribute(atribute, learning_data);
  double entropy=0;

  for(auto single_set : sets_divided)
      entropy += (double)single_set.second.size() / (double) learning_data.size() * calculate_entropy(single_set.second);
  return entropy;
}

std::map<int,std::vector<std::vector<int>>> ID3::sets_divided_by_atribute(int atribute, std::vector<std::vector<int>> learning_data){
  std::map<int,std::vector<std::vector<int>>> sets_divided_by_atribute; //class, occurences

  for(auto single_case : learning_data){
    int curr_attrib = single_case[atribute];
    sets_divided_by_atribute[curr_attrib].push_back(single_case);
  }
  return sets_divided_by_atribute;
}

double ID3::inf_gain(int atribute, std::vector<std::vector<int>> learning_data){
  return calculate_entropy(learning_data) - entropy_for_division(atribute,learning_data);
}

bool ID3::is_one_class_in_data_set(std::vector<std::vector<int>> learning_data){
  std::set<int> classes; //classes that occur in learning data

  for(auto single_case : learning_data) {
    int curr_class = single_case.back();
    classes.insert(curr_class);
  }

  return classes.size() == 1;
}

int ID3::most_oftem_occurring_class(std::vector<std::vector<int>> learning_data){
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

int ID3::atribute_with_max_entropy(std::set<int> atributes, std::vector<std::vector<int>> learning_data){
  double max_entropy_gain = 0.0;
  int max_atribute = *atributes.begin();
  for(auto atribute : atributes){
      double information_gain_from_division = inf_gain(atribute, learning_data);
      if(information_gain_from_division>max_entropy_gain)
      {
          max_entropy_gain = information_gain_from_division;
          max_atribute = atribute;
      }
  }
  return max_atribute;
}

void ID3::print(){
  std::vector<std::tuple<int, int, int, std::shared_ptr<Node>>> nodes = {{0, -1, -1, root}}; // {tree height, attributeName, attributeValue, node}
  int currLevel = 0;

  std::cout << "root";
  for(size_t i = 0; i < nodes.size(); i++) {
    int height = std::get<0>(nodes[i]);
    int attribName = std::get<1>(nodes[i]);
    int attribValue = std::get<2>(nodes[i]);
    std::shared_ptr<Node> currNode = std::get<3>(nodes[i]);

    if (height != currLevel) {
        currLevel++;
        std::cout << std::endl << std::endl;
    }

    if (attribName != -1) {
        std::cout  << "Atr" + std::to_string(attribName) << ": " << attribValue << " ";
    }

    if  (currNode->return_class != -1) {
        std::cout << "returned class: " << currNode->return_class;
    }

    std::cout << "     ";

    for (auto child : currNode->children)
        nodes.push_back({height + 1, currNode->attribute, child.first, child.second});
  }
}

int ID3::predict(std::vector<int> input_case){
  std::shared_ptr<Node> currNode = root;
  while (currNode->return_class == -1) {
      int attrValue = input_case[currNode->attribute];
      currNode = currNode->children[attrValue];
  }

  return currNode->return_class;
}
