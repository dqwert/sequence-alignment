#ifndef SEQUENCE_ALIGNMENT_DYNAMIC_PROGRAMMING_ALGORITHM_H
#define SEQUENCE_ALIGNMENT_DYNAMIC_PROGRAMMING_ALGORITHM_H

#include <vector>
#include <string>
#include <unordered_map>


void dynamic_programming(std::vector<std::vector<int>> & min_cost,
                         const std::string & s1, const std::string & s2,
                         std::unordered_map<char, std::unordered_map<char, int>> & cost,
                         int gap_cost);

void dynamic_programming_trace_back(std::string & s1_matched,
                                    std::string & s2_matched,
                                    std::vector<std::vector<int>> & min_cost,
                                    const std::string & s1,
                                    const std::string & s2,
                                    std::unordered_map<char, std::unordered_map<char, int>> & mismatch_cost,
                                    int gap_cost);

#endif //SEQUENCE_ALIGNMENT_DYNAMIC_PROGRAMMING_ALGORITHM_H
