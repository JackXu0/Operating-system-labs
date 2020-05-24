//
//  Process.cpp
//  IO_Schedular
//
//  Created by Zhuocheng Xu on 12/1/19.
//  Copyright © 2019 Zhuocheng Xu. All rights reserved.
//

#include "main.h"

using namespace std;

IO_Operation::IO_Operation(int id, int at, int t){
    this->id = id;
    arriving_time = at;
    track = t;
}
