#include "dynamic_programming_algorithm.h"

#include <algorithm>

void dynamic_programming(std::vector<std::vector<int>> & min_cost,
                         const std::string & s1, const std::string & s2,
                         std::unordered_map<char, std::unordered_map<char, int>> & cost,
                         const int gap_cost) {
  for (int i = 0; i <= s1.size(); i++) { min_cost[i][0] = i * gap_cost; }
  for (int j = 0; j <= s2.size(); j++) { min_cost[0][j] = j * gap_cost; }

  for (int i = 1; i <= s1.size(); i++) {
    for (int j = 1; j <= s2.size(); j++) {
      min_cost[i][j] =
        gap_cost + std::min(min_cost[i - 1][j], min_cost[i][j - 1]);
      min_cost[i][j] = std::min(min_cost[i][j],
                                min_cost[i - 1][j - 1] +
                                cost[s1[i - 1]][s2[j - 1]]);
    }
  }
}


void dynamic_programming_trace_back(std::string & s1_matched,
                                    std::string & s2_matched,
                                    std::vector<std::vector<int>> & min_cost,
                                    const std::string & s1,
                                    const std::string & s2,
                                    std::unordered_map<char, std::unordered_map<char, int>> & mismatch_cost,
                                    const int gap_cost) {
  for (int i = (int) s1.size(), j = (int) s2.size(), k = 100;
       k > 0 && i >= 0 && j >= 0; k--) {
    if (i != 0 && j != 0) {
      if (abs(min_cost[i][j]) ==
          abs(min_cost[i - 1][j - 1]) + mismatch_cost[s1[i - 1]][s2[j - 1]]) {
//        std::cout << std::setw(6) << " ↖";
        min_cost[i][j] = -min_cost[i][j];

        s1_matched.push_back(s1[--i]);
        s2_matched.push_back(s2[--j]);
      } else if (abs(min_cost[i][j]) == abs(min_cost[i][j - 1]) + gap_cost) {
//        std::cout << std::setw(6) << " ←";
        min_cost[i][j] = -min_cost[i][j];

        s1_matched.push_back('_');
        s2_matched.push_back(s2[--j]);
      } else if (abs(min_cost[i][j]) == abs(min_cost[i - 1][j]) + gap_cost) {
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
}