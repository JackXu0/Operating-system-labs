//
//  Process.cpp
//  Schedular
//
//  Created by Zhuocheng Xu on 10/8/19.
//  Copyright © 2019 Zhuocheng Xu. All rights reserved.
//

#include "main.h"
#include <string>
using namespace std;

Process::Process(int id, int at, int tc, int cb, int io, int prio) {
    pid = id;
    AT = at;
    AT_origin = at;
    state_ts = at;
    TC = tc;
    TC_origin = tc;
    CB = cb;
    IO = io;
    PRIO = prio;
    current_prio = prio;
    cpu_burst = 0;
    io_time = 0;
    state_ts = 0;
    CW = 0;
}
