#pragma once

#include <map>
#include <memory>

struct Node {
  int return_class; // valid only for leafs
  int attribute;
  std::map<int, std::shared_ptr<Node>> children;

  Node(int ret_class, int attr);
};