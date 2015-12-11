#include "rooibos/util.hh"

#include <iostream>

using std::cerr;
using std::endl;
using std::exit;
using std::string;

namespace rooibos
{
  void
  panic(const string & msg)
  {
    cerr << msg << endl;
    exit(1);
  }
}
