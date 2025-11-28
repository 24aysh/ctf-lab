#ifndef VM_H
#define VM_H

#include <iostream>
#include <vector>
#include <cstdint>
#include <string>
using namespace std;

enum REG {
    r0, r1, r2, r3, r4, r5, r6, r7
};

typedef struct {
    int ip;
    int zf;
    int halt;
    uint8_t memory[1000000];
    int reg[8]; 
}state;

typedef struct {
    uint8_t opcode;
    uint8_t op1;
    uint8_t op2;
} instr;

class VM
{
public:
    state *vmState;
    instr instruction;
    
    void fetch();
    void decode();
    void execute();

    VM();
    ~VM();
    void run();
    void loadProgram(const vector<int> &prog);
    void loadInput(const std::string &input);
    uint8_t getMemoryByte(size_t idx) const;
    uint32_t read_u32_at(size_t idx) const;
};
#endif // VM_H