
// Project Description: Program will ask the user for an file in HACK assembly language which it will then 
//                      use to produce an equivalent file in HACK machine language. 
// NOTE: The program is based on the "psuedo assembler" demonstrated during the video labs. So, while it is 
//       possible that it may fail some exception cases were not shown in the lab or that I did not consider,
//       it should be able to cover the same parameters as the aforementioned "pseudo assembler" in addition 
//       to a few extra file handling features.  

#include <iostream>
#include <math.h>
#include <unordered_map>
#include <string>
#include <fstream>
#include <ostream>
#include <vector>

std::unordered_map<std::string, int> symTable; 
std::unordered_map<std::string, std::string> compTable;
std::unordered_map<std::string, std::string> destTable;
std::unordered_map<std::string, std::string> jumpTable;

// converts a integer to a 15 bit binary string  
std::string decToBinary(int x)
{
    std::string bin = "";
    for (int i= 14; i >= 0; --i)
    {
        if ((x - pow(2,i) >= 0))
        {
            x = x-pow(2,i);
            bin = bin + "1";
        }
        else{bin=bin + "0";}
    }
    return bin;
}

// removes .asm extension from a string. returns error if no .asm extension 
std::string removeExt(std::string fileName)
{
    std::string actualName;
    if (fileName.find(".asm") == (fileName.size() - 4))
    {
        for (int i = 0; i < (fileName.size() - 4); ++i)
        {
            actualName.push_back(fileName.at(i));
        }
    }
    else{actualName = "error";}
    return actualName;
}

// remove whitespaces and comments from given string
std::string cleanLine(std::string line)
{
    int size = line.size();
    std::string cleaned = "";
    for (int i=0; i<size;++i)
    {
        if (line.at(i) != ' ')
        {
            if ((line.at(i) == '/') && (line.at(i + 1) == '/')){break;}
            cleaned.push_back(line.at(i));
        }
    }
    return cleaned;
}

// remove parenthesis from string in form: (myString)
std::string removeParenthesis(std::string line)
{
    int size = line.size();
    std::string removed = "";
    for (int i=0; i<size;++i)
    {
        if ( !((line.at(i) == '(') && (i == 0)) && !((line.at(i) == ')') && (i == (size - 1))))
        {
            removed.push_back(line.at(i));
        }
    }
    return removed;
}

// returns true if every character in the string is a number, returns false otherwise
bool isNumber(std::string line)
{
    bool isNum = true;
    for (int i = 0; i < line.size();++i)
    {
        if (std::isdigit(line.at(i)) == false)
        {
            isNum = false;
        }
    }
    return isNum;
}

// returns true if characters in a string are either numbers of letters, false otherwise
bool validSymbolChars(std::string line)
{
    for (int i = 0; i < line.size(); ++i)
    {
        if (!(((line.at(i) >= '0') && (line.at(i) <= '9')) || ((line.at(i) >= 'A') && (line.at(i) <= 'Z')) || ((line.at(i) >= 'a') && (line.at(i) <= 'z'))))
        {
            return false;
        }
    }
    return true;
}

// determines if given line is an A-Instruction
bool isAInstruction(std::string line)
{
    // remove @ from string
    if (line.at(0) != '@'){return false;}
    std::string dropAt;
    for (int i = 1; i < line.size(); ++i){
        dropAt.push_back((line).at(i));
    }
    // if a instruct is a number
    if (isNumber(dropAt)){return true;}
    // if a insutrct is not a number, variable cannot start with a digit
    if ((std::isdigit(dropAt.at(0)))){ return false;}
    // if variable has valid chars, then a instruct 
    if (validSymbolChars(dropAt)) {return true;}     

    return true;
}

