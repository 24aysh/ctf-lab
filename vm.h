#ifndef VM_H
#define VM_H

#include <iostream>
#include <vector>
using namespace std;
enum REG{
    r0,
    r1,
    r2,
    r3,
    r4,
    r5,
    r6,
    r7
};
typedef struct 
{
    int ip;
    int zf;
    int halt;
    char memory[100000];
    int reg[8]; 
}state;

typedef struct
{
    char opcode;
    char op1;
    char op2;
} instr;

class VM
{
    state *vmState;
    instr instruction;
    
    void fetch();
    void decode();
    void execute();
public:
    VM();
    void run();
    void loadProgram(vector<int> prog);

};
#endif // VM_H