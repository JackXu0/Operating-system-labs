//
//  Process.cpp
//  VirtualMemory
//
//  Created by Zhuocheng Xu on 11/21/19.
//  Copyright © 2019 Zhuocheng Xu. All rights reserved.
//

#include <stdio.h>
#include "main.h"

using namespace std;

Process::Process(int id){
    process_id = id;
    for(int i=0; i<PAGETABLE_SIZE; i++){
        pageTable.push_back({i,0, id, false,false,false,false,false});
    }
}

//frame_t Process::select_victim_frame(Process* process){
//    
//    frame_t t = frame_list.front();
//    return t;
//}

void Process::add_PTE(pte_t e){
    pageTable[e.vma] = e;
}
