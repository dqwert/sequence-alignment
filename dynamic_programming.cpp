#include "SequenceAlignment.h"
#include <iostream>

using namespace std;


int main(int argc, char * argv[]) {
  if (argc < 2) { cerr << "写错命令行参数啦衰仔！" << endl; }
  string input_filename = argv[1];

  SequenceAlignment sequenceAlignment(input_filename);
  string str1, str2;

  // divide and conquer
  tie(str1, str2) = sequenceAlignment.divide_conquer_alignment(sequenceAlignment.str1, sequenceAlignment.str2, 0);

  // dynamic programming
//  sequenceAlignment.find_alignment_dynamic_programming(sequenceAlignment.str1, sequenceAlignment.str2);

  cout << "str1=" << str1 << "\nstr2=" << str2 << endl;
}
