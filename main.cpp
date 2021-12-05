#include "SequenceGen.h"

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

  for (int i = 0; i <= s1.size(); i++) { min_cost[i][0] = i * GAP_COST; }
  for (int j = 0; j <= s2.size(); j++) { min_cost[0][j] = j * GAP_COST; }

  for (int i = 1; i <= s1.size(); i++) {
    for (int j = 1; j <= s2.size(); j++) {
      min_cost[i][j] = GAP_COST + min(min_cost[i - 1][j], min_cost[i][j - 1]);
      min_cost[i][j] = min(min_cost[i][j],
                           min_cost[i - 1][j - 1] +
                           MISMATCH_COST[s1[i - 1]][s2[j - 1]]);
    }
  }


  string s1_matched, s2_matched;
  for (int i = (int) s1.size(), j = (int) s2.size(), k = 100;
       k > 0 && i >= 0 && j >= 0; k--) {
    if (i != 0 && j != 0) {
      if (abs(min_cost[i][j]) ==
          abs(min_cost[i - 1][j - 1]) + MISMATCH_COST[s1[i - 1]][s2[j - 1]]) {
//        std::cout << std::setw(6) << " ↖";
        min_cost[i][j] = -min_cost[i][j];

        s1_matched.push_back(s1[--i]);
        s2_matched.push_back(s2[--j]);
      } else if (abs(min_cost[i][j]) == abs(min_cost[i][j - 1]) + GAP_COST) {
//        std::cout << std::setw(6) << " ←";
        min_cost[i][j] = -min_cost[i][j];

        s1_matched.push_back('_');
        s2_matched.push_back(s2[--j]);
      } else if (abs(min_cost[i][j]) == abs(min_cost[i - 1][j]) + GAP_COST) {
//        std::cout << std::setw(6) << " ↑";
        min_cost[i][j] = -min_cost[i][j];

        s1_matched.push_back(s1[--i]);
        s2_matched.push_back('_');
      } else { assert(false); }
    } else {  // i == 0 || j == 0
      if (i == 0 && j == 0) {
//        std::cout << std::setw(6) << " ↘";
      } else if (i == 0) {
//        std::cout << std::setw(6) << " ←";
        min_cost[i][j] = -min_cost[i][j];

        s1_matched.push_back('_');
        s2_matched.push_back(s2[--j]);
      } else {
//        std::cout << std::setw(6) << " ↑";
        min_cost[i][j] = -min_cost[i][j];

        s1_matched.push_back(s1[--i]);
        s2_matched.push_back('_');
      }
    }
  }
  cout << endl;

  trace_back(min_cost, s1, s2, MISMATCH_COST, GAP_COST);


  std::reverse(s1_matched.begin(), s1_matched.end());
  std::reverse(s2_matched.begin(), s2_matched.end());
  cout << "s1_matched=" << s1_matched << endl;
  cout << "s2_matched=" << s2_matched << endl;

  return 0;
}
