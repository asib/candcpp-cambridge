#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <numeric>
#include <stdlib.h>

#define NON_POSITIVE_COLUMN_NUMBER 1
#define INFILE_NOT_OPEN 2
#define COLUMN_NUMBER_GREATER_THAN_NUMBER_OF_COLUMNS 3
#define INCORRECT_ARGUMENTS 4
#define OUTFILE_NOT_OPEN 5

void print_usage() {
  std::cout << "Usage: csvorder <file> <delim> <col> <i|s>" << std::endl;
}

void split_insert_line(std::vector< std::vector<std::string> >& lines,
    std::string& line, char delim) {
  // Split this line into a vector of tokens.
  std::istringstream iss(line);
  std::string token;
  std::vector<std::string> cols;
  while (std::getline(iss, token, delim)) {
    cols.push_back(token);
  }

  // Now insert this vector of tokens into the vector of lines provided.
  lines.push_back(cols);
}

int main(int argc, char *argv[]) {
  if (argc != 5) {
    print_usage();
    return INCORRECT_ARGUMENTS;
  }

  char *ifname = argv[1];
  char delim = *(argv[2]);
  int col_num = atoi(argv[3]);
  bool is_int = (*(argv[4]) == 'i'); // If column is integers, give 'i' as last arg.

  if (col_num < 1) {
    std::cout << "Column number must be positive." << std::endl;
    return NON_POSITIVE_COLUMN_NUMBER;
  }

  std::ifstream ifs(ifname);
  if (!ifs.is_open()) {
    std::cout << "Couldn't open input file <" << ifname << ">" << std::endl;
    return INFILE_NOT_OPEN;
  }

  // Open out file.
  std::string ofname(ifname);
  ofname += "_OUT";
  std::ofstream ofs(ofname);
  if (!ofs.is_open()) {
    std::cout << "Couldn't open output file <" << ofname << ">" << std::endl;
    return OUTFILE_NOT_OPEN;
  }

  std::vector< std::vector<std::string> > csv_lines;

  // Get first line to check that column number is valid (<= number of columns).
  std::string line;
  std::getline(ifs, line);
  split_insert_line(csv_lines, line, delim);
  if (col_num > csv_lines[0].size()) {
    std::cout << "Column number must be less than number of columns." << std::endl;
    return COLUMN_NUMBER_GREATER_THAN_NUMBER_OF_COLUMNS;
  }

  // Read all lines into vector.
  while (std::getline(ifs, line)) {
    split_insert_line(csv_lines, line, delim);
  }

  // Close input file.
  ifs.close();

  // Sort input lines by provided column number, converting to integers if requested.
  std::sort(csv_lines.begin(), csv_lines.end(), [=](std::vector<std::string> s1,
    std::vector<std::string> s2) {
      if (!is_int)
        return s1[col_num-1] < s2[col_num-1];
      else
        return atoi(s1[col_num-1].c_str()) < atoi(s2[col_num-1].c_str());
    });

  // Write result to output file.
  for (std::vector<std::string> &s : csv_lines) {
    ofs << std::accumulate(std::next(s.begin()), s.end(), s[0], [=](std::string a,
      std::string b) { return a + delim + b; }) << std::endl;
  }

  // Close output file.
  ofs.close();
}
