#pragma once

#include <map>

struct Node {
  int return_class; // valid only for leafs
  int attribute;
  std::map<int, Node*> children;

  Node(int ret_class, int attr);
};