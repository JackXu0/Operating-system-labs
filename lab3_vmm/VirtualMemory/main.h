//
//  main.h
//  VirtualMemory
//
//  Created by Zhuocheng Xu on 11/21/19.
//  Copyright © 2019 Zhuocheng Xu. All rights reserved.
//

#ifndef main_h
#define main_h
#include <queue>
#include <list>
#include <iostream>
#include <unordered_map>

using namespace std;

const int PAGETABLE_SIZE = 64;

struct pte_t{
    int vma;
    int frame_id;
    int process_id;
    bool present;
    bool file_mapped;
    bool write_protected;
    bool modified;
    bool referenced;
    bool pagedOut;
};

struct Instruction{
    string op;
    int virtual_address;
};

struct virtual_entry{
    int start;
    int end;
    bool write_protected;
    bool file_mapped;
};

struct frame_t{
    bool valid;
    int frame_id;
};

class Process {
public:
    int process_id;
    list<virtual_entry> vlist;
    vector<pte_t> pageTable;
    Process(int process_id);
    void add_PTE(pte_t e);
};

class Pager {
public:
    string name;
    virtual frame_t select_victim_frame(unordered_map<frame_t*, pte_t*>* frame_map, list<frame_t*>* frame_list);
    Pager();
};

class FIFO:public Pager{
public:
    frame_t select_victim_frame(unordered_map<frame_t*, pte_t*>* frame_map, list<frame_t*>* frame_list);
    FIFO();
};

class Clock:public Pager{
public:
    frame_t* current_pointer;
    frame_t select_victim_frame(unordered_map<frame_t*, pte_t*>* frame_map, list<frame_t*>* frame_list);
    Clock();
};

//class Random:public Pager
//{
//public:
//    frame_t select_victim_frame(map<frame_t*, pte_t*> frame_list;
//    Random();
//};

#endif /* main_h */
