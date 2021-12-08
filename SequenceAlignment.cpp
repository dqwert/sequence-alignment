#include "SequenceAlignment.hpp"


std::vector<std::vector<int>>
SequenceAlignment::do_align_dynamic_programming(const std::string & s1,
                                                const std::string & s2) {
  std::vector<std::vector<int>> min_cost(
    s2.size() + 1, std::vector<int>(s1.size() + 1, 0));

  for (int i = 0; i <= s2.size(); i++) { min_cost[i][0] = i * gap_cost; }
  for (int j = 0; j <= s1.size(); j++) { min_cost[0][j] = j * gap_cost; }

  for (int i = 1; i <= s2.size(); i++) {
    for (int j = 1; j <= s1.size(); j++) {
      min_cost[i][j] = std::min(
        {gap_cost + min_cost[i - 1][j],
         gap_cost + min_cost[i][j - 1],
         min_cost[i - 1][j - 1] + mismatch_cost[s2[i - 1]][s1[j - 1]]});
    }
  }
  return min_cost;
}


SequenceAlignment::SequenceAlignment(string & input_file) {
  mismatch_cost = {
    {'A', {{'A', 0},   {'C', 110}, {'G', 48},  {'T', 94}}},
    {'C', {{'A', 110}, {'C', 0},   {'G', 118}, {'T', 48}}},
    {'G', {{'A', 48},  {'C', 118}, {'G', 0},   {'T', 110}}},
    {'T', {{'A', 94},  {'C', 48},  {'G', 110}, {'T', 0}}}
  };

  generate_input(str1, str2, input_file);
//  cout << str1 << endl << str2 << endl;
  assert(!str1.empty());
  assert(!str2.empty());
}


