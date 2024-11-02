#ifndef FILEMTX_H
#define FILEMTX_H

#include <cstddef>
#include <fstream>

namespace tkach
{
  std::istream & inputMtx(std::istream & in, int* mtx1, size_t str);
  void outputMtx(std::ostream & out, const double* mtx, size_t str);
}

#endif