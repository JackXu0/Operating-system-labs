//
//  instruction.cpp
//  Linker
//
//  Created by Zhuocheng Xu on 9/22/19.
//  Copyright © 2019 Zhuocheng Xu. All rights reserved.
//

#include "Linker.h"
#include <string>
using namespace std;

Instruction::Instruction(string s, int i) {
    type = s;
    instr = i;
    operand = i % 1000;
    opcode = i / 1000;
}

Instruction::Instruction(string s) {
    type = s;
}

void Instruction::setInstr(int i){
    instr = i;
    operand = i % 1000;
    opcode = i / 1000;
}


