#include "sequence_utils.h"
#include "dynamic_programming_algorithm.h"


void test_dynamic_programming(
  std::string & s1_matched, std::string & s2_matched,
  std::string & s1, std::string & s2,
  std::unordered_map<char, std::unordered_map<char, int>> & mismatch_cost,
  int gap_cost) {

  auto min_cost = dynamic_programming(s1, s2, mismatch_cost, gap_cost);

  dynamic_programming_find_alignment(
    s1_matched, s2_matched, min_cost, s1, s2, mismatch_cost, gap_cost);

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
    int min_c = std::numeric_limits<int>::max();
    int i_min = -1;
    for (int i = 0; i < min_cost_forward.size(); i++) {
      int curr_c = min_cost_forward[i] + min_cost_backward[i];
      if (curr_c < min_c) {
        min_c = curr_c;
        i_min = i;
      }
    }
    std::cout << "[test_forward_backward] found node@(" << separator << ", " << i_min
         << ") with cost " << min_c << std::endl;
  }
}


void process_input(const std::string & input_filename, std::string & s1,
                   std::string & s2, std::vector<int> & insert_pos1,
                   std::vector<int> & insert_pos2) {
  std::ifstream input(input_filename, std::ios_base::in);
  int pos;

  input >> s1;
  while (input >> pos) { insert_pos1.push_back(pos); }

  input.clear();

  input >> s2;
  while (input >> pos) { insert_pos2.push_back(pos); }

  input.close();
}


void generate_string(std::string & s, const std::vector<int> & insert_pos) {
  // allocate string of final size * 2
  s.reserve((size_t) pow(2, insert_pos.size() + 1) * s.size());
//  std::cout << "[generate_string] reserve string capacity of " <<  s.capacity() << std::endl;
  for (int pos: insert_pos) {
    s.insert(s.begin() + pos + 1, s.begin(), s.end());    // potential risk!
  }
}


std::unordered_map<char, std::unordered_map<char, int>>
construct_mismatch_cost() {
  return {
    {'A', {
            {'A', 0},   {'C', 110}, {'G', 48},  {'T', 94}}
    },
    {'C', {
            {'A', 110}, {'C', 0},   {'G', 118}, {'T', 48}}
    },
    {'G', {
            {'A', 48},  {'C', 118}, {'G', 0},   {'T', 110}}
    },
    {'T', {
            {'A', 94},  {'C', 48},  {'G', 110}, {'T', 0}}
    }
  };
}


void print_mismatch_cost(
  std::unordered_map<char, std::unordered_map<char, int>> & cost) {
  std::cout << "[print_mismatch_cost] cost table:\n";
  std::string s = "ACGT";
  for (char i: s) {
    for (char j: s) {
      std::cout << "\t" << cost[i][j];
    }
    std::cout << "\n";
  }
}


int alignment_cost(
  const std::string & s1, const std::string & s2,
  std::unordered_map<char, std::unordered_map<char, int>> & mismatch_cost) {

  int cost = 0;
  for (int i = 0; i < s1.size(); i++) {
    if (s1[i] == '_' || s2[i] == '_') { cost += 30; }
    else { cost += mismatch_cost[s1[i]][s2[i]]; }
  }
  return cost;
}
