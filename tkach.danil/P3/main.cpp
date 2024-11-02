#include <iostream>
#include <fstream>
#include "filemtx.h"
#include "mtxlogic.h"

int main(int argc, char** argv)
{
  std::ifstream input(argv[2]);
  std::ofstream output(argv[3]);
  char * s = argv[1];
  int flag = 1;
  int id = 0;
  if (argc < 4)
  {
    std::cerr << "Not enough arguments\n";
    return 1;
  }
  else if (argc > 4)
  {
    std::cerr << "Too many arguments\n";
  }
  for (size_t i = 0; s[i] != '\0'; i++)
  {
    if (!((s[i] >= '0') && (s[i] <= '9')))
    {
      if (s[i] != '-' && (i != 0))
      {
        flag = 0;
      }
    }
  }
  if (flag == 0)
  {
    std::cerr << "First parameter is not a number\n";
    return 1;
  }
  else
  {
    id = atoi(s);
    if ((id != 1) && (id != 2))
    {
      std::cerr << "First parameter is out of range\n";
      return 1;
    }
  }
  size_t str = 0, stl = 0;
  if ((input >> str >> stl).good())
  {
    if (stl < str)
    {
      str = stl;
    }
    else
    {
      stl = str;
    }
    if (id == 1)
    {
      int mtx[10000];
      if (!(tkach::inputMtx(input, mtx, str)).good())
      {
        std::cerr << "ERROR: Invalid input\n";
        return 2;
      }
      double* mtx2 = nullptr;
      try
      {
        mtx2 = tkach::createMtx2(mtx, str);
      }
      catch(const std::bad_alloc & e)
      {
        std::cerr << "Out of memory\n";
        return 2;
      }
      tkach::outputMtx(output, mtx2, str);
      output << "\n";
    }
    else if (id == 2)
    {
      int* mtx = nullptr;
      try
      {
        mtx = new int[str * str];
      }
      catch(const std::bad_alloc & e)
      {
        delete[] mtx;
        std::cerr << "Out of memory\n";
        return 1;
      }
      if (!(tkach::inputMtx(input, mtx, str)).good())
      {
        delete[] mtx;
        std::cerr << "ERROR: Invalid input\n";
        return 2;
      }
      double* mtx2 = nullptr;
      try
      {
        mtx2 = tkach::createMtx2(mtx, str);
        delete[] mtx;
      }
      catch(const std::bad_alloc & e)
      {
        delete[] mtx;
        std::cerr << "Out of memory\n";
        return 2;
      }
      tkach::outputMtx(output, mtx2, str);
      output << "\n";
    }
    else
    {
      std::cerr << "First parameter is out of range\n";
      return 1;
    }
  }
  else
  {
    std::cerr << "ERROR: No needed information\n";
    return 1;
  }
}