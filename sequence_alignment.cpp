#include "SequenceAlignment.hpp"
#include <iostream>
#include <chrono>
#include <sys/resource.h>

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
  const int SEG_SIZE = 50;

  SequenceAlignment sequenceAlignment(input_filename);
  int m = (int) sequenceAlignment.str1.size();
  int n = (int) sequenceAlignment.str2.size();
  string str1, str2;

  auto start = chrono::high_resolution_clock::now();

  if (is_efficient) {
    // divide and conquer
    tie(str1, str2) = sequenceAlignment.divide_conquer_alignment(
      sequenceAlignment.str1, sequenceAlignment.str2, 0);
  } else {
    tie(str1, str2) = sequenceAlignment.find_alignment_dynamic_programming(
      sequenceAlignment.str1, sequenceAlignment.str2);
  }

  auto stop = chrono::high_resolution_clock::now();
  auto duration = chrono::duration_cast<chrono::microseconds>(stop - start);

  struct rusage usage;
  if (0 != getrusage(RUSAGE_SELF, &usage)) { cerr << "getrusage failed!" << endl; }

  int cost = sequenceAlignment.alignment_cost(str1, str2);

  std::ofstream output("output.txt", std::ios_base::out);

  output << str1.substr(0, SEG_SIZE) << " ";
  output << str1.substr(str1.size() - SEG_SIZE) << endl;
  output << str2.substr(0, SEG_SIZE) << " ";
  output << str2.substr(str2.size() - SEG_SIZE) << endl;

  output << cost << "\n" << (int) duration.count() / 1e6 << endl << (long double) usage.ru_maxrss / 1e3 << endl;
  output << "(" << m << ", " << n << ") -> " << m * n << endl;

  cout << "alignment_cost=" << sequenceAlignment.alignment_cost(str1, str2)
       << endl;
  cout << "str1=" << str1 << "\nstr2=" << str2 << endl;
}
