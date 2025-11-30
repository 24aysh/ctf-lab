#ifndef VM_H
#define VM_H

#include <iostream>
#include <vector>
#include <cstdint>
#include <string>
using namespace std;

typedef struct {
    int ip;
    int zf;
    int halt;
    uint8_t memory[1000000];
    uint8_t sbox[256];
    uint8_t key[39];
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
    instr i;
    
    void fetch();
    void decode();
    void execute();

    VM();
    ~VM();
    void run();
    void loadProgram(const vector<int> &prog);
    void loadInput(const std::string &input);
    void loadSbox(const vector<int> &sbox);
    void loadKey(const vector<int> &key);
    uint8_t getMemoryByte(size_t idx) const;
    uint32_t read_u32_at(size_t idx) const;
};
#endif // VM_H