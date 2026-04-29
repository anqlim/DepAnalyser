#ifndef DEPANALYSER_GRAPH_H
#define DEPANALYSER_GRAPH_H

#include <vector>
#include <string>

class Node {
public:
    Node(std::string file_name_);
    ~Node();

    std::string file_name;
    std::vector<Node*> dependencies;
    std::vector<Node*> dependents;

};

class Graph {

};


#endif
