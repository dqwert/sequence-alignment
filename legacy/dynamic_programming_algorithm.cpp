#include "dynamic_programming_algorithm.h"

#include <iostream>
#include <iomanip>
#include <algorithm>
#include <limits>
#include <map>


void divide_conquer_alignment(
  std::string & s1, int s1_begin, int s1_end,
  std::string & s2, int s2_begin, int s2_end,
  std::string & s1_res, std::string & s2_res,
  std::unordered_map<char, std::unordered_map<char, int>> & mismatch_cost,
  const int gap_cost, int depth) {

  for (int i = 0; i < depth; i++) { std::cout << "  "; }
  std::cout << "[divide_conquer_alignment] str1@[" << s1_begin << "," << s1_end
            << "), str2@[" << s2_begin << "," << s2_end << "), depth=" << depth
            << std::endl;

  assert(s1_end >= s1_begin && s2_end >= s2_begin);

  if (s1_end - s1_begin <= 2 || s2_end - s2_begin <= 2) {

    for (int i = 0; i < depth; i++) { std::cout << "  "; }
    std::cout << "  Calling dynamic_programming, ";

    std::string s1_matched, s2_matched;
    s1_matched.reserve((size_t) pow(2, s1_end - s1_begin + 2));
    s2_matched.reserve((size_t) pow(2, s2_end - s2_begin + 2));

    std::string s1_seg = s1.substr(s1_begin, s1_end - s1_begin);
    std::string s2_seg = s2.substr(s2_begin, s2_end - s2_begin);

    auto min_cost = dynamic_programming(
      s1_seg, s2_seg,
      mismatch_cost, gap_cost);
    dynamic_programming_find_alignment(
      s1_matched, s2_matched, min_cost,
      s1_seg, s2_seg, mismatch_cost, gap_cost);

    for (int i = 0; i < depth; i++) { std::cout << "  "; }
    std::cout << "get str1="
              << s1.substr(s1_begin, s1_end - s1_begin) << ", str2="
              << s2.substr(s2_begin, s2_end - s2_begin)
              << ", matched str1=" << s1_matched << ", str2=" << s2_matched
              << std::endl;
//    trace_back(min_cost, s1_seg, s2_seg, mismatch_cost, gap_cost);
    s1_res.append(s1_matched);
    s2_res.append(s2_matched);
    return;
  }

  // select median char of str1 as the separator, find point on path,
  // then do the divide on str1 and str2, by (separator, index with min cost)
  int i_separator = (s1_begin + s1_end) / 2;

  auto min_cost_forward = dynamic_programming_space_efficient(
    s1, s1_begin, i_separator,
    s2, s2_begin, s2_end,
    mismatch_cost, gap_cost);

  auto min_cost_backward = dynamic_programming_space_efficient_backward(
    s1, i_separator, s1_end,
    s2, s2_begin, s2_end,
    mismatch_cost, gap_cost);


  int min_c = std::numeric_limits<int>::max();
  int i_min_cost = -1;
  for (int i = 0; i < min_cost_forward.size(); i++) {
    int curr_c = min_cost_forward[i] + min_cost_backward[i];
    if (curr_c < min_c) {
      min_c = curr_c;
      i_min_cost = i;
    }
    std::cout << curr_c << ",";
  }
  std::cout << std::endl;

  i_min_cost += s2_begin;

  for (int i = 0; i < depth; i++) { std::cout << "  "; }
  std::cout << "  found node@("
            << i_separator << ", " << i_min_cost << ") with cost=" << min_c
            << ", min_cost_forward/backward.size=" << min_cost_forward.size()
            << ", "
            << min_cost_backward.size() << std::endl;


  divide_conquer_alignment(s1, s1_begin, i_separator + 1,
                           s2, s2_begin, i_min_cost + 1,
                           s1_res, s2_res,
                           mismatch_cost, gap_cost, depth + 1);
  divide_conquer_alignment(s1, i_separator + 1, s1_end,
                           s2, i_min_cost + 1, s2_end,
                           s1_res, s2_res,
                           mismatch_cost, gap_cost, depth + 1);
}


std::vector<std::vector<int>> dynamic_programming(
  const std::string & s1, const std::string & s2,
  std::unordered_map<char, std::unordered_map<char, int>> & cost,
  const int gap_cost) {

  std::vector<std::vector<int>> min_cost(s1.size() + 1,
                                         std::vector<int>(s2.size() + 1, 0));

  for (int i = 0; i <= s1.size(); i++) { min_cost[i][0] = i * gap_cost; }
  for (int j = 0; j <= s2.size(); j++) { min_cost[0][j] = j * gap_cost; }

  for (int i = 1; i <= s1.size(); i++) {
    for (int j = 1; j <= s2.size(); j++) {
      min_cost[i][j] = std::min(
        {gap_cost + min_cost[i - 1][j], gap_cost + min_cost[i][j - 1],
         min_cost[i - 1][j - 1] +
         cost[s1[i - 1]][s2[j - 1]]});
    }
  }
  return min_cost;
}


