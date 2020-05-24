//
//  LOOK.cpp
//  IO_Schedular
//
//  Created by Zhuocheng Xu on 12/5/19.
//  Copyright © 2019 Zhuocheng Xu. All rights reserved.
//

#include "main.h"
#include <list>
#include <limits>
#include <unordered_map>

using namespace std;

LOOK::LOOK(){
    moving_right = 1;
}

IO_Operation* LOOK::get_next_operation(int track){
    
    int min_margin = numeric_limits<int>::max();
    int max_value = 0;
    int min_value = numeric_limits<int>::max();
    IO_Operation* result = nullptr;
    IO_Operation* max_operation = nullptr;
    IO_Operation* min_operation = nullptr;
    for(auto it = active_operations.begin(); it != active_operations.end(); ++it){
        if((*it)->track > max_value){max_value = (*it)->track; max_operation = *it;}
        if((*it)->track < min_value){min_value = (*it)->track; min_operation = *it;}
        if(moving_right*((*it)->track - track) >= 0 && moving_right*((*it)->track - track) < min_margin){
           min_margin = moving_right*((*it)->track - track);
           result = *it;
        }
       
    }
    
    if(result == nullptr && active_operations.size() != 0){
        if(moving_right > 0){
            result = max_operation;
        }else{
            result = min_operation;
        }
        moving_right = -moving_right;
        
    }
    
    if(result != nullptr){
        insertExpired(result);
        active_operations.remove(result);
    }
    
    return result;
}
