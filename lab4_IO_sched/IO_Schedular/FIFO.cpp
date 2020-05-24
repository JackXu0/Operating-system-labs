//
//  FIFO.cpp
//  IO_Schedular
//
//  Created by Zhuocheng Xu on 12/1/19.
//  Copyright © 2019 Zhuocheng Xu. All rights reserved.
//

#include "main.h"
#include <list>
#include <unordered_map>

using namespace std;

FIFO::FIFO(){
    
}

IO_Operation* FIFO::get_next_operation(int track){
    
    if(active_operations.size() == 0) return nullptr;
    IO_Operation* result = active_operations.front();
    insertExpired(result);
    active_operations.pop_front();
    
    return result;
}
