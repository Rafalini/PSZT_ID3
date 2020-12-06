#include "Node.hpp"
#include <vector>
#include <string>
#include <algorithm>
#include <utility>
#include <map>
#include <math.h>
#include <iostream>

Node::Node(){}
Node::Node(std::map<int,std::string> atributes, std::vector<std::vector<int>> learning_data){
  //ID3 algorithm
  //if only one class occurs in learning data return that class
  std::cout <<"tree level: "<<55-atributes.size() <<std::endl;
  if(how_many_classes_in_data_set(learning_data)==1){
    is_leave=true;
    return_class = learning_data[0][learning_data[0].size()-1];
    return;
  }

  //if atributes.size() == 0 return most oftem class
  if(atributes.size() == 0){
    is_leave=true;
    return_class = most_oftem_occurring_class(learning_data);
    return;
  }

    //select best atribute
  is_leave = false;
  this->atribute = atribute_with_max_entropy(atributes, learning_data);
  atributes.erase(this->atribute.first);

  std::map<int,std::vector<std::vector<int>>> sets_divided = sets_divided_by_atribute(this->atribute.first, learning_data);
    //for all possible values of atribute add child
  for(auto child : sets_divided)
    children.insert(std::pair<int,Node>(child.first, Node(atributes, child.second)));
    //std::cout <<"atrib size: "<<atributes.size() <<" dataset size:"<< child.second.size() <<std::endl;
}

double Node::set_entropy(std::vector<std::vector<int>> learning_data){
  std::map<int,int> class_occurences; //class, occurences
  int all_cases = 0;

  for(auto single_case : learning_data){
    ++all_cases;
    std::map<int,int>::iterator position = class_occurences.find(single_case[single_case.size()-1]);
    if(position == class_occurences.end())
        class_occurences.insert(std::pair<int,int>(single_case[single_case.size()-1],1));
    else
        position->second++;
  }

  double entropy=0;
  for(auto single_class : class_occurences){
      double frequency = (double)single_class.second / (double)all_cases;
      entropy += (-1)*frequency * log(frequency);
  }
  return entropy;
}

double Node::entropy_for_division(int atribute, std::vector<std::vector<int>> learning_data){

  std::map<int,std::vector<std::vector<int>>> sets_divided = sets_divided_by_atribute(atribute, learning_data);
  double entropy=0;

  for(auto single_set : sets_divided)
      entropy += set_entropy(single_set.second);
  return entropy;
}

std::map<int,std::vector<std::vector<int>>> Node::sets_divided_by_atribute(int atribute, std::vector<std::vector<int>> learning_data){
  std::map<int,std::vector<std::vector<int>>> sets_divided_by_atribute; //class, occurences

  for(auto single_case : learning_data){
    std::map<int,std::vector<std::vector<int>>>::iterator position = sets_divided_by_atribute.find(single_case[atribute]);

    if(position == sets_divided_by_atribute.end()){
        std::vector<std::vector<int>> that_case{single_case};
        sets_divided_by_atribute.insert(std::pair<int,std::vector<std::vector<int>>>(single_case[atribute],that_case));
    }else{
        std::vector<int> that_case{single_case};
        position->second.push_back(that_case);
    }
  }
  return sets_divided_by_atribute;
}

double Node::inf_gain(int atribute, std::vector<std::vector<int>> learning_data){
  return set_entropy(learning_data) - entropy_for_division(atribute,learning_data);
}

int Node::how_many_classes_in_data_set(std::vector<std::vector<int>> learning_data){
  std::vector<int> classes; //classes that occur in learning data

  for(auto& single_case : learning_data)
    if(std::find(classes.begin(), classes.end(), single_case[single_case.size()-1]) == classes.end())
      classes.push_back(single_case[single_case.size()-1]);

  return classes.size();
}

int Node::most_oftem_occurring_class(std::vector<std::vector<int>> learning_data){
  std::map<int,int> classes; //classes that occur in learning data

  int class_=learning_data[0][learning_data[0].size()-1]; //first class to remembered in tmp variable
  int max_occurrence=1;                                    //its occurence

  for(auto single_case : learning_data){
    std::map<int,int>::iterator position = classes.find(single_case[single_case.size()-1]);
    if(position == classes.end())
      classes.insert(std::pair<int,int>(single_case[single_case.size()-1],1));
    else{
      position->second++;

      if(position->second>max_occurrence){                 //refresh most occurring class
        max_occurrence=position->second; class_=position->first;
      }
    }
  }

  return class_;
}

std::pair<int,std::string> Node::atribute_with_max_entropy(std::map<int,std::string> atributes, std::vector<std::vector<int>> learning_data){

  double max_entropy_gain = 0.0;
  std::pair<int,std::string> max_atribute = std::pair<int,std::string>(atributes.begin()->first, atributes.begin()->second);
  for(auto atribute : atributes){
      int information_gain_from_division = inf_gain(atribute.first, learning_data);
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
