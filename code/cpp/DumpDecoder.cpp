/*
EC311 - F23 - Final Project
Group XYZ

Memory Dump Decoder
*/

#include <iostream>
#include <vector>
#include <fstream>

using namespace std;

/*
@param filename : file where memory dump is stored

return a string consisting of the memory dump
*/
string getMemoryDump(string filename) {

  ifstream file;
  file.open(filename);

  string memory;
  getline(file, memory);

  file.close(); 

  return memory;
}

/*
@param memory : string consisting of memory dump

decode the memory and print accordingly
*/
void printMemoryDump(string memory) {

  // split every cell of memory
  vector<string> memory_cells = vector<string>();
  for (int i = 0; i < memory.length(); i += 24) {
    memory_cells.push_back(memory.substr(i, 24));
  }

  // convert memory cell value and print data in table format
  cout << "Memory Address" << "          " << "Data" << endl;
  cout << "----------------------------------------" << endl;
  for (int i = 0; i < memory_cells.size(); i++) {
    cout << "     " << i << "                 " << stoi(memory_cells[i], nullptr, 2) << endl;
  }

}

int main(int argc, const char * argv[]) {

  if (argc != 2)
  {
      cout << "Please supply a file name as input" << endl;
      return -1;
  }

  // get memory dump from inputted file
  string memory = getMemoryDump(argv[1]);

  // prints memory dump
  printMemoryDump(memory);

  return 0;
}
