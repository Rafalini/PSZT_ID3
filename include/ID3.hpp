#pragma once

#include "Node.hpp"

#include <vector>
#include <string>
#include <set>
#include <map>

class ID3{
private:
  Node* build_ID3(std::set<int> attributes, std::vector<std::vector<int>> learning_data);

  double calculate_entropy(std::vector<std::vector<int>> learning_data);
  double entropy_for_division(int atribute, std::vector<std::vector<int>> learning_data);
  double inf_gain(int atribute, std::vector<std::vector<int>> learning_data);

  bool is_one_class_in_data_set(std::vector<std::vector<int>> learning_data);
  int most_oftem_occurring_class(std::vector<std::vector<int>> learning_data);
  int atribute_with_max_entropy(std::set<int> atributes, std::vector<std::vector<int>> learning_data);
  std::map<int,std::vector<std::vector<int>>> sets_divided_by_atribute(int atribute, std::vector<std::vector<int>> learning_data);

public:
  Node *root;
  ID3(std::set<int> attributes, std::vector<std::vector<int>> learning_data);
  int predict(std::vector<int> input_case);
  void print();
};

