#include "SequenceAlignment.hpp"
#include <iostream>
#include <chrono>
#include <sys/resource.h>

using namespace std;


int main(int argc, char * argv[]) {
  if (argc < 2) {
    cerr << "写错命令行参数啦衰仔！" << endl;
    return -1;
  }

  string input_filename = argv[1];
  if (input_filename.empty()) {
    cerr << "empty filename" << endl;
    return -1;
  }
  const int SEG_SIZE = 50;

  SequenceAlignment sequenceAlignment(input_filename);
  int m = (int) sequenceAlignment.str1.size();
  int n = (int) sequenceAlignment.str2.size();

  string str1, str2;

  auto start = chrono::high_resolution_clock::now();

  // divide and conquer
  tie(str1, str2) = sequenceAlignment.divide_conquer_alignment(
    sequenceAlignment.str1, sequenceAlignment.str2, 0);

  auto stop = chrono::high_resolution_clock::now();
  auto duration = chrono::duration_cast<chrono::microseconds>(stop - start);

  struct rusage usage;
  if (0 != getrusage(RUSAGE_SELF, &usage)) {
    cerr << "getrusage failed!" << endl;
    return -1;
  }

  int cost = sequenceAlignment.alignment_cost(str1, str2);

  std::ofstream output("output.txt", std::ios_base::out);

  output << str1.substr(0, SEG_SIZE) << " ";
  output << str1.substr(str1.size() - SEG_SIZE) << endl;
  output << str2.substr(0, SEG_SIZE) << " ";
  output << str2.substr(str2.size() - SEG_SIZE) << endl;

  output << cost << "\n" << (int) duration.count() / 1e6 << endl
         << (long double) usage.ru_maxrss / 1e3;
//  output << "\n(" << m << ", " << n << ") -> " << m * n;
}
