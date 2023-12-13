/*
EC311 - F23 - Final Project
Group XYZ

XYZ Assembler
*/

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <bitset>     //https://en.cppreference.com/w/cpp/utility/bitset

using namespace std;

/*
struct to store 8 bit chunks of an instruction
*/
struct instruction{
  bitset<8> opcode;
  bitset<8> param1;
  bitset<8> param2;
};

/*
@param filename : filename where errors were found
@param line_nums : vector containing line numbers where errors were found

prints line numbers where error occurs to warn user of syntax issues
*/
void printErrors(string filename, vector<int> line_nums) {
  cout << endl << "Program did not assemble properly!" << endl;
  cout << "Syntax error(s) in file: " << filename << endl;
  cout << "Error(s) in line(s): ";
  for (int num : line_nums) {
    cout << num << " ";
  }
  cout << endl;
}

/*
@param reg : string of register to be checked

returns whether register actually exists (i.e. between R0 - R7)
ensures user doesn't try to access registers that don't exist
*/
bool isValidRegister(string reg) {
  string first_letter = reg.substr(0, 1);
  int reg_num = stoi(reg.substr(1));
  return (reg_num >= 0 && reg_num <= 7 && first_letter == "R");
}

/*
@param instr : entire instruction in string format

returns a vector of all components of the instruction including
the name and any operands.
*/
vector<string> splitInstruction(string instr) {
  vector<string> instruction = vector<string>();

  /* code adapted from: https://www.javatpoint.com/how-to-split-strings-in-cpp */
  int currIndex = 0, i = 0;
  int startIndex = 0, endIndex = 0;
  while (i <= instr.length()) {
    if (instr[i] == ' ' || i == instr.length()) {
      endIndex = i;
      string subStr = "";
      subStr.append(instr, startIndex, endIndex - startIndex);
      instruction.push_back(subStr);
      currIndex += 1;
      startIndex = endIndex + 1;
    }
    i++;
  }

  return instruction;
}