// determines if given line is an C-Instruction
bool isCInstruction(std::string line)
{
    // determine how many times = and ; appear in the line
    int eqNum = 0, semiNum = 0;
    for (int i = 0; i < line.size(); ++i)
    {
        if (line.at(i) == '='){++eqNum;}
        if (line.at(i) == ';'){++semiNum;}
    }

    // if line does not have at least a dest or jump, then not c-instruct
    if (!(eqNum == 1 || semiNum == 1)){
        return false;
    }

    // if only dest and comp
    if ((eqNum == 1) && (semiNum == 0))
    {
        int iterator = line.find('=');
        std::string dest, comp;
        for (int i = 0; i < iterator;++i)
        {
            dest.push_back(line.at(i));
        }
        // if dest does not exist in HACK, then not valid C-Instruction
        if (!(destTable.count(dest) == 1)){return false;}
        for (int i = (iterator + 1); i < line.size();++i)
        {
            comp.push_back(line.at(i));
        }
        // if comp does not exist in HACK, then not  valid C-Instruction
        if (!(compTable.count(comp) == 1)){return false;}
    }

    // if only comp and jump
    if ((eqNum == 0) && (semiNum == 1))
    {
        int iterator = line.find(';');
        std::string comp, jump;
        for (int i = 0; i < iterator;++i)
        {
            comp.push_back(line.at(i));
        }
        if (!(compTable.count(comp) == 1)){return false;}
        for (int i = (iterator + 1); i < line.size();++i)
        {
            jump.push_back(line.at(i));
        }
        if (!(jumpTable.count(jump) == 1)){return false;}
    }

    // if dest,comp, and jump
    if ((eqNum == 1) && (semiNum == 1))
    {
        int eqIterator = line.find('=');
        int semiIterator = line.find(';');
        std::string dest, comp, jump;
        for (int i = 0; i < eqIterator;++i)
        {
            dest.push_back(line.at(i));
        }
        if (!(destTable.count(dest) == 1)){return false;}
        for (int i = (eqIterator + 1); i < semiIterator;++i)
        {
            comp.push_back(line.at(i));
        }
        if (!(compTable.count(comp) == 1)){return false;}
        for (int i = (semiIterator + 1); i < line.size();++i)
        {
            jump.push_back(line.at(i));
        }
        if (!(jumpTable.count(jump) == 1)){return false;}
    }

    return true;
     
}

// encodes hack assembly language into an A-instruction; assumes the given line is A-Instruction
std::string aInstruction(std::string line)
{
    std::string dropAt;
    for (int i = 1; i < line.size(); ++i){
        dropAt.push_back((line).at(i));
    }
    if (isNumber(dropAt))
    {
        std::string bin = decToBinary(stoi(dropAt));
        return ("0" + bin); 
    }
    std::string bin = decToBinary(symTable[dropAt]);
    return ("0" + bin); 
}

// encodes hack assembly language into an A-instruction; assumes the given line is A-Instruction
std::string cInstruction(std::string line)
{
    int eqNum = 0, semiNum = 0;
    for (int i = 0; i < line.size(); ++i)
    {
        if (line.at(i) == '='){++eqNum;}
        if (line.at(i) == ';'){++semiNum;}
    }

    std::string prefix = "111", dest = "", jump = "", comp = ""; 

    // if just dest and comp, look up in corresponding tables 
    if ((eqNum == 1) && (semiNum == 0))
    {
        int iterator = line.find('=');
        for (int i = 0; i < iterator;++i)
        {
            dest.push_back(line.at(i));
        }
        dest = destTable[dest];
        for (int i = (iterator + 1); i < line.size();++i)
        {
            comp.push_back(line.at(i));
        }
        comp = compTable[comp];
        jump = "000";
    }

    // if just comp and jump
    if ((eqNum == 0) && (semiNum == 1))
    {
        int iterator = line.find(';');
        for (int i = 0; i < iterator;++i)
        {
            comp.push_back(line.at(i));
        }
        comp = compTable[comp];
        for (int i = (iterator + 1); i < line.size();++i)
        {
            jump.push_back(line.at(i));
        }
        jump = jumpTable[jump];
        dest = "000";
    }

    // if dest, comp, and jump
    if ((eqNum == 1) && (semiNum == 1))
    {
        int eqIterator = line.find('=');
        int semiIterator = line.find(';');
        for (int i = 0; i < eqIterator;++i)
        {
            dest.push_back(line.at(i));
        }
        dest = destTable[dest];
        for (int i = (eqIterator + 1); i < semiIterator;++i)
        {
            comp.push_back(line.at(i));
        }
        comp = compTable[comp];
        for (int i = (semiIterator + 1); i < line.size();++i)
        {
            jump.push_back(line.at(i));
        }
        jump = jumpTable[jump];
    }

    // compine components into instruction syntax
    return (prefix + comp + dest + jump);
     
}


