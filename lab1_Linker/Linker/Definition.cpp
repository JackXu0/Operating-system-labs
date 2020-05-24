//
//  definition.cpp
//  Linker
//
//  Created by Zhuocheng Xu on 9/22/19.
//  Copyright © 2019 Zhuocheng Xu. All rights reserved.
//

#include "Linker.h"
#include <string>
#include <iostream>
using namespace std;

Definition::Definition(string s, int a, int m) {
    symbol = s;
    address = a;
    used = false;
    multiVal = false;
    moduleNum = m;
}

Definition::Definition(string s, int m) {
    symbol = s;
    used = false;
    multiVal = false;
    moduleNum =m;
}
