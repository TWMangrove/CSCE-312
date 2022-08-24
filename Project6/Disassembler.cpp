// Name: Freddy Velasco
// UIN: 327004703
// Project Description: This is a program which will take a .hack file written in HACK machine 
//                      language and "disassemble" into an equivalent .asm file written in HACK 
//                      assembly language

#include <iostream>
#include <math.h>
#include <unordered_map>
#include <string>
#include <fstream>
#include <ostream>
#include <vector>

std::unordered_map<std::string, std::string> compTable;
std::unordered_map<std::string, std::string> destTable;
std::unordered_map<std::string, std::string> jumpTable;

// converts binary into decimal
int binarytoDec(long long x)
{
    int dec = 0, temp;
    for (int i = 0; x != 0; ++i)
    {
        temp = x % 10;
        x /= 10;
        dec += temp * pow(2,i);
    }
    return dec;
}

// removes .hack extension from a string. returns error if no .asm extension 
std::string removeExt(std::string fileName)
{
    std::string actualName;
    if (fileName.find(".hack") == (fileName.size() - 5))
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

// returns true if the given string is in binary, false otherwise
bool isBinary(std::string line)
{
    bool bin= true;
    for (int i = 0; i < line.size(); ++i)
    {
        if (!((line.at(i) == '0') || (line.at(i) == '1'))){
            bin = false;
        }
    }
    return bin;
}

// returns true if given line is a valid A-Instruction
bool isAInstruction(std::string line)
{
    // A-Instruction must be in binary
    if (isBinary(line))
    {
        // A-Instruction must have size of 16
        if (line.size() == 16)
        {
            // A-Instruction must start with a 0
            if (line.at(0) == '0'){
                return true;
            }
        }
    }
    return false;
}

// evaluates given binary line into an assembly A-Intruction
std::string aInstruction(std::string line)
{
    std::string instruct, num = "";
    
    // remove first char
    for (int i=1; i < line.size();++i)
    {
        num.push_back(line.at(i));
    }
    
    // conver the rest of binary into decimal
    int dec = binarytoDec(std::stoi(num));

    // add @ to get A-Instruction
    return ("@" + std::to_string(dec));
}

// returns true if given line is a valid C-Instruction
bool isCInstruction(std::string line)
{
    // if not in binary or not 16 bits, then return false 
    if (!isBinary(line)){return false;}
    if (line.size() != 16){return false;}

    // if prefix is not 111, return false
    for (int i=0; i < 3; ++i)
    {
        if (line.at(i) != '1'){return false;}
    }
    
    // get comp, dest, and jump based on expected syntax
    std::string comp = "", dest = "", jump = ""; 
    for (int i=3; i < 10; ++i)
    {
        comp.push_back(line.at(i));
    }
    for (int i=10; i < 13; ++i)
    {
        dest.push_back(line.at(i));
    }
    for (int i =13; i< 16; ++i)
    {
        jump.push_back(line.at(i));
    }

    // if dest is empty but comp and jump are valid, then true
    if ((dest == "000") && (compTable.count(comp) == 1) && (jumpTable.count(jump) == 1))
    {
        return true;
    }
    // if jump is empty but comp and dest are valid, then true
    if ((jump == "000") && (compTable.count(comp) == 1) && (destTable.count(dest) == 1))
    {
        return true;
    }
    
    // if all components present, then check all if valid
    if (!(compTable.count(comp) == 1)){return false;}
    if (!(destTable.count(dest) == 1)){return false;}
    if (!(jumpTable.count(jump) == 1)){return false;}

    return true;    
}

// evaluates given binary line into an assembly C-Intruction
std::string cInstruction(std::string line)
{
    // get comp, dest, and jump based on expected syntax
    std::string comp = "", dest = "", jump = ""; 
    for (int i=3; i < 10; ++i)
    {
        comp.push_back(line.at(i));
    }
    for (int i=10; i < 13; ++i)
    {
        dest.push_back(line.at(i));
    }
    for (int i =13; i< 16; ++i)
    {
        jump.push_back(line.at(i));
    }

    // if jump is empty, then output in form: dest = comp
    if (jump == "000")
    {
        return (destTable[dest] + " = " + compTable[comp]);
    }
    // if dest is empty, then output in form: comp ; jump
    if (dest == "000")
    {
        return (compTable[comp] + " ; " + jumpTable[jump]);
    }
    // if all present, then output in form: dest = comp ; jump
    else{
        return (destTable[dest] + " = " +  compTable[comp] + " ; " + jumpTable[jump]);
    }
}

int main(){

    // populate a hash table with all possible comp values
    compTable["0101010"] = "0"; 
    compTable["0111111"] = "1";
    compTable["0111010"] = "-1";
    compTable["0001100"] = "D";
    compTable["0110000"] = "A";
    compTable["1110000"] = "M";
    compTable["0001101"] = "!D";
    compTable["0110001"] = "!A";
    compTable["0001111"] = "-D";
    compTable["0110011"] = "A";
    compTable["1110011"] = "-M";
    compTable["0011111"] = "D+1";
    compTable["0110111"] = "A+1";
    compTable["1110111"] = "M+1";
    compTable["0001110"] = "D-1";
    compTable["0110010"] = "A-1";
    compTable["1110010"] = "M-1";
    compTable["0000010"] = "D+A";    
    compTable["1000010"] = "D+M";
    compTable["0010011"] = "D-A";
    compTable["1010011"] = "D-M";
    compTable["0000111"] = "A-D";
    compTable["1000111"] = "M-D";
    compTable["0000000"] = "D&A";
    compTable["1000000"] = "D&M";
    compTable["0010101"] = "D|A";
    compTable["1010101"] = "D|M";

    // populate a hash table with all possible dest values
    destTable["001"] = "M";
    destTable["010"] = "D";
    destTable["011"] = "MD";
    destTable["100"] = "A";
    destTable["101"] = "AM";
    destTable["110"] = "AD";
    
    // populate a hash table with all possible jump values
    jumpTable["001"] = "JGT";
    jumpTable["010"] = "JEQ";
    jumpTable["011"] = "JGE";
    jumpTable["100"] = "JLT";
    jumpTable["101"] = "JNE";
    jumpTable["110"] = "JLE";    
    jumpTable["111"] = "JMP";
    
    // ask user for hack file and then open file
    std::cout << "Name of hack file that will be assembled into Hack (include .hack file extension): ";
    std::string fileName;
    std::cin >> fileName;

    // if file does not have .hack extension, then error
    std::string actualName = removeExt(fileName);
    if (actualName == "error"){
        std::cout << "The given file is not a .asm file!";
        return 0;
    }

    // open .hack file
    std::ifstream hackfile;
    hackfile.open(fileName); 
    if (!hackfile){ std::cout << "Unable to open file!";return 0;}

    // open/create .asm file
    std::ofstream asmFile;
    asmFile.open(actualName + "asm");
    if (!asmFile){std::cout << "Assembly file was not created!" << std::endl;return 0;}

    std::string currline;

    // parse through assembly file
    while (!hackfile.eof())
    {
        // get line and remove whitepsace and/or comments
        std::getline(hackfile, currline);
        std::string clean = cleanLine(currline);

        // if empyty line then skip
        if (clean == ""){continue;}

        // if A-Instriction, then evaluate and output accordingly 
        if (isAInstruction(clean)){
            std::string assembly = aInstruction(clean);
            asmFile << (assembly + "\n");
            continue;
        }
        // if C-Instruction, then evaluate and output accordingly 
        if (isCInstruction(clean))
        {
            std::string assembly = cInstruction(clean);
            asmFile << (assembly + "\n");
            continue;
        }
    }
    
    asmFile.close();
    hackfile.close();
    return 0;
}