//
//  token.cpp
//  Linker
//
//  Created by Zhuocheng Xu on 9/24/19.
//  Copyright © 2019 Zhuocheng Xu. All rights reserved.
//

#include "Linker.h"
#include <string>

using namespace std;
Token::Token(string s, int l, int o) {
    value = s;
    line = l;
    offset = o;
}

Token::Token() {}
