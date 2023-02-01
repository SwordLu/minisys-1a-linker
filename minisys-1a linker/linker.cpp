#include<iostream>
#include<fstream>
#include<string>
#include<vector>
#include<regex>

using namespace std;

bool Link(int inputFilesAmount, string* inputFilePathList, string outputFilePath) {
    //token
    string token_data = "(\\.data)";
    string token_text = "(\\.text)";
    string token_space0 = "([\t ]*)";
    string token_space1 = "([\t ]+)";
    string token_lable = "[a-z_][0-9a-z_\\.]*";
    string token_address = "(0x[0-9a-f]{1,4}|[01]{1,16}[b]|[0-9]{1,5})";
    string instruction[58] = {
        "add", "addu", "sub", "subu", "and", "or", "xor", "nor", "slt", "sltu", "sllv", "srlv", "srav",
        "addi", "addiu", "andi", "ori", "xori", "slti", "sltiu", "sll", "srl", "sra",
        "lb", "lbu", "lh", "lhu", "sb", "sh", "lw", "sw", "beq", "bne",
        "mult", "multu", "div", "divu", "mfc0", "mtc0", "jalr", "lui",
        "bgez", "bgtz", "blez", "bltz", "bgezal", "bltzal", "mfhi", "mflo", "mthi", "mtlo", "jr",
        "j", "jal", "break", "syscall", "eret", "nop"
    };
    string token_instruction = "(";
    for (int i = 0; i < 57; i++) {
        token_instruction += instruction[i] + "|";
    }
    token_instruction += instruction[57] + ")";


    //pattern
    regex pattern_data(token_space0 + token_data + token_space0);
    regex pattern_text(token_space0 + token_text + token_space0);
    regex pattern_note("#");
    regex pattern_colon(":");
    regex pattern_lable(token_lable);
    regex pattern_notSeparate("[^ ,()\t]+");
    regex pattern_space(token_space1);
    regex pattern_instruction(token_instruction);
    regex pattern_address(token_address);

    //open file
    ofstream outputFile;
    vector<ifstream> inputFileList(inputFilesAmount);

    outputFile.open(outputFilePath.c_str());
    for (int i = 0; i < inputFilesAmount; ++i) {
        inputFileList.at(i).open(inputFilePathList[i].c_str());
    }

    //read and write data
    outputFile << ".data" << endl;
    for (int i = 0; i < inputFilesAmount; ++i) {
        ifstream& inputFile = inputFileList.at(i);
        bool beforeData = true;
        bool beforeText = true;
        while (!inputFile.eof() && beforeText) {
            string line;
            string outputLine;
            getline(inputFile, line);
            //处理注释和全部转成小写
            smatch noteResult;
            regex_search(line, noteResult, pattern_note);
            if (noteResult.size() != 0) {
                line = noteResult.prefix().str();
            }
            if (line.length() == 0) {
                continue;
            }
            else {
                transform(line.begin(), line.end(), line.begin(), ::tolower);
            }
            //data
            if (beforeData) {
                smatch dataResult;
                if (regex_match(line, pattern_data)) {
                    beforeData = false;
                }
            }
            else {
                if (regex_match(line, pattern_text)) {
                    beforeText = false;
                }
                else {
                    smatch colonResult;
                    regex_search(line, colonResult, pattern_colon);
                    if (colonResult.size() != 0) {
                        string beforeColon = colonResult.prefix().str();
                        smatch lableResult;
                        regex_search(beforeColon, lableResult, pattern_lable);
                        outputLine = lableResult.prefix().str() + "f" + to_string(i) + "_" + lableResult.str() + lableResult.suffix().str() + ":" + colonResult.suffix().str();
                    }
                    else {
                        outputLine = line;
                    }
                    outputFile << outputLine << endl;
                }
            }
        }
    }

    //read and write text
    int instructionAmount = 0;

    outputFile << ".text" << endl;
    for (int i = 0; i < inputFilesAmount; ++i) {
        int fileInstructionAmount = 0;
        ifstream& inputFile = inputFileList.at(i);
        while (!inputFile.eof()) {
            string line;
            string outputLine = " ";
            getline(inputFile, line);

            //处理注释和全部转成小写
            smatch noteResult;
            regex_search(line, noteResult, pattern_note);
            if (noteResult.size() != 0) {
                line = noteResult.prefix().str();
            }
            if (line.length() == 0) {
                continue;
            }
            if (regex_match(line, pattern_space)) {
                continue;
            }
            transform(line.begin(), line.end(), line.begin(), ::tolower);
            //text
            smatch colonResult;
            regex_search(line, colonResult, pattern_colon);
            if (colonResult.size() != 0) {
                string beforeColon = colonResult.prefix().str();
                smatch lableResult;
                regex_search(beforeColon, lableResult, pattern_lable);
                outputLine = lableResult.prefix().str() + "f" + to_string(i) + "_" + lableResult.str() + lableResult.suffix().str() + ":";
                line = colonResult.suffix().str();
            }
            //change line
            smatch keywordResult;
            regex_search(line, keywordResult, pattern_notSeparate);
            if (keywordResult.size() != 0) {
                string keywordPrefix = keywordResult.prefix().str();
                string keywordSuffix = keywordResult.suffix().str();
                string firstKeyword = keywordResult.str();
                line = keywordPrefix + firstKeyword;
                if (regex_match(firstKeyword, pattern_instruction)) {    
                    fileInstructionAmount++;
                    while (keywordSuffix.length() != 0) {
                        smatch newKeywordResult;
                        regex_search(keywordSuffix, newKeywordResult, pattern_notSeparate);
                        if (newKeywordResult.size() == 0) {
                            line += keywordSuffix;
                            break;
                        }
                        else {
                            string newKeyword = newKeywordResult.str();
                            keywordPrefix = newKeywordResult.prefix().str();
                            keywordSuffix = newKeywordResult.suffix().str();
                            line += keywordPrefix;
                            if (regex_match(newKeyword, pattern_lable)) {
                                line += "f" + to_string(i) + "_" + newKeyword;
                            }
                            else if ((firstKeyword == "j" || firstKeyword == "jal") && regex_match(newKeyword, pattern_address)) {
                                int address = atoi(newKeyword.c_str());
                                address += 4 * instructionAmount;
                                line += to_string(address);
                            }
                            else {
                                line += newKeyword;
                            }
                        }
                    }
                }
            }
            //
            outputLine = outputLine + line;
            outputFile << outputLine << endl;
        }
        instructionAmount += fileInstructionAmount;
    }

    //close file
    outputFile.close();
    for (int i = 0; i < inputFilesAmount; ++i) {
        inputFileList.at(i).close();
    }

    return true;
}

int main() {
    string inputFilePathList[] = { "cpu.s", "leds.s" };
    int inputFilesAmount = 2;
    string outputFilePath = "afterLink.s";
    if (Link(inputFilesAmount, inputFilePathList, outputFilePath)) {
        cout << "Done Succesfully.\n";
    }
    return 1;
}