void process_input(const std::string & input_filename,
                   std::string & s1, std::string & s2,
                   std::vector<int> & insert_pos1,
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


void SequenceAlignment::generate_input(std::string & s1, std::string & s2,
                                       string & input_filename) {
  vector<int> insert_pos1, insert_pos2;
  process_input(input_filename, s1, s2, insert_pos1, insert_pos2);
  generate_string(s1, insert_pos1);
  generate_string(s2, insert_pos2);
}


int SequenceAlignment::alignment_cost(const std::string & s1,
                                      const std::string & s2) {
  int cost = 0;
  for (int i = 0; i < s1.size(); i++) {
    if (s1[i] == '_' || s2[i] == '_') { cost += gap_cost; }
    else { cost += mismatch_cost[s1[i]][s2[i]]; }
  }
  return cost;
}


[[maybe_unused]] void SequenceAlignment::print_mismatch_cost() {
  std::cout << "[print_mismatch_cost] cost table:\n";
  std::string s = "ACGT";
  for (char i: s) {
    for (char j: s) {
      std::cout << "\t" << mismatch_cost[i][j];
    }
    std::cout << "\n";
  }
}


void SequenceAlignment::generate_string(std::string & s,
                                        const std::vector<int> & insert_pos) {
  // allocate string of final size * 2
  s.reserve((size_t) pow(2, insert_pos.size() + 1) * s.size());
  for (int pos: insert_pos) {
    s.insert(s.begin() + pos + 1, s.begin(), s.end());    // potential risk!
  }
}


pair<string, string>
SequenceAlignment::find_alignment_dynamic_programming(const string & s1,
                                                      const string & s2) {
//  std::cout << "[dynamic_programming_find_alignment] s.size=" << s1.size() << "," << s2.size() << std::endl;

  auto min_cost = do_align_dynamic_programming(s1, s2);

  string match1, match2;

  for (int i = (int) s2.size(), j = (int) s1.size(); i >= 0 && j >= 0;) {
    if (i != 0 && j != 0) {
      if (abs(min_cost[i][j]) ==
          abs(min_cost[i - 1][j - 1]) + mismatch_cost[s2[i - 1]][s1[j - 1]]) {
        min_cost[i][j] = -min_cost[i][j];

        match1.push_back(s1[--j]);
        match2.push_back(s2[--i]);
      } else if (abs(min_cost[i][j]) == abs(min_cost[i][j - 1]) + gap_cost) {
        min_cost[i][j] = -min_cost[i][j];

        match1.push_back(s1[--j]);
        match2.push_back('_');
      } else if (abs(min_cost[i][j]) == abs(min_cost[i - 1][j]) + gap_cost) {
        min_cost[i][j] = -min_cost[i][j];

        match1.push_back('_');
        match2.push_back(s2[--i]);
      } else { assert(false); }
    } else {  // i == 0 || j == 0
      if (i == 0 && j == 0) {
        break;
      } else if (i == 0) {
        min_cost[i][j] = -min_cost[i][j];

        match1.push_back(s1[--j]);
        match2.push_back('_');
      } else {  // if j ==0
        min_cost[i][j] = -min_cost[i][j];

        match1.push_back('_');
        match2.push_back(s2[--i]);
      }
    }
  }
  std::reverse(match1.begin(), match1.end());
  std::reverse(match2.begin(), match2.end());

//  trace_back(s1, s2, min_cost);

  return {match1, match2};
}


std::vector<int>
SequenceAlignment::do_align_dynamic_programming_space_efficient(
  const string & s1, const string & s2) {

  std::vector<int> min_cost(s1.size() + 1, 0);
  for (int j = 0; j <= s1.size(); j++) { min_cost[j] = j * gap_cost; }

  for (int j = 1; j <= s2.size(); j++) {
    int min_cost_prev = (j - 1) * gap_cost;  // min_cost[j - 1][j - 1]
    min_cost[0] = j * gap_cost;
    for (int i = 1; i <= s1.size(); i++) {
      int min_cost_curr = min_cost[i];
      min_cost[i] = std::min(
        {gap_cost + min_cost[i - 1], gap_cost + min_cost[i],
         min_cost_prev + mismatch_cost[s1[i - 1]][s2[j - 1]]});
      min_cost_prev = min_cost_curr;
    }
  }
  return min_cost;
}


[[maybe_unused]] void
SequenceAlignment::trace_back(string s1, string s2,
                              vector<std::vector<int>> & min_cost) {
  std::cout << "[trace_back]" << std::endl;
  for (int i = 0, s2_size = (int) s2.size(); i <= s2_size; i++) {
    for (int j = 0, s1_size = (int) s1.size(); j <= s1_size; j++) {
      if (i != 0 && j != 0) {
        if (abs(min_cost[i][j]) ==
            abs(min_cost[i - 1][j - 1]) + mismatch_cost[s2[i - 1]][s1[j - 1]]) {
          std::cout << std::setw(6) << " ↖";
        } else if (abs(min_cost[i][j]) == abs(min_cost[i][j - 1]) + gap_cost) {
          std::cout << std::setw(6) << " ←";
        } else if (abs(min_cost[i][j]) == abs(min_cost[i - 1][j]) + gap_cost) {
          std::cout << std::setw(6) << " ↑";
        } else { assert(false); }
      } else {
        if (i == 0 && j == 0) { std::cout << std::setw(6) << " ↘"; }
        else if (i == 0) { std::cout << std::setw(6) << " ↑"; }
        else { std::cout << std::setw(6) << " ←"; }
      }
      if (min_cost[i][j] <= 0) {
        std::cout << "[" << std::setw(4) << abs(min_cost[i][j]) << "]";
      } else {
        std::cout << std::setw(5) << min_cost[i][j] << " ";
      }
    }
    std::cout << std::endl;
  }
}


pair<string, string>
SequenceAlignment::divide_conquer_alignment(std::string s1, std::string s2,
                                            int depth) {
//  for (int i = 0; i < depth; i++) { std::cout << "  "; }
//  std::cout << "[divide_conquer_alignment] s1=" << s1 << ", s2=" << s2
//            << ", depth=" << depth << std::endl;


  if (s1.empty() || s2.empty()) {
    string match1, match2;
    if (s1.empty()) {
      for (int i = 0; i < s2.size(); i++) { match1.push_back('_'); }
      match2 = s2;
    } else {
      for (int i = 0; i < s1.size(); i++) { match2.push_back('_'); }
      match1 = s1;
    }
    return {match1, match2};
  } else if (s1.size() == 1 || s2.size() == 1) {
    return find_alignment_dynamic_programming(s1, s2);
  }

  // select median char of str1 as the separator, find point on path,
  // then do the divide on str1 and str2, by (separator, index with min cost)
  int i_separator = (int) s2.size() / 2;

  vector<int> min_cost_forward =
    do_align_dynamic_programming_space_efficient(s1, s2.substr(0, i_separator));

  string s1_reversed(s1.rbegin(), s1.rend());
  string s2_reversed(s2.rbegin(), s2.rend() - i_separator);

  vector<int> min_cost_backward =
    do_align_dynamic_programming_space_efficient(s1_reversed, s2_reversed);

  int min_cost = std::numeric_limits<int>::max();
  int i_min_cost = -1;
  for (int i = 0, n = (int) min_cost_forward.size(); i < n; i++) {
    int curr_c = min_cost_forward[i] + min_cost_backward[n - i - 1];
    if (curr_c < min_cost) {
      min_cost = curr_c;
      i_min_cost = i;
    }
//    cout << "[" << min_cost_forward[i] << "," << min_cost_backward[n - i - 1]<< "]@" << i << ",";
  }
//  cout << endl;

//  for (int i = 0; i < depth; i++) { std::cout << "  "; }
//  std::cout << "  found node@("
//            << i_separator << ", " << i_min_cost << ") with cost=" << min_cost
//            << ", min_cost.size=" << min_cost_forward.size()
//            << ", "
//            << min_cost_backward.size() << std::endl;

  pair<string, string> upper_left =
    divide_conquer_alignment(s1.substr(0, i_min_cost),
                             s2.substr(0, i_separator),
                             depth + 1);

  pair<string, string> lower_right =
    divide_conquer_alignment(s1.substr(i_min_cost),
                             s2.substr(i_separator),
                             depth + 1);

  return {upper_left.first + lower_right.first, upper_left.second + lower_right.second};
}


bool validate_match_to_string(const string & s, const string & match) {
  int i = 0;
  for (char c: s) {
    while (i < match.size() && match[i] == '_') { i++; }
    if (c != match[i++]) { return false; }
  }
  while (i < match.size() && match[i] == '_') { i++; }
  return i == match.size();
}


bool SequenceAlignment::validator(const string & s1,
                                  const string & s2,
                                  const string & match1,
                                  const string & match2) {
  return validate_match_to_string(s1, match1) && validate_match_to_string(s2, match2);
}