int main(){
    
    // populate symbol table with predefined symbols
    symTable["SP"] = 0; 
    symTable["LCL"] = 1;
    symTable["ARG"] = 2;
    symTable["THIS"] = 3; 
    symTable["THAT"] = 4;
    for (int r = 0; r < 15; ++r){
        symTable["R" + r] = r; 
    }
    symTable["SCREEN"] = 16384; 
    symTable["KBD"] = 24576; 

    // populate a hash table with all possible comp values
    compTable["0"] =   "0101010"; 
    compTable["1"] =   "0111111";
    compTable["-1"] =  "0111010";
    compTable["D"] =   "0001100";
    compTable["A"] =   "0110000";
    compTable["M"] =   "1110000";
    compTable["!D"] =  "0001101";
    compTable["!A"] =  "0110001";
    compTable["-D"] =  "0001111";
    compTable["-A"] =  "0110011";
    compTable["-M"] =  "1110011";
    compTable["D+1"] = "0011111";
    compTable["A+1"] = "0110111";
    compTable["M+1"] = "1110111";
    compTable["D-1"] = "0001110";
    compTable["A-1"] = "0110010";
    compTable["M-1"] = "1110010";
    compTable["D+A"] = "0000010";    
    compTable["D+M"] = "1000010";
    compTable["D-A"] = "0010011";
    compTable["D-M"] = "1010011";
    compTable["A-D"] = "0000111";
    compTable["M-D"] = "1000111";
    compTable["D&A"] = "0000000";
    compTable["D&M"] = "1000000";
    compTable["D|A"] = "0010101";
    compTable["D|M"] = "1010101";

    // populate a hash table with all possible dest values
    destTable["M"] =   "001";
    destTable["D"] =   "010";
    destTable["MD"] =  "011";
    destTable["A"] =   "100";
    destTable["AM"] =  "101";
    destTable["AD"] =  "110";
    
    // populate a hash table with all possible jump values
    jumpTable["JGT"] = "001";
    jumpTable["JEQ"] = "010";
    jumpTable["JGE"] = "011";
    jumpTable["JLT"] = "100";
    jumpTable["JNE"] = "101";
    jumpTable["JLE"] = "110";    
    jumpTable["JMP"] = "111";

    // ask user for assmebly file and then opeb file
    std::cout << "Name of assembly file that will be assembled into Hack (include .asm file extension): ";
    std::string fileName;
    std::cin >> fileName;

    // if file is not .asm, then error
    if (removeExt(fileName) == "error"){
        std::cout << "The given file is not a .asm file!";
        return 0;
    }

    // open file
    std::ifstream asmfile;
    asmfile.open(fileName); 
    if (!asmfile){ std::cout << "Unable to open file!";return 0;}

    int pc = 0;
    std::string currline;

    // first pass, used to add labels to symbol table
    while (!asmfile.eof())
    {
        std::getline(asmfile, currline);
        std::string clean = cleanLine(currline);
        
        // if line has parenthesis, then add to symbol table if not already there
        if ((clean.front() == '(') && (clean.back() == ')'))
        {
            std::string label = removeParenthesis(clean);
            if (symTable.count(label) == 0)
            {
                symTable[label] = pc;
            }
        }
        
        else{pc = ++pc;}
    }

    // close file
    asmfile.close();

    // reopen file for another pass
    asmfile.open(fileName);

    // next possible empty address in which new variables can be placed
    int nextAddress = 16;

    // second pass, used to add variables from A-instructions to symbol tables 
    while (!asmfile.eof())
    {
        std::getline(asmfile, currline);
        std::string clean = cleanLine(currline);
        
        // if A-Insutrction, then add variable to symbol table if not already there
        if (clean.front() == '@')
        {
            std::string aInstruct = "";
            for (int i =1; i < clean.size(); ++i)
            {
                aInstruct.push_back(clean.at(i));
            }

            if (!isNumber(aInstruct)){
                if (validSymbolChars(aInstruct)){
                    if (symTable.find(aInstruct) == 0)
                    {
                        symTable[aInstruct] = nextAddress;
                        nextAddress = ++nextAddress; 
                    }
                }
            }
        }
    }

    // open a hack file with the same name as the given assembly file
    asmfile.close();
    std::string actualFileName = removeExt(fileName);
    std::ofstream hackFile;
    hackFile.open(actualFileName + ".hack");
    if (!hackFile){std::cout << "Hack file was not created!" << std::endl;return 0;}

    // reopen assembly file
    asmfile.open(fileName);
    
    // parse through assembly file
    while (!asmfile.eof())
    {
        // get line and remove whitepsace and/or comments
        std::getline(asmfile, currline);
        std::string clean = cleanLine(currline);

        // if empyty line then skip
        if (clean == "")
        {  
            continue;
        }

        // if A-Instriction, then evaluate and output accordingly 
        if (isAInstruction(clean)){
            std::string bin = aInstruction(clean);
            hackFile << (bin + "\n");
            continue;
        }
        // if C-Instriction, then evaluate and output accordingly 
        if (isCInstruction(clean))
        {
            std::string bin = cInstruction(clean);
            hackFile << (bin + "\n");
            continue;
        }
    }

    // close files
    hackFile.close();
    asmfile.close();

    return 0;

}
