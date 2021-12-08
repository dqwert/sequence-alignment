#ifndef SEQUENCE_ALIGNMENT_SEQUENCEALIGNMENT_HPP
#define SEQUENCE_ALIGNMENT_SEQUENCEALIGNMENT_HPP


#include <vector>
#include <string>
#include <unordered_map>
#include <limits>
#include <iostream>
#include <fstream>
#include <filesystem>
#include <cmath>


using namespace std;


class SequenceAlignment {
public:
  std::unordered_map<char, std::unordered_map<char, int>> mismatch_cost;
  int gap_cost = 30;
  string str1, str2;

  explicit SequenceAlignment(string & input_file);

  int alignment_cost(const std::string & s1, const std::string & s2);

  [[maybe_unused]] void print_mismatch_cost();

  static void
  generate_string(std::string & s, const std::vector<int> & insert_pos);

  static void
  generate_input(std::string & s1, std::string & s2, string & input_filename);

  std::vector<std::vector<int>> do_align_dynamic_programming(
    const std::string & s1, const std::string & s2);

  pair<string, string>
  find_alignment_dynamic_programming(const string & s1,
                                     const string & s2);

  std::vector<int> do_align_dynamic_programming_space_efficient(
    const std::string & s1, const std::string & s2);

  pair<string, string>
  divide_conquer_alignment(std::string s1, std::string s2, int depth);

  [[maybe_unused]] void trace_back(string s1, string s2, vector<std::vector<int>> & min_cost);

  static bool validator(const string & s1, const string & s2, const string & match1, const string & match2);
};


#endif //SEQUENCE_ALIGNMENT_SEQUENCEALIGNMENT_HPP
