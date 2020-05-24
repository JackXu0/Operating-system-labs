//
//  Linker.hpp
//  Linker
//
//  Created by Zhuocheng Xu on 9/22/19.
//  Copyright © 2019 Zhuocheng Xu. All rights reserved.
//

#include <string>
#include <iostream>
#include <vector>



using namespace std;

class Token {
private:
    
public:
    string value;
    int line;
    int offset;
    
    Token(string s, int l, int o);
    Token();
};

class Definition {
private:
    
public:
    string symbol;
    int address;
    bool multiVal;
    int moduleNum;
    bool used;
    Definition(string s, int m);
    Definition(string s, int n, int m);
};

class Use {
private:
    
public:
    string symbol;
    bool used;
    int moduleNum;
    Use(string s, int m );
};

class Instruction {
    
public:
    string type;
    int opcode;
    int operand;
    int instr;
    Instruction(string s);
    Instruction(string s, int instr);
    void setInstr(int i);
};

class Module{
private:
public:
    Module();
    int id;
    int base;
    vector<Definition> definitions;
    vector<Use> uses;
    vector<Instruction> instructions;
    
};

