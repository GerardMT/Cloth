#include "util.h"

#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;

void readFile(const string& filename, string& shader_source) {
  ifstream infile(filename.c_str());

  if (!infile.is_open() || !infile.good()) {
    cerr << "Error " + filename + " not found." << endl;
    throw 1;
  }

  stringstream stream;
  stream << infile.rdbuf();
  infile.close();

  shader_source = stream.str();
}
