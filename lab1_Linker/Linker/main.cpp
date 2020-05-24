//
//  main.cpp
//  Linker
//
//  Created by Zhuocheng Xu on 9/21/19.
//  Copyright © 2019 Zhuocheng Xu. All rights reserved.
//
#include <fstream>
#include <vector>
#include "Linker.h"
#include <iostream>
using namespace std;

vector<Definition> definitions;

//judge is a token is number
bool isDigit(const string s){
    string::const_iterator it = s.begin();
    while (it != s.end() && isdigit(*it)) ++it;
    return !s.empty() && it == s.end();
}

//convert a string to an integer if it represents a numer
int readInt(const string s){
    if(isDigit(s)){
        return stoi(s);
    }else{
        return -1;
    }
}

//get the index of a symbol in the symbol table
int getDefIndex(string s){
    for(int i=0; i<definitions.size(); i++){
        if(definitions[i].symbol == s){
            return i;
        }
    }
    return -1;
}

//possess every index indicator three digits
string appendZero(int i){
    if(i<10){
        return "00"+to_string(i);
    }else if(i<100){
        return "0"+to_string(i);
    }else{
        return to_string(i);
    }
}

//check if a symbol already exists
bool containSymbol(string s ){
    for(int i=0; i<definitions.size(); i++){
        if(definitions[i].symbol == s){
            definitions[i].multiVal = true;
            return true;
        }
    }
    
    return false;
}


//(1) verifies the correct syntax
//(2) determines the base address for each module
//(3) determines the absolute address for each defined symbol and store in vector<Definition> definitions

