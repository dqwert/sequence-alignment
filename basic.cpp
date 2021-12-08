#include "SequenceAlignment.hpp"
#include <iostream>

using namespace std;


int main(int argc, char * argv[]) {
  if (argc < 2) { cerr << "写错命令行参数啦衰仔！" << endl; }

  string input_filename = argv[2];

  SequenceAlignment sequenceAlignment(input_filename);
  string str1, str2;

  tie(str1, str2) = sequenceAlignment.find_alignment_dynamic_programming(
    sequenceAlignment.str1, sequenceAlignment.str2);

  cout << "str1=" << str1 << "\nstr2=" << str2 << endl;
}
