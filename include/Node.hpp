#ifndef NODE
#define NODE

#include <vector>
#include <string>
#include <utility>
#include <map>

class Node{
  int return_class;                    //only if IS a leaf
  std::pair<int,std::string> atribute; //only if IS NOT leaf
  std::map<int,Node> children;

  double calculate_entropy(std::vector<std::vector<int>> learning_data);
  double entropy_for_division(int atribute, std::vector<std::vector<int>> learning_data);
  double inf_gain(int atribute, std::vector<std::vector<int>> learning_data);


  int how_many_classes_in_data_set(std::vector<std::vector<int>> learning_data); //counts different values on last position of 'case' vectors
  int most_oftem_occurring_class(std::vector<std::vector<int>> learning_data);   //returns value of most occuring class value
  //returns atribute which should be chosen to node in order to achieve max information gain
  std::pair<int,std::string> atribute_with_max_entropy(std::map<int,std::string> atributes, std::vector<std::vector<int>> learning_data);
  std::map<int,std::vector<std::vector<int>>> sets_divided_by_atribute(int atribute, std::vector<std::vector<int>> learning_data);

public:
  Node(std::map<int,std::string> atributes, std::vector<std::vector<int>> learning_data);
  void print(int level);
  int predict(std::vector<int> input_case);
};


#endif