void pass1(string filename){
    
    vector<Module> modules;
    vector<Instruction> instructions;
    ifstream file(filename);
    if (file.is_open()) {
        string line;
        int lineNum = 1;
        int lineLength = 0;
        int position = 0;      //used to track tokens in each part
        int partLength = 0;    //used to indicate how many pairs of tokens will appear in each part
        int partNum = 1;       //used to track current part, each module has three parts
        int moduleId = 1;
        int instNum = 0;       //count the total number of instructions
        int symInModule = 0;
        while (getline(file, line)) {
            string s = line.c_str();
            lineLength = (int) s.length();
            int preLength = 1; //used to track current position in line
            int begToken = 0;
            int endToken = 0;
            
            
            //read, judge and operate on eah token
            while (endToken < s.length()){
                while(begToken<s.length() && (s.at(begToken)==' ' || s.at(begToken)=='\t')){
                    begToken++;
                }
                preLength += begToken - endToken;
                endToken = begToken;
                
                while(endToken < s.length() && s.at(endToken)!=' ' && s.at(endToken)!='\t'){
                    endToken++;
                }
                string token = s.substr(begToken, endToken-begToken);
                cout << "line "<<lineNum<<" offset "<<preLength << '\n';
                begToken = endToken;
//                while(begToken<s.length() && (s.at(begToken)==' ' || s.at(begToken)=='\t')){
//                    begToken++;
//                }
//                preLength += begToken - endToken;
//                endToken = begToken;

//                cout<< "Token: " << partNum << ":" << preLength << " : "<<token <<'\n';
         
                
                //read definition list
                if(partNum%3 == 1){
                    //read number of definitions
                    if(position == 0){
                        Module m;
                        m.base = instNum;
                        m.id = moduleId;
                        int numOfDef = readInt(token);
                        
                        //if not a number
                        if(numOfDef < 0){
                            cout << "Parse Error line "<<lineNum<<" offset "<<preLength<<": NUM_EXPECTED"<< '\n';
                            exit(-1);
                        }else if(numOfDef == 0){
                            //if no definition is to be made, jump to use part
                            position = 0;
                            partLength = 0;
                            partNum++;
                        }else{
                            //prepare for reading definitions
                            partLength = numOfDef;
                            position++;
                        }
                        
                        symInModule = numOfDef;
                        
                        //too many definitions in module
                        if(numOfDef > 16){
                            cout << "Parse Error line "<<lineNum<<" offset "<<preLength<<": TOO_MANY_DEF_IN_MODULE"<< '\n';
                            exit(-1);
                        }
                        modules.push_back(m);
                        moduleId++;
                    }else{
                        //read the symbol name of a definition
                        if(position%2 == 1){
                            if(isDigit(token) || isdigit(token[0])){
                                cout << "Parse Error line "<<lineNum<<" offset "<<preLength<<": SYM_EXPECTED"<< '\n';
                                exit(-1);
                            }else if(token.length()>16){
                                cout << "Parse Error line "<<lineNum<<" offset "<<preLength<<": SYM_TOO_LONG"<< '\n';
                                exit(-1);
                            }else if(containSymbol(token)){ // if symbol already defined
                                position++;
                            }else{
                                //add symbol to the symbol table
                                definitions.push_back(Definition(token, moduleId));
                                position++;
                            }
                        }else{
                            //read the symbol address of a definition
                            if(!isDigit(token)){
                                cout << "Parse Error line "<<lineNum<<" offset "<<preLength<<": NUM_EXPECTED"<< '\n';
                                exit(-1);
                            }else{
                                //if the symbol is already defined, skip
                                if(!definitions[definitions.size()-1].multiVal){
                                    definitions[definitions.size()-1].address = readInt(token)+modules[moduleId-2].base;
                                }
                                
                                position++;
                                if(position == 1+partLength*2){
                                    //if reach the end of instruction part
                                    position = 0;
                                    partLength = 0;
                                    partNum++;
                                    
                                }
                            }
                        }
                    }
                }else if
                
                //read use list
                (partNum%3 == 2){
                    
                    //read number of uses
                    if(position == 0){
                        int numOfUse = readInt(token);
                        if(numOfUse < 0){
                            cout << "Parse Error line "<<lineNum<<" offset "<<preLength<<": NUM_EXPECTED"<< '\n';
                            exit(-1);
                        }else if(numOfUse == 0){
                            position = 0;
                            partLength = 0;
                            partNum++;
                        }else{
                            partLength = numOfUse;
                            position++;
                        }
                        if(numOfUse > 16){
                            cout << "Parse Error line "<<lineNum<<" offset "<<preLength<<": TOO_MANY_USE_IN_MODULE"<< '\n';
                            exit(-1);
                        }
                        
                        
                    }else{
                        //read the symbol name of uses
                        if(isDigit(token) || isdigit(token[0])){
                            cout << "Parse Error line "<<lineNum<<" offset "<<preLength<<": SYM_EXPECTED"<< '\n';
                            exit(-1);
                        }else if(token.length()>16){
                            cout << "Parse Error line "<<lineNum<<" offset "<<preLength<<": SYM_TOO_LONG"<< '\n';
                            exit(-1);
                        }
                        
                        position++;
                        
                        if(position == 1+partLength){
                            position = 0;
                            partLength = 0;
                            partNum++;
                            
                        }
                        
                    }
                }else if
                
                //read instructions list
                (partNum%3 == 0){
                    //read number of instructions
                    if(position == 0){
                        int numOfInst = readInt(token);
                        if(numOfInst < 0){
                            cout << "Parse Error line "<<lineNum<<" offset "<<preLength<<": NUM_EXPECTED"<< '\n';
                            exit(-1);
                        }else if(numOfInst == 0){
                            position = 0;
                            partLength = 0;
                            partNum++;
                        }else{
                            partLength = numOfInst;
                            position++;
                        }
                        if(instructions.size()+numOfInst > 512){
                            cout << "Parse Error line "<<lineNum<<" offset "<<preLength<<": TOO_MANY_INSTR"<< '\n';
                            exit(-1);
                        }
                        
                    }else{
                        //read the IAER of an instruction
                        if(position%2 == 1){
                            if(token != "R" && token != "E" && token != "I" && token != "A"){
                                cout << "Parse Error line "<<lineNum<<" offset "<<preLength<<": ADDR_EXPECTED"<< '\n';
                                exit(-1);
                            }else{
                                
                                instructions.push_back(Instruction(token));
                                position++;
                            }
                        }else{
                            //read the symbol address of a definition
                            if(!isDigit(token)){
                                cout << "Parse Error line "<<lineNum<<" offset "<<preLength<<": NUM_EXPECTED"<< '\n';
                                exit(-1);
                            }else{
                                
                                position++;
                                if(position == 1+partLength*2){
                                    
                                    //check if an address appearing in a definition exceeds the size of the module
                                    for(int i=(int)definitions.size()-symInModule; i< definitions.size(); i++){
                                        if(definitions[i].address - modules[moduleId-2].base >= partLength){
                                            cout << "Warning: Module " << modules.size() << ": " << definitions[i].symbol << " too big "<< definitions[i].address - modules[moduleId-2].base <<" (max="<<partLength-1<<") assume zero relative\n" ;
                                            definitions[i].address = modules[moduleId-2].base;
                                        }
                                        
                                    }
                                    partNum++;
                                    instNum += partLength;
                                    position = 0;
                                    partLength = 0;
                                    
                                }
                            }
                        }
                    }
                }
                
                preLength += token.size();
                
            }
            lineNum++;
        }
        
        //if reading stops abnormally
        if(position != 0){
            if(position%2 == 0){
                //stop where should be given the symbol name of use
                if(partNum %3 ==2){
                    cout << "Parse Error line "<<lineNum-1<<" offset "<<lineLength+1<<": SYM_EXPECTED"<< '\n';
                    exit(-1);
                }else{
                // stop where should be given the address of symbol or opcode of instruction
                    cout << "Parse Error line "<<lineNum-1<<" offset "<<lineLength+1<<": NUM_EXPECTED"<< '\n';
                    exit(-1);
                }
                
            }else{
                //stop where should be given the symbol name of definition or use
                if(partNum%3 != 0){
                    cout << "Parse Error line "<<lineNum-1<<" offset "<<lineLength+1<<": SYM_EXPECTED"<< '\n';
                    exit(-1);
                }else{
                //stops where should be given the instruction type
                    cout << "Parse Error line "<<lineNum-1<<" offset "<<lineLength+1<<": ADDR_EXPECTED"<< '\n';
                    exit(-1);
                }
            }
        }
        
        file.close();
    }
    
    cout << "Symbol Table" << '\n';
    for(int i=0; i<definitions.size(); i++){
        cout << definitions[i].symbol << '=' << definitions[i].address;
        if(definitions[i].multiVal == true){
            cout << " Error: This variable is multiple times defined; first value used";
        }
        
        cout << '\n';
    }
    
    cout << "\nMemory Map" << '\n';
    
}



