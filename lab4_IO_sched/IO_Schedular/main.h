//
//  main.h
//  IO_Schedular
//
//  Created by Zhuocheng Xu on 12/1/19.
//  Copyright © 2019 Zhuocheng Xu. All rights reserved.
//

#ifndef main_h
#define main_h

#include <list>
using namespace std;

class IO_Operation {
public:
    int id;
    int arriving_time;
    int start_time;
    int finishing_time;
    int track;
    IO_Operation(int id, int at, int t);
};

class IO_Schedular {
public:
    void insertExpired(IO_Operation* op);
    virtual IO_Operation* get_next_operation(int track);
    IO_Schedular();
};

class FIFO:public IO_Schedular{
public:
    IO_Operation* get_next_operation(int track);
    FIFO();
};

class SSTF:public IO_Schedular{
public:
    IO_Operation* get_next_operation(int track);
    SSTF();
};

class LOOK:public IO_Schedular{
public:
    int moving_right;
    IO_Operation* get_next_operation(int track);
    LOOK();
};

class CLOOK:public IO_Schedular{
public:
    IO_Operation* get_next_operation(int track);
    CLOOK();
};

class FLOOK:public IO_Schedular{
public:
    int moving_right;
    list<IO_Operation*> add_queue;
    list<IO_Operation*> active_queue;
    IO_Operation* get_next_operation(int track);
    FLOOK();
};

extern list<IO_Operation*> all_operations;
extern list<IO_Operation*> active_operations;
extern list<IO_Operation*> expired_operations;

#endif /* main_h */
