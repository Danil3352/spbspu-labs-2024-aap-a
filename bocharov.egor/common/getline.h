#ifndef GETLINE_H
#define GETLINE_H
#include <iostream>
namespace bocharov
{
  char * extend_the_string(const char * string, std::size_t capacity);
  char * getline(std::istream & in);
}
#endif
