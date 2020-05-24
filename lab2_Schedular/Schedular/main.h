//
//  main.h
//  Schedular
//
//  Created by Zhuocheng Xu on 10/7/19.
//  Copyright © 2019 Zhuocheng Xu. All rights reserved.
//

#ifndef main_h
#define main_h
#include <iostream>
#include <queue>
#include <list>
using namespace std;

enum STATE {STATE_CREATED, STATE_READY, STATE_RUNNING, STATE_BLOCKED, STATE_PREEMPTED};
enum TRANSACTION {TRANS_TO_READY, TRANS_TO_RUN,  TRANS_TO_BLOCK};


class Process {
private:
    
public:
    int pid;
    int AT;   // Arrival Time
    int TC;   // Total CPU Time
    int CB;   // CPU Burst
    int IO;   // IO Burst
    int cpu_burst;
    int PRIO;
    int TC_origin;
    int AT_origin;
    int finishing_time;
    int current_prio;
    int io_time;
    int state_ts;
    int CW;
//    int supposed_AT;
    string state_str;
    enum STATE state;
    
    Process(int id, int at, int tc, int cb, int io, int prio);
};

class Schedular {
private:
    
public:
    int quantum;
    string type;
    int blockTill;
    int non_overleap_io;
    bool is_preemitive;
    list<Process*> activeQueue, expiredQueue;
    virtual void add_active_process(Process* p);
    virtual void add_expired_process(Process* p);
    virtual Process* get_next_process();
    virtual Process* get_next_process(int timestamp);
//    virtual void test_preempt(Process *p, int curtime);
    
    Schedular(int quantum);
    
};

class FCFS:public Schedular
{

public:
    bool isEmpty();
    void add_active_process(Process* p);
    void add_expired_process(Process* p);
    Process* get_next_process(int timestamp);
//    void test_preempt(Process *p, int curtime);
    FCFS(int quantum);
};

class LCFS:public Schedular
{
public:
    bool isEmpty();
    void add_active_process(Process* p);
    void add_expired_process(Process* p);
    Process* get_next_process(int timestamp);
    void test_preempt(Process *p, int curtime);
    LCFS(int quantum);
};

class SRTF:public Schedular
{
public:
    bool isEmpty();
    void add_active_process(Process* p);
    void add_expired_process(Process* p);
    Process* get_next_process(int timestamp);
    void test_preempt(Process *p, int curtime);
    SRTF(int quantum);
};

class RR:public Schedular
{
public:
    bool isEmpty();
    void add_active_process(Process* p);
    void add_expired_process(Process* p);
    Process* get_next_process(int timestamp);
    void test_preempt(Process *p, int curtime);
    RR(int quantum);
};

class PRIO:public Schedular
{
public:
    int maxprio;
    bool isEmpty();
    void add_active_process(Process* p);
    void add_expired_process(Process* p);
    Process* get_next_process(int timestamp);
    void test_preempt(Process *p, int curtime);
    PRIO(int quantum, int maxprio);
};

class Pre_PRIO:public Schedular
{
public:
    int maxprio;
    bool isEmpty();
    void add_active_process(Process* p);
    void add_expired_process(Process* p);
    Process* get_next_process(int timestamp);
    void test_preempt(Process *p, int curtime);
    Pre_PRIO(int quantum, int maxprio);
};

class Event {
private:
    int timestamp;
    Process* evtProcess;
    enum TRANSACTION transaction;
public:
    int get_next_event_time();
    int get_timestamp() const;
    Process* get_evt_process() const;
    TRANSACTION get_transaction();
    Event(int ts, Process* p, TRANSACTION t);
};

class DES {
private:
    
public:
    
    list<Event*> evt_queue;
    void insert_event(Event* e);
    Event* get_event();
    void delete_event();
    DES();
};

#endif /* main_h */
