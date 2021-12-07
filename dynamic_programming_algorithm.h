#ifndef SEQUENCE_ALIGNMENT_DYNAMIC_PROGRAMMING_ALGORITHM_H
#define SEQUENCE_ALIGNMENT_DYNAMIC_PROGRAMMING_ALGORITHM_H

#include <vector>
#include <string>
#include <unordered_map>


std::vector<std::vector<int>> dynamic_programming(
  const std::string & s1, const std::string & s2,
  std::unordered_map<char, std::unordered_map<char, int>> & cost,
  int gap_cost);


void dynamic_programming_find_alignment(std::string & s1_matched,
                                        std::string & s2_matched,
                                        std::vector<std::vector<int>> & min_cost,
                                        const std::string & s1,
                                        const std::string & s2,
                                        std::unordered_map<char, std::unordered_map<char, int>> & mismatch_cost,
                                        int gap_cost);


std::vector<int>
dynamic_programming_space_efficient(
  const std::string & s1, int s1_begin, int s1_end,
  const std::string & s2, int s2_begin, int s2_end,
  std::unordered_map<char, std::unordered_map<char, int>> & mismatch_cost,
  int gap_cost);


std::vector<int>
dynamic_programming_space_efficient_backward(
  const std::string & s1,
  int s1_begin, int s1_end,
  const std::string & s2,
  int s2_begin, int s2_end,
  std::unordered_map<char, std::unordered_map<char, int>> & mismatch_cost,
  int gap_cost);


void trace_back(
  std::vector<std::vector<int>> & min_cost, std::string & s1, std::string & s2,
  std::unordered_map<char, std::unordered_map<char, int>> mismatch_cost,
  int gap_cost);


#endif //SEQUENCE_ALIGNMENT_DYNAMIC_PROGRAMMING_ALGORITHM_H
