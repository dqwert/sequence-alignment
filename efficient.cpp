#include "SequenceAlignment.hpp"
#include <iostream>

using namespace std;


int main(int argc, char * argv[]) {
  if (argc < 3) { cerr << "写错命令行参数啦衰仔！" << endl; }

  // prepare
  string mode_str = string(argv[1]);
  bool is_efficient;
  if (mode_str == "basic") { is_efficient = false; }
  else if (mode_str == "efficient") { is_efficient = true; }
  else {
    cerr << "invalid command-line argument" << endl;
    return -1;
  }

  string input_filename = argv[2];

  SequenceAlignment sequenceAlignment(input_filename);
  string str1, str2;

  if (is_efficient) {
    // divide and conquer
    tie(str1, str2) = sequenceAlignment.divide_conquer_alignment(
      sequenceAlignment.str1, sequenceAlignment.str2, 0);
  } else {
    tie(str1, str2) = sequenceAlignment.find_alignment_dynamic_programming(
      sequenceAlignment.str1, sequenceAlignment.str2);
  }


  cout << "alignment_cost=" << sequenceAlignment.alignment_cost(str1, str2)
       << endl;
  cout << "str1=" << str1 << "\nstr2=" << str2 << endl;
}
