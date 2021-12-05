#include "SequenceGen.h"
#include "dynamic_programming_algorithm.h"

using namespace std;

int main(int argc, char * argv[]) {
  if (argc < 2) { cerr << "写错命令行参数啦衰仔！" << endl; }
  string input_filename = argv[1];

  // hardcoded MISMATCH_COST
  const int GAP_COST = 30;
  auto MISMATCH_COST = construct_mismatch_cost();

  // head/tail output size
  const int OUTPUT_SIZE = 50;

  string s1, s2;
  vector<int> insert_pos1, insert_pos2;

  process_input(input_filename, s1, s2, insert_pos1, insert_pos2);

  generate_string(s1, insert_pos1);
  generate_string(s2, insert_pos2);

  cout << "Program parameters:\n\ts1=" << s1 << "\n\ts2=" << s2 << endl;

  print_mismatch_cost(MISMATCH_COST);

  // ready to run algorithm
  vector<vector<int>> min_cost(s1.size() + 1, vector<int>(s2.size() + 1, 0));
  string s1_matched, s2_matched;

  dynamic_programming(min_cost, s1, s2, MISMATCH_COST, GAP_COST);
  dynamic_programming_trace_back(s1_matched, s2_matched, min_cost, s1, s2,
                                 MISMATCH_COST, GAP_COST);

  std::reverse(s1_matched.begin(), s1_matched.end());
  std::reverse(s2_matched.begin(), s2_matched.end());
  cout << "s1_matched=" << s1_matched << endl;
  cout << "s2_matched=" << s2_matched << endl;

  //  trace_back(min_cost, s1, s2, MISMATCH_COST, GAP_COST);

  return 0;
}
