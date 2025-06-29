#include <exception>
#include <fstream>
#include <functional>
#include <iostream>
#include <string>
#include <utility>
#include <tree.hpp>
#include "keySum.hpp"

namespace
{
  using namespace bocharov;
  using AVLTree = Tree< int, std::string >;
  void input(std::istream & input, AVLTree & tree)
  {
    std::pair< int, std::string > pair;
    while (input >> pair.first >> pair.second)
    {
      tree.insert(pair);
    }
  }
}

int main(int argc, char ** argv)
{
  if (argc != 3)
  {
    std::cerr << "Invalid parameters\n";
    return 1;
  }
  std::ifstream file(argv[2]);
  AVLTree tree;
  input(file, tree);
  if (!file.eof())
  {
    std::cerr << "Incorrect input\n";
    return 1;
  }
  if (tree.empty())
  {
    std::cout << "<EMPTY>\n";
    return 0;
  }
  else
  {
    Tree< std::string, std::function< KeySum(KeySum) > > traverse;
    using namespace std::placeholders;
    traverse["ascending"] = std::bind(& AVLTree::traverse_lnr< KeySum >, std::ref(tree), _1);
    traverse["descending"] = std::bind(& AVLTree::traverse_rnl< KeySum >, std::ref(tree), _1);
    traverse["breadth"] = std::bind(& AVLTree::traverse_breadth< KeySum >, std::ref(tree), _1);
    try
    {
      KeySum funct;
      funct = traverse.at(argv[1])(funct);
      std::cout << funct.sum << funct.str << "\n";
    }
    catch (const std::exception & e)
    {
      std::cerr << e.what() << "\n";
      return 1;
    }
  }
}
