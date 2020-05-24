//
//  use.cpp
//  Linker
//
//  Created by Zhuocheng Xu on 9/22/19.
//  Copyright © 2019 Zhuocheng Xu. All rights reserved.
//

#include "Linker.h"
#include <string>
using namespace std;

Use::Use(string s, int m) {
    symbol = s;
    used = false;
    moduleNum = m;
}
