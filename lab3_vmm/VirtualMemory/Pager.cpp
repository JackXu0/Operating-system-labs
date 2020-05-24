//
//  Pager.cpp
//  VirtualMemory
//
//  Created by Zhuocheng Xu on 11/21/19.
//  Copyright © 2019 Zhuocheng Xu. All rights reserved.
//

#include "main.h"
#include <unordered_map>
using namespace std;
//Pager::Pager(){
//    name = "pager";
//}

Pager::Pager(){
    
}

frame_t Pager::select_victim_frame(unordered_map<frame_t*, pte_t*>* frame_map, list<frame_t*>* frame_list){
    return {true,0};
}