//read again, generate the actual output by relocating relative addresses and resolving external references

void pass2(string filename){
    vector<Module> modules;
    vector<Use> uses;
    vector<Instruction> instructions;
    ifstream file(filename);
    if (file.is_open()) {
        string line;
        int lineNum = 1;
        int position = 0;
        int partLength = 0;
        int partNum = 1;
        int moduleId = 1;
        int instNum = 0;
        int preUse = 0;
        while (getline(file, line)) {
            string s = line.c_str();
            int preLength = 1;
            int begToken = 0;
            int endToken = 0;
            
            while(begToken<s.length() && (s.at(begToken)==' ' || s.at(begToken)=='\t')){
                begToken++;
            }
            preLength += begToken - endToken;
            endToken = begToken;
            while (endToken < s.length()){
                while(endToken < s.length() && s.at(endToken)!=' ' && s.at(endToken)!='\t'){
                    endToken++;
                }
                string token = s.substr(begToken, endToken-begToken);
                
                begToken = endToken;
                while(begToken<s.length() && (s.at(begToken)==' ' || s.at(begToken)=='\t')){
                    begToken++;
                }
                preLength += begToken - endToken;
                endToken = begToken;

//                cout<< "Token: " << lineNum << ":" << preLength << " : "<<token << '\n';

                //just create the module
                if(partNum%3 == 1){
                    
                    if(position == 0){
                        Module m;
                        m.base = instNum;
                        m.id = moduleId;
                        int numOfDef = readInt(token);
                        
                        if(numOfDef == 0){
                            position = 0;
                            partLength = 0;
                            partNum++;
                        }else{
                            partLength = numOfDef;
                            position++;
                        }
                        modules.push_back(m);
                        moduleId++;
                    }else{
                        position++;
                        if(position == 1+partLength*2){
                            position = 0;
                            partLength = 0;
                            partNum++;
                            
                        }
                    }
                    
                    
                }else if(partNum%3 == 2){
                    //read number of uses
                    if(position == 0){
                        int numOfUse = readInt(token);
                        //unexcepted tokens
                        if(numOfUse == 0){
                            position = 0;
                            partLength = 0;
                            partNum++;
                        }else{
                            partLength = numOfUse;
                            position++;
                        }
                        
                        
                    }else{
                        
                        uses.push_back(Use(token, moduleId));
                        
                        position++;
                        
                        if(position == 1+partLength){
                            position = 0;
                            partLength = 0;
                            partNum++;
                            
                        }
                        
                    }
                }else if(partNum%3 == 0){
                    //read number of instructions
                    if(position == 0){
                        instructions.clear();
                        int numOfInst = readInt(token);
                        if(numOfInst == 0){
                            position = 0;
                            partLength = 0;
                            partNum++;
                        }else{
                            partLength = numOfInst;
                            position++;
                        }
                        
                        
                    }else{
                        //read the symbol name of an instruction
                        if(position%2 == 1){
                            instructions.push_back(Instruction(token));
                            position++;
                        }else{
                            //read the symbol address of a definition
                            
                                instructions[instructions.size()-1].instr = readInt(token);
                                
                                position++;
                                if(position == 1+partLength*2){
                                    //if the last instruction has been read
                                    for(int i=0; i<instructions.size(); i++){
                                        string type = instructions[i].type; //AIER
                                        int addr = instructions[i].instr;
                                        string id = appendZero(i+instNum);  //index of instruction
                                        bool addressTooLarge = false;
                                        bool relativeTooLarge = false;
                                        bool absoluteTooLarge = false;
                                        
                                        if(type == "I"){
                                            if(addr>=10000){
                                                addr = 9999;
                                                addressTooLarge = true;
                                            }
                                            string addrString = addr >= 10? to_string(addr) : "000"+to_string(addr);
                                            cout<< id << ": " << addrString;
                                            if(addressTooLarge){
                                                cout << " Error: Illegal immediate value; treated as 9999\n";
                                                continue;
                                            }
                                        }
                                        
                                        if(addr>=10000){
                                            addr = 9999;
                                            addressTooLarge = true;
                                            cout<< id << ": " << addr;
                                            cout << " Error: Illegal opcode; treated as 9999\n";
                                            continue;
                                        }
                                        
                                        if( type== "E"){
                                            
                                            int operand = addr%1000;
                                            bool notDefined = false;
                                            
                                            if(operand+preUse >= uses.size()){
//                                                cout<< "current index = " << addr%1000+preUse << '\n';
                                                string addrString = addr >= 10? to_string(addr) : "000"+to_string(addr);
                                                cout<< id << ": " << addrString << " Error: External address exceeds length of uselist; treated as immediate\n";
                                                continue;
                                            }
                                            //mark as used
                                            string var = uses[operand+preUse].symbol;
                                            uses[operand+preUse].used = true;
                                            //get the index of the symbol in the symbol table
                                            int index = getDefIndex(var);
                                            int offset;
                                            if (index == -1){
                                                notDefined = true;
                                                offset = 0;
                                            }else{
                                                offset = definitions[index].address;
                                                definitions[index].used = true;
                                            }
                                            
//                                            cout<< "use " << var << " ; index = " << operand+preUse << '\n';
                                            string addrString = addr/1000*1000+offset >= 10? to_string(addr/1000*1000+offset) : "000"+to_string(addr/1000*1000+offset);
                                            cout<< id << ": " << addrString;
                                            
                                            if(notDefined){
                                                cout<< " Error: "<< var <<" is not defined; zero used\n";
                                                continue;
                                            }
                                            
                                        }
                                        
                                        
                                        
                                        if(type == "R"){
                                            
                                            if(addr%1000>= partLength){
                                                addr = addr/1000*1000;
                                                relativeTooLarge = true;
                                            }
                                            
                                            string addrString = addr+instNum >= 10? to_string(addr+instNum) : "000"+to_string(addr+instNum);
                                            cout<< id << ": " << addrString;
                                            if(relativeTooLarge){
                                                cout << " Error: Relative address exceeds module size; zero used\n";
                                                continue;
                                            }
                                        }
                                        
                                        if(type == "A"){
                                            
                                            if(addr%1000 >=512){
                                                absoluteTooLarge = true;
                                                addr = addr/1000*1000;
                                            }
                                            string addrString = addr >= 10? to_string(addr) : "000"+to_string(addr);
                                            cout<< id << ": " << addrString;
                                            if(absoluteTooLarge){
                                                cout<< " Error: Absolute address exceeds machine size; zero used\n";
                                                continue;
                                            }
                                        }
                                        
                                        cout << "\n";
                                    }
                                    
                                    for(int i=preUse; i<uses.size(); i++){
                                        if(!uses[i].used){
                                            cout << "Warning: Module " << modules.size() << ": " << uses[i].symbol << " appeared in the uselist but was not actually used\n" ;
                                        }
                                    }
                                    
                                    partNum++;
                                    preUse = (int) uses.size();
                                    instNum += partLength;
                                    position = 0;
                                    partLength = 0;
                                }
                            
                        }
                    }
                    
                }
                
                preLength += token.size();
            }
            lineNum++;
        }
        
        cout << '\n';
        //if symbol defined but not used
        for(int i=0; i<definitions.size(); i++){
            if(!definitions[i].used){
                cout << "Warning: Module " << definitions[i].moduleNum-1 << ": " << definitions[i].symbol << " was defined but never used\n" ;
            }
        }
        
        
        file.close();
        }
        
    
}

int main(int argc, const char * argv[]){
//    if (argc >= 2)
//    {
//        for(int i=1; i<argc; i++){
//            pass1(argv[i]);
//            pass2(argv[i]);
//            definitions.clear();
//        }
//    }
//    else
//    {
//        cout << "Invalid Number of arguments";
//    }
    pass1("/Users/zhuocheng/downloads/lab1samples/input-3");
    pass2("/Users/zhuocheng/downloads/lab1samples/input-3");
////    definitions.clear();
    
    return 0;
    
}






