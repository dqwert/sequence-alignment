#include "sequence_gen.h"
#include "dynamic_programming_algorithm.h"
#include <limits>

#include <iostream>

using namespace std;

//void
//divide_conquer_alignment(string & s1, int s1_begin, int s1_end,
//                         string & s2, int s2_begin, int s2_end,
//                         std::unordered_map<char, std::unordered_map<char, int>> & mismatch_cost,
//                         int gap_cost, int depth);

void test_forward_backward(
  const std::string & s1, const std::string & s2,
  std::unordered_map<char, std::unordered_map<char, int>> & mismatch_cost,
  int gap_cost);


void test_dynamic_programming(
  std::string & s1_matched, std::string & s2_matched,
  std::string & s1, std::string & s2,
  std::unordered_map<char, std::unordered_map<char, int>> & mismatch_cost,
  int gap_cost);


int main(int argc, char * argv[]) {
  if (argc < 2) { cerr << "写错命令行参数啦衰仔！" << endl; }
  string input_filename = argv[1];

  // hardcoded MISMATCH_COST
  const int GAP_COST = 30;
  auto MISMATCH_COST = construct_mismatch_cost();

  // head/tail output size
  const int OUTPUT_SIZE = 50;

  // generate string
  string s1, s2;
  vector<int> insert_pos1, insert_pos2;
  process_input(input_filename, s1, s2, insert_pos1, insert_pos2);
  generate_string(s1, insert_pos1);
  generate_string(s2, insert_pos2);

  // results
  string s1_matched, s2_matched;
  s1_matched.reserve((size_t) pow(2, insert_pos1.size() + 2));
  s2_matched.reserve((size_t) pow(2, insert_pos2.size() + 2));

  // print parameters
  cout << "Program parameters:\n\ts1=" << s1 << "\n\ts2=" << s2 << endl;
  cout << "s.size=" << s1.size() << "," << s2.size() << endl;
  print_mismatch_cost(MISMATCH_COST);

  // ready to run algorithm
  test_dynamic_programming(
    s1_matched, s2_matched, s1, s2, MISMATCH_COST, GAP_COST);
  test_forward_backward(s1, s2, MISMATCH_COST, GAP_COST);

//  divide_conquer_alignment(s1, 0, (int) s1.size(), s2, 0, (int) s2.size(), MISMATCH_COST, GAP_COST, 0);

  return 0;
}


//void
//divide_conquer_alignment(string & s1, int s1_begin, int s1_end,
//                         string & s2, int s2_begin, int s2_end,
//                         std::unordered_map<char, std::unordered_map<char, int>> & mismatch_cost,
//                         const int gap_cost, int depth) {
//  for (int i = 0; i < depth; i++) { cout << "  "; }
//
//  cout << "[divide_conquer_alignment] s1@[" << s1_begin << "," << s1_end
//       << "), s2@[" << s2_begin << "," << s2_end << ")" << endl;
//  if (s1_end - s1_begin <= 2 || s2_end - s2_begin <= 2) {
//    string s1_matched, s2_matched;
//    s1_matched.reserve((size_t) pow(2, s1_end - s1_begin + 2));
//    s2_matched.reserve((size_t) pow(2, s2_end - s2_begin + 2));
//
//    auto min_cost = dynamic_programming(s1, s2, mismatch_cost, gap_cost);
//    dynamic_programming_find_alignment(s1_matched, s2_matched, min_cost, s1, s2,
//                                       mismatch_cost, gap_cost);
//
//    for (int i = 0; i < depth; i++) { cout << "  "; }
//
//    cout << "  Calling dynamic_programming, get s1="
//         << s1.substr(s1_begin, s1_end - s1_begin) << ", s2="
//         << s2.substr(s2_begin, s2_end - s2_begin) << endl;
//    return;
//  }
//
//  int i_separator = (s1_begin + s1_end) / 2;
//
//  for (int i = 0; i < depth; i++) { cout << "  "; }
//
//  cout << "  i_separator=" << i_separator << endl;
//
//  auto min_cost_forward = dynamic_programming_space_efficient(
//    s1, s1_begin, i_separator,
//    s2, s2_begin, s2_end,
//    mismatch_cost, gap_cost);
//  auto min_cost_backward = dynamic_programming_space_efficient_backward(
//    s1, i_separator, s1_end,
//    s2, s2_begin, s2_end,
//    mismatch_cost, gap_cost);
//
//  int min_c = numeric_limits<int>::max();
//  int i_min = -1;
//  for (int i = 0; i < min_cost_forward.size(); i++) {
//    int curr_c = min_cost_forward[i] + min_cost_backward[i];
//    if (curr_c < min_c) {
//      min_c = curr_c;
//      i_min = i;
//    }
//  }
//  for (int i = 0; i < depth; i++) { cout << "  "; }
//
//  cout << "  found mim_c=" << min_c << "@" << i_min << endl;
//  divide_conquer_alignment(s1, s1_begin, i_separator, s2, s2_begin, i_min,
//                           mismatch_cost, gap_cost, depth + 1);
//  divide_conquer_alignment(s1, i_separator, s1_end, s2, i_min + 1, s2_end,
//                           mismatch_cost, gap_cost, depth + 1);
//}

void test_dynamic_programming(
  std::string & s1_matched, std::string & s2_matched,
  std::string & s1, std::string & s2,
  std::unordered_map<char, std::unordered_map<char, int>> & mismatch_cost,
  int gap_cost) {

  auto min_cost = dynamic_programming(s1, s2, mismatch_cost, gap_cost);

  dynamic_programming_find_alignment(
    s1_matched, s2_matched, min_cost, s1, s2, mismatch_cost, gap_cost);

  // algorithm end
  cout << "s1_matched=" << s1_matched << endl;
  cout << "s2_matched=" << s2_matched << endl;

  trace_back(min_cost, s1, s2, mismatch_cost, gap_cost);
}


void test_forward_backward(
  const std::string & s1, const std::string & s2,
  std::unordered_map<char, std::unordered_map<char, int>> & mismatch_cost,
  int gap_cost) {

  for (int separator = 0; separator < s1.size(); ++separator) {
    auto min_cost_forward = dynamic_programming_space_efficient(
      s1, 0, separator,
      s2, 0, (int) s2.size(),
      mismatch_cost, gap_cost);
    auto min_cost_backward = dynamic_programming_space_efficient_backward(
      s1, separator, (int) s1.size(),
      s2, 0, (int) s2.size(),
      mismatch_cost, gap_cost);
    int min_c = numeric_limits<int>::max();
    int i_min = -1;
    for (int i = 0; i < min_cost_forward.size(); i++) {
      int curr_c = min_cost_forward[i] + min_cost_backward[i];
      if (curr_c < min_c) {
        min_c = curr_c;
        i_min = i;
      }
    }
    cout << "[test_forward_backward] found node@(" << separator << ", " << i_min << ") with cost " << min_c << endl;
  }
}