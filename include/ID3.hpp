#pragma once

#include "Node.hpp"

#include <vector>
#include <string>
#include <set>
#include <map>

class ID3{
private:
  std::shared_ptr<Node> root;
  const int MIN_ATTRIBUTE_VALUE = 0;
  const int MAX_ATTRIBUTE_VALUE = 4;
  
  std::shared_ptr<Node> build_ID3(std::set<int> attributes, std::vector<std::vector<int>> learning_data);

  static double calculate_entropy(std::vector<std::vector<int>> &learning_data);
  static double inf_gain(int attribute, std::vector<std::vector<int>> &learning_data);

  static bool is_one_class_in_data_set(std::vector<std::vector<int>> &learning_data);
  static int most_often_occurring_class(std::vector<std::vector<int>> &learning_data);
  static int attribute_with_max_entropy(std::set<int> &attributes, std::vector<std::vector<int>> &learning_data);
  static std::map<int,std::vector<std::vector<int>>> sets_divided_by_attribute(int attribute, std::vector<std::vector<int>> &learning_data);

public:
  ID3(std::set<int> &attributes, std::vector<std::vector<int>> &learning_data);
  static double entropy_for_division(int attribute, std::vector<std::vector<int>> &learning_data);
  int predict(std::vector<int> input_case) const;
  void print() const;
};

