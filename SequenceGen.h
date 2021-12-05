#ifndef SEQUENCE_ALIGNMENT_SEQUENCEGEN_H
#define SEQUENCE_ALIGNMENT_SEQUENCEGEN_H

#include <iostream>
#include <fstream>
#include <filesystem>
#include <vector>
#include <cmath>
#include <unordered_map>

void process_input(const std::string & input_filename, std::string & s1, std::string & s2,
                   std::vector<int> & insert_pos1,
                   std::vector<int> & insert_pos2);

void generate_string(std::string & s, const std::vector<int> & insert_pos);

std::unordered_map<char, std::unordered_map<char, int>> construct_mismatch_cost();

void print_mismatch_cost(std::unordered_map<char, std::unordered_map<char, int>> & cost);

void trace_back(std::vector<std::vector<int>> & min_cost, std::string & s1, std::string & s2,
                std::unordered_map<char, std::unordered_map<char, int>> mismatch_cost,
                int gap_cost);



#endif //SEQUENCE_ALIGNMENT_SEQUENCEGEN_H