/*
@param filename : string of filename containing the XYZ assembly code

returns of a vector of instruction structs where each element of the
vector is a single 24 bit instruction.
*/
vector<instruction> createInstructionVector(string filename) {
  
  ifstream file;
  file.open(filename);

  // vector to store all instructions
  vector<instruction> program;  

  // syntax-error detection variables
  vector<int> syntax_error_lines = vector<int>();
  int line_num = 0;

  string line;
  while (getline(file, line)) {
    line_num++;
    vector<string> curr_instr = splitInstruction(line);
    
    string curr_instr_name = curr_instr[0];
    if (curr_instr_name == "halt") {

      if (curr_instr.size() != 1) { syntax_error_lines.push_back(line_num);; }
      else { program.push_back(instruction({0b00000000, 0b00000000, 0b00000000})); }

    } else if (curr_instr_name == "rstregs") {

      if (curr_instr.size() != 1) { syntax_error_lines.push_back(line_num);; }
      else { program.push_back(instruction({0b00000001, 0b00000000, 0b00000000})); }

    } else if (curr_instr_name == "mov") {
      if (curr_instr.size() == 3) {
        // check which mov operands are being used.
        if (curr_instr[1].substr(0, 1) == "R" && curr_instr[2].substr(0, 1) != "R" && curr_instr[2].substr(0, 1) != "[") { // mov Rn num
          program.push_back(instruction({0b00000010, stoi(curr_instr[1].substr(1, 1)), stoi(curr_instr[2])}));
        } 
        else if (curr_instr[1].substr(0, 1) == "R" && curr_instr[2].substr(0, 1) == "R") {  // mov Rn Rm
          program.push_back(instruction({0b00000011, stoi(curr_instr[1].substr(1, 1)), stoi(curr_instr[2].substr(1, 1))}));
        } 
        else if (curr_instr[1].substr(0, 1) == "[" && curr_instr[2].substr(0, 1) == "R") {  // mov [Rn] Rm
          program.push_back(instruction({0b00000100, stoi(curr_instr[1].substr(2, 1)), stoi(curr_instr[2].substr(1, 1))}));
        } 
        else if (curr_instr[1].substr(0, 1) == "R" && curr_instr[2].substr(0, 1) == "[") {   // mov Rn [Rm]
          program.push_back(instruction({0b00000101, stoi(curr_instr[1].substr(1, 1)), stoi(curr_instr[2].substr(2, 1))}));
        } else {
          syntax_error_lines.push_back(line_num);
        }
      } else {
        syntax_error_lines.push_back(line_num);
      }

    } else if (curr_instr_name == "add") {

      if (curr_instr.size() != 3 || !isValidRegister(curr_instr[1]) || !isValidRegister(curr_instr[2])) { 
        syntax_error_lines.push_back(line_num); 
      } else {
        program.push_back(instruction({0b00000110, stoi(curr_instr[1].substr(1)), stoi(curr_instr[2].substr(1))}));
      }

    } else if (curr_instr_name == "sub") {

      if (curr_instr.size() != 3 || !isValidRegister(curr_instr[1]) || !isValidRegister(curr_instr[2])) { 
        syntax_error_lines.push_back(line_num); 
      } else {
        program.push_back(instruction({0b00000111, stoi(curr_instr[1].substr(1)), stoi(curr_instr[2].substr(1))}));
      }

    } else if (curr_instr_name == "inc") {

      if (curr_instr.size() != 2 || !isValidRegister(curr_instr[1])) { 
        syntax_error_lines.push_back(line_num); 
      } else {
        program.push_back(instruction({0b00001000, stoi(curr_instr[1].substr(1)), 0b00000000}));
      }

    } else if (curr_instr_name == "dec") {

      if (curr_instr.size() != 2 || !isValidRegister(curr_instr[1])) { 
        syntax_error_lines.push_back(line_num); 
      } else {
        program.push_back(instruction({0b00001001, stoi(curr_instr[1].substr(1)), 0b00000000}));
      }

    } else if (curr_instr_name == "and") {

      if (curr_instr.size() != 3 || !isValidRegister(curr_instr[1]) || !isValidRegister(curr_instr[2])) { 
        syntax_error_lines.push_back(line_num); 
      } else {
        program.push_back(instruction({0b00001010, stoi(curr_instr[1].substr(1)), stoi(curr_instr[2].substr(1))}));
      }

    } else if (curr_instr_name == "or") {

      if (curr_instr.size() != 3 || !isValidRegister(curr_instr[1]) || !isValidRegister(curr_instr[2])) { 
        syntax_error_lines.push_back(line_num); 
      } else {
        program.push_back(instruction({0b00001011, stoi(curr_instr[1].substr(1)), stoi(curr_instr[2].substr(1))}));
      }

    } else if (curr_instr_name == "xor") {

      if (curr_instr.size() != 3 || !isValidRegister(curr_instr[1]) || !isValidRegister(curr_instr[2])) { 
        syntax_error_lines.push_back(line_num); 
      } else {
        program.push_back(instruction({0b00001100, stoi(curr_instr[1].substr(1)), stoi(curr_instr[2].substr(1))}));
      }

    } else if (curr_instr_name == "cmp") {

      if (curr_instr.size() != 3 || !isValidRegister(curr_instr[1]) || !isValidRegister(curr_instr[2])) { 
        syntax_error_lines.push_back(line_num); 
      } else {
        program.push_back(instruction({0b00001101, stoi(curr_instr[1].substr(1)), stoi(curr_instr[2].substr(1))}));
      }

    } else if (curr_instr_name == "jmp") {

      if (curr_instr.size() != 2) { syntax_error_lines.push_back(line_num); }
      else { program.push_back(instruction({0b00001110, stoi(curr_instr[1]), 0b00000000})); }

    } else if (curr_instr_name == "je") {

      if (curr_instr.size() != 2) { syntax_error_lines.push_back(line_num); }
      else { program.push_back(instruction({0b00001111, stoi(curr_instr[1]), 0b00000000})); }

    } else if (curr_instr_name == "jne") {

      if (curr_instr.size() != 2) { syntax_error_lines.push_back(line_num); }
      else { program.push_back(instruction({0b00010000, stoi(curr_instr[1]), 0b00000000})); }

    } else {
      syntax_error_lines.push_back(line_num);
    }
  }

  file.close();

  if (!syntax_error_lines.empty()) {
    printErrors(filename, syntax_error_lines);
    return vector<instruction>(); // return empty vector so program does not continue
  }

  return program;
}

/*
@param filename : name of file from which program was loaded
@param program : vector of instructions that define the program

writes all instructions from program into a new file titled [filename].BIN
*/
void writeToBinFile(string filename, vector<instruction> program) {

  if (!program.empty()) {
    
    string bin_filename = filename.substr(0, filename.find(".")) + ".BIN";
    ofstream output_file(bin_filename);

    if (output_file.is_open()) {
      for (instruction instr : program) {
        output_file << instr.opcode << instr.param1 << instr.param2;
      }
      cout << bin_filename << " is ready!" << endl;
    } else {
      cout << "Error opening " << bin_filename << "." << endl;
    }

    output_file.close();
  }

}

int main(int argc, const char * argv[]) {
    
    if (argc != 2)
    {
        cout << "Please supply a file name as input" << endl;
        return -1;
    }

    // create binary code of program
    vector<instruction> program;
    program = createInstructionVector(argv[1]);

    // write program to .bin file
    writeToBinFile(argv[1], program);

    return 0;
}