void dynamic_programming_find_alignment(
  std::string & s1_matched, std::string & s2_matched,
  std::vector<std::vector<int>> & min_cost,
  const std::string & s1,
  const std::string & s2,
  std::unordered_map<char, std::unordered_map<char, int>> & mismatch_cost,
  int gap_cost) {

  std::cout << "[dynamic_programming_find_alignment] s.size=" << s1.size()
            << "," << s2.size() << std::endl;

  for (int i = (int) s1.size(), j = (int) s2.size(); i >= 0 && j >= 0;) {
    if (i != 0 && j != 0) {
      if (abs(min_cost[i][j]) ==
          abs(min_cost[i - 1][j - 1]) + mismatch_cost[s1[i - 1]][s2[j - 1]]) {
        min_cost[i][j] = -min_cost[i][j];

        s1_matched.push_back(s1[--i]);
        s2_matched.push_back(s2[--j]);
      } else if (abs(min_cost[i][j]) == abs(min_cost[i][j - 1]) + gap_cost) {
        min_cost[i][j] = -min_cost[i][j];

        s1_matched.push_back('_');
        s2_matched.push_back(s2[--j]);
      } else if (abs(min_cost[i][j]) == abs(min_cost[i - 1][j]) + gap_cost) {
        min_cost[i][j] = -min_cost[i][j];

        s1_matched.push_back(s1[--i]);
        s2_matched.push_back('_');
      } else { assert(false); }
    } else {  // i == 0 || j == 0
      if (i == 0 && j == 0) {
        break;
      } else if (i == 0) {
        min_cost[i][j] = -min_cost[i][j];

        s1_matched.push_back('_');
        s2_matched.push_back(s2[--j]);
      } else {
        min_cost[i][j] = -min_cost[i][j];

        s1_matched.push_back(s1[--i]);
        s2_matched.push_back('_');
      }
    }
  }
  std::reverse(s1_matched.begin(), s1_matched.end());
  std::reverse(s2_matched.begin(), s2_matched.end());
}


std::vector<int>
dynamic_programming_space_efficient(
  const std::string & s1, int s1_begin, int s1_end,
  const std::string & s2, int s2_begin, int s2_end,
  std::unordered_map<char, std::unordered_map<char, int>> & mismatch_cost,
  int gap_cost) {

  std::vector<int> min_cost(s2_end - s2_begin + 1, 0);
  for (int j = 0; j <= s2_end - s2_begin; j++) { min_cost[j] = j * gap_cost; }

  for (int i = 1; i <= s1_end - s1_begin; i++) {
    int min_cost_prev = (i - 1) * 30;  // min_cost[i - 1][j - 1]
    min_cost[0] = i * 30;
    for (int j = 1; j <= s2_end - s2_begin; j++) {
      int min_cost_curr = min_cost[j];
      min_cost[j] = std::min(
        {gap_cost + min_cost[j - 1], gap_cost + min_cost[j],
         min_cost_prev + mismatch_cost[s1[i - 1]][s2[j - 1]]});
      min_cost_prev = min_cost_curr;
    }
  }
  return min_cost;
}


std::vector<int>
dynamic_programming_space_efficient_backward(
  const std::string & s1,
  int s1_begin, int s1_end,
  const std::string & s2,
  int s2_begin, int s2_end,
  std::unordered_map<char, std::unordered_map<char, int>> & mismatch_cost,
  int gap_cost) {
  std::vector<int> min_cost(s2_end - s2_begin + 1, 0);
  for (int j = s2_end - s2_begin; j >= 0; j--) {
    min_cost[j] = (s2_end - s2_begin - j) * gap_cost;
  }

  for (int i = s1_end - s1_begin - 1; i >= 0; i--) {
    int min_cost_prev =
      (s1_end - s1_begin - i - 1) * 30;  // min_cost[i + 1][j + 1]
    min_cost[s2_end - s2_begin] = (s1_end - s1_begin - i) * 30;
    for (int j = s2_end - s2_begin - 1; j >= 0; j--) {
      int min_cost_curr = min_cost[j];
      min_cost[j] = std::min(
        {gap_cost + min_cost[j + 1], gap_cost + min_cost[j],
         min_cost_prev + mismatch_cost[s1[s1_begin + i]][s2[s2_begin + j]]});
      min_cost_prev = min_cost_curr;
    }
  }
  return min_cost;
}


void trace_back(
  std::vector<std::vector<int>> & min_cost, std::string & s1,
  std::string & s2,
  std::unordered_map<char, std::unordered_map<char, int>> mismatch_cost,
  int gap_cost) {
  std::cout << "[trace_back]" << std::endl;
  for (int i = 0, s1_size = (int) s1.size(); i <= s1_size; i++) {
    for (int j = 0, s2_size = (int) s2.size(); j <= s2_size; j++) {
      if (i != 0 && j != 0) {
        if (abs(min_cost[i][j]) ==
            abs(min_cost[i - 1][j - 1]) + mismatch_cost[s1[i - 1]][s2[j - 1]]) {
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
      if (min_cost[i][j] < 0) {
        std::cout << "[" << std::setw(4) << abs(min_cost[i][j]) << "]";
      } else {
        std::cout << std::setw(5) << min_cost[i][j] << " ";
      }
    }
    std::cout << std::endl;
  }
}
