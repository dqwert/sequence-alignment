#include "sequence_utils.h"
#include "dynamic_programming_algorithm.h"

#include <limits>
#include <iostream>

using namespace std;


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

  // -------------------- ready to run algorithm --------------------

//  test_dynamic_programming(s1_matched, s2_matched, str1, str2, MISMATCH_COST,
//                           GAP_COST);
//  test_forward_backward(str1, str2, MISMATCH_COST, GAP_COST);

  divide_conquer_alignment(
    s1, 0, (int) s1.size(), s2, 0, (int) s2.size(),
    s1_matched, s2_matched,
    MISMATCH_COST, GAP_COST, 0);

  cout << alignment_cost("ACACACTGACTACTGACTGGTGACTACTGACTGGACTGACTACTGACTGGTGACTAC_TGACTG_G", "______T___TA_T___T____A_TAC_G_C_G_AC_G_C____GA_T__T_A_TACGCGAC_GCG", MISMATCH_COST) << endl;
  // --------------------   algorithm finished   --------------------

  cout << "s1_res=" << s1_matched << "\ns2_res=" << s2_matched << endl;

  return 0;
}
