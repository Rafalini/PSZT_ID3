#include "Node.hpp"
#include <vector>
#include <string>


Node::Node(int output_class):return_class(output_class){
  is_leave = true;
}
Node::Node(std::vector<int> classes, std::vector<std::string> atributes, std::vector<std::vector<int>> learning_data){
  //id3 alghoritm

  //if only one class occurs in learning data return that class

  //if atributes.size() == 0 return most oftem class

  //else design children

  //select best atribute

  //for all possible values of atribute add child
}

double Node::entropy(){
  return 0;
}

double Node::inf_gain(){
  return 0;
}

int Node::predict(std::vector<int> input_case){
  return 0;
}
