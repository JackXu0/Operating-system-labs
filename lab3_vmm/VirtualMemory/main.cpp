//
//  main.cpp
//  VirtualMemory
//
//  Created by Zhuocheng Xu on 11/21/19.
//  Copyright © 2019 Zhuocheng Xu. All rights reserved.
//
#include "main.h"
#include <iostream>
#include <fstream>
#include <list>
#include <map>
#include <unordered_map>


using namespace std;
int current_size = 0;
int frame_size = 16;
Process* current;
Process* processes[2];
list<Instruction> instructions;
Pager* pager;
unordered_map<frame_t*, pte_t*> frame_map;
list<frame_t*> frame_list;


frame_t allocate_frame_from_free_list(pte_t* pte){
    struct frame_t* f = new frame_t;
    if(current_size < frame_size){
        *f =  {true, current_size};
        current_size++;
        frame_list.push_back(f);
        frame_map.insert(make_pair(f,pte));
    }else{
        *f = {false, current_size};
        
    }
    
    return *f;
}

frame_t get_frame(pte_t* pte){
    frame_t f = allocate_frame_from_free_list(pte);
    cout<< "frame id = " << f.frame_id << "\n";
    if(!f.valid){
        f = pager->select_victim_frame(&frame_map, &frame_list);
    }
    return f;
}

void readInput(string filename){
    fstream file;
    string process_num, ve_num;
    string op, id;
    string start, end, write, mapped;
    int lineNumber = 0;
    file.open(filename.c_str());
    file >> process_num;
    
    for(int i=0; i<atoi(process_num.c_str()); i++){
        file >> ve_num;
        processes[i] = new Process(i);
        list<virtual_entry> vlist;
        for(int j=0; j<atoi(ve_num.c_str()); j++){
            file >> start >> end >> write >> mapped;
            int s = atoi(start.c_str());
            int e = atoi(end.c_str());
            bool w = write == "1";
            bool m = mapped == "1";
            struct virtual_entry ve = {s,e,w,m};
            vlist.push_back(ve);
        }
        processes[i]->vlist = vlist;
    }
    
    while (file >> op >> id){
        cout<< op << " "<< id<<"\n";
        instructions.push_back({op,atoi(id.c_str())});
    }
    
    
}

int main(int argc, const char * argv[]) {
    // insert code here...
    readInput("/Users/zhuocheng/Downloads/lab3_assign/inputs/in1");
    pager = new Clock();
    int counter = 0;
    for(list<Instruction>::iterator it = instructions.begin(); it != instructions.end(); ++ it){
        int va = it->virtual_address;
        cout<< counter << ": ==> " << it->op << " " << va<<"\n";
        if(it->op == "c"){
            current = processes[va];
        }else{
//            cout<< current->process_id <<"\n";
//            current ->pageTable;
//            pp->
//            pte_t t = current ->pageTable[0];
            pte_t* pte = &(current->pageTable[va]);
            if(pte->present == false){
                frame_t f = get_frame(pte);
                //init page table
                if(!pte->referenced){
                    bool found = false;
                    for(list<virtual_entry>::iterator itv = current->vlist.begin(); itv != current->vlist.end(); ++itv){
                        if(va >= itv->start && va <= itv->end){
                            pte->referenced = true;
                            pte->present = true;
                            pte->frame_id = f.frame_id;
                            pte->write_protected = itv->write_protected;
                            pte->file_mapped = itv->file_mapped;
                            found = true;
                            
                            break;
                        }
                    }
                    if(!found){
                        cout<< " SEGV\n";
                        pte->referenced = false;
                    }
                }
                if(pte->pagedOut && pte->file_mapped){
                    cout << " FIN\n";
                }else if(pte->pagedOut && !pte->file_mapped){
                    cout << " IN\n";
                }else{
                    cout << " ZERO\n";
                }
                frame_map[&f] = pte;
                cout<< " MAP " << f.frame_id<<"\n";
            }

            if(it->op == "w"){
                if(pte->write_protected){
                    cout<< " SEGPROT\n";
                }else{
                    pte->modified = true;
                }
                
            }
        }
        counter++;
    }
    return 0;
}
