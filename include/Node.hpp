#ifndef NODE
#define NODE

#include<vector>
#include <string>

class Node{

  bool is_leave;
  int return_class;
  std::vector<Node> children;

  double node_entropy; //?


  double entropy();
  double inf_gain();

public:
  Node(int output_class);
  Node(std::vector<int> classes, std::vector<std::string> atributes, std::vector<std::vector<int>> learning_data);

  int predict(std::vector<int> input_case);
};


#endif
