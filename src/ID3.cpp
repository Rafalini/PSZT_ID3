#include "ID3.hpp"
#include "Node.hpp"

#include <memory>
#include <vector>
#include <string>
#include <map>
#include <tuple>
#include <set>
#include <cmath>
#include <iostream>

ID3::ID3(std::set<int> &attributes, std::vector<std::vector<int>> &learning_data){
  root = build_ID3(attributes, learning_data);
}

std::shared_ptr<Node> ID3::build_ID3(std::set<int> attributes, std::vector<std::vector<int>> learning_data) {
  //ID3 algorithm
  //if only one class occurs in learning data return that class
  if(is_one_class_in_data_set(learning_data)){
    int return_class = learning_data[0].back();
    return std::make_shared<Node>(return_class, -1);
  }

  //if attributes.empty() return most often class
  if(attributes.empty()) {
    int return_class = most_often_occurring_class(learning_data);
    return std::make_shared<Node>(return_class, -1);
  }

  //select best attribute
  int division_attribute = attribute_with_max_entropy(attributes, learning_data);
  attributes.erase(division_attribute);

  std::map<int,std::vector<std::vector<int>>> sets_divided = sets_divided_by_attribute(division_attribute, learning_data);

  // call recursively for sets divided by attribute
  std::shared_ptr<Node> currRoot(new Node(-1, division_attribute));
  for (int attribValue = MIN_ATTRIBUTE_VALUE; attribValue <= MAX_ATTRIBUTE_VALUE; attribValue++) {
    auto subset = sets_divided[attribValue];
    if (!subset.empty()) {
        currRoot->children[attribValue] = build_ID3(attributes, subset);
    }
    else {
        int most_occurring_class = most_often_occurring_class(learning_data);
        currRoot->children[attribValue] = std::make_shared<Node>(most_occurring_class, -1);
    }
  }

  return currRoot;
}

double ID3::calculate_entropy(std::vector<std::vector<int>> &learning_data){
  std::map<int,int> class_occurrences; //class, occurrences

  for (auto single_case : learning_data) {
      int curr_class = single_case.back();
      ++class_occurrences[curr_class];
  }

  int all_cases = learning_data.size();

  double entropy = 0;
  for(auto single_class : class_occurrences){
      double frequency = (double)single_class.second / (double)all_cases;
      entropy -= frequency * log(frequency);
  }
  return entropy;
}

double ID3::entropy_for_division(int attribute, std::vector<std::vector<int>> &learning_data){

  std::map<int,std::vector<std::vector<int>>> sets_divided = sets_divided_by_attribute(attribute, learning_data);
  double entropy=0;

  for(auto single_set : sets_divided)
      entropy += (double)single_set.second.size() / (double) learning_data.size() * calculate_entropy(single_set.second);
  return entropy;
}

std::map<int,std::vector<std::vector<int>>> ID3::sets_divided_by_attribute(int attribute, std::vector<std::vector<int>> &learning_data){
  std::map<int,std::vector<std::vector<int>>> sets_divided_by_atribute; //class, occurences

  for(auto single_case : learning_data){
    int curr_attrib = single_case[attribute];
    sets_divided_by_atribute[curr_attrib].push_back(single_case);
  }
  return sets_divided_by_atribute;
}

double ID3::inf_gain(int attribute, std::vector<std::vector<int>> &learning_data){
  return calculate_entropy(learning_data) - entropy_for_division(attribute, learning_data);
}

bool ID3::is_one_class_in_data_set(std::vector<std::vector<int>> &learning_data){
  std::set<int> classes; //classes that occur in learning data

  for(auto single_case : learning_data) {
    int curr_class = single_case.back();
    classes.insert(curr_class);
  }

  return classes.size() == 1;
}

int ID3::most_often_occurring_class(std::vector<std::vector<int>> &learning_data){
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

int ID3::attribute_with_max_entropy(std::set<int> &attributes, std::vector<std::vector<int>> &learning_data){
  double max_entropy_gain = 0.0;
  int max_attribute = *attributes.begin();
  for(auto attribute : attributes){
      double information_gain_from_division = inf_gain(attribute, learning_data);
      if(information_gain_from_division>max_entropy_gain)
      {
          max_entropy_gain = information_gain_from_division;
          max_attribute = attribute;
      }
  }
  return max_attribute;
}

void ID3::print() const{
  std::vector<std::tuple<int, int, int, std::shared_ptr<Node>>> nodes = {{0, -1, -1, root}}; // {tree height, attributeName, attributeValue, node}
  int currLevel = 0;

  std::cout << "root";
  for(size_t i = 0; i < nodes.size(); i++) {
    int height = std::get<0>(nodes[i]);
    int attribName = std::get<1>(nodes[i]) + 1;
    int attribValue = std::get<2>(nodes[i]);
    std::shared_ptr<Node> currNode = std::get<3>(nodes[i]);

    if (height != currLevel) {
        currLevel++;
        std::cout << std::endl << std::endl;
    }

    if (attribName != 0) {
        std::cout  << "Atr" + std::to_string(attribName) << ": " << attribValue << " ";
    }

    if  (currNode->return_class != -1) {
        std::cout << "returned class: " << currNode->return_class;
    }

    std::cout << "     ";

    for (const auto &child : currNode->children)
        nodes.emplace_back(height + 1, currNode->attribute, child.first, child.second);
  }

  std::cout << std::endl << std::endl;
}

int ID3::predict(std::vector<int> input_case) const{
  std::shared_ptr<Node> currNode = root;
  while (currNode->return_class == -1) {
      int attrValue = input_case[currNode->attribute];
      currNode = currNode->children[attrValue];
  }

  return currNode->return_class;
}
