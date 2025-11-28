#include "vm.h"
#include <cstring>
#include <stdexcept>
#include <algorithm>

using namespace std;

static inline void write_u32_le(uint8_t *mem, uint32_t addr, uint32_t v) {
    mem[addr]   = static_cast<uint8_t>( v        & 0xFF);
    mem[addr+1] = static_cast<uint8_t>((v >> 8)  & 0xFF);
    mem[addr+2] = static_cast<uint8_t>((v >> 16) & 0xFF);
    mem[addr+3] = static_cast<uint8_t>((v >> 24) & 0xFF);
}

static inline uint32_t read_u32_le(const uint8_t *mem, uint32_t addr) {
    return  uint32_t(mem[addr]) |
           (uint32_t(mem[addr+1]) << 8) |
           (uint32_t(mem[addr+2]) << 16) |
           (uint32_t(mem[addr+3]) << 24);
}

VM::VM() {
    vmState = new state();
    memset(vmState, 0, sizeof(state));
    memset(&instruction, 0, sizeof(instr));
    vmState->reg[r7] = static_cast<int>(sizeof(vmState->memory));
}

VM::~VM() {
    delete vmState;
}


void VM::loadProgram(const std::vector<int>& prog) {
    const size_t LOAD_OFFSET = 98;
    if (LOAD_OFFSET + prog.size() >= sizeof(vmState->memory))
        throw runtime_error("Program too large to fit into VM memory");

    for (size_t i = 0; i < prog.size(); ++i) {
        vmState->memory[LOAD_OFFSET + i] = static_cast<uint8_t>(prog[i] & 0xFF);
    }
}

void VM::loadInput(const std::string &input) {
    const size_t HEADER = 4;

    write_u32_le(vmState->memory, 0, static_cast<uint32_t>(input.size()));

    for (size_t i = 0; i < input.size(); ++i) {
        vmState->memory[HEADER + i] = static_cast<uint8_t>(input[i]);
    }
}

uint8_t VM::getMemoryByte(size_t idx) const {
    if (idx >= sizeof(vmState->memory)) throw runtime_error("memory index out of range");
    return vmState->memory[idx];
}

uint32_t VM::read_u32_at(size_t idx) const {
    if (idx + 3 >= sizeof(vmState->memory)) throw runtime_error("read_u32_at: out of range");
    return read_u32_le(vmState->memory, static_cast<uint32_t>(idx));
}

void VM::run(){
    vmState->ip -=3;
    while(!vmState->halt){
        fetch();
        decode();
        execute();
    }
}
void VM::fetch(){
    vmState->ip+=3;
}
void VM::decode(){
    int opcode = vmState->memory[99+vmState->ip];
    int op1 = vmState->memory[99+vmState->ip+1];
    int op2 = vmState->memory[99 +vmState->ip+2];

    instruction.op1 = op1;
    instruction.op2 = op2;
    instruction.opcode = opcode;
}
void VM::execute(){
    int opcode = instruction.opcode;
    bool val;
    switch (opcode)
    {
    case 0x1: //MOVI
        vmState->reg[instruction.op1] = instruction.op2;
        break;
    case 0x2: // MOV
        vmState->reg[instruction.op1] = vmState->reg[instruction.op2];
        break;
    case 0x3: // ADD
        vmState->reg[instruction.op1] += vmState->reg[instruction.op2];
        break;
    case 0x4: // ADDI
        vmState->reg[instruction.op1] += instruction.op2;
        break;
    case 0x5: // SUB
        vmState->reg[instruction.op1] -= vmState->reg[instruction.op2];
        break;
    case 0x6: // SUBI
        vmState->reg[instruction.op1] -= instruction.op2;
        break;
    case 0x7: // MUL
        vmState->reg[instruction.op1] *= vmState->reg[instruction.op2];
        break;
    case 0x8: // MULI
        vmState->reg[instruction.op1] *= instruction.op2;
        break;
    case 0x9: // XOR
        vmState->reg[instruction.op1] ^= vmState->reg[instruction.op2];
        break;
    case 0x10: // XORI
        vmState->reg[instruction.op1] ^= instruction.op2;
        break;
    case 0x11: // AND
        vmState->reg[instruction.op1] = vmState->reg[instruction.op1] && vmState->reg[instruction.op2];
        break;
    case 0x12: // ANDI
        vmState->reg[instruction.op1] = vmState->reg[instruction.op1] && instruction.op2;
        break;
    case 0x13: // OR
        vmState->reg[instruction.op1] = vmState->reg[instruction.op1] || vmState->reg[instruction.op2];
        break;
    case 0x14: // ORI
        vmState->reg[instruction.op1] = vmState->reg[instruction.op1] || instruction.op2;
        break;
    case 0x15: // SHL
        vmState->reg[instruction.op1] = vmState->reg[instruction.op1] << vmState->reg[instruction.op2];
        break;
    case 0x16: // SHLI
        vmState->reg[instruction.op1] = vmState->reg[instruction.op1] << instruction.op2;
        break;
    case 0x17: // SHR
        vmState->reg[instruction.op1] = vmState->reg[instruction.op1] >> vmState->reg[instruction.op2];
        break;
    case 0x18: // SHRI
        vmState->reg[instruction.op1] = vmState->reg[instruction.op1] >> instruction.op2;
        break;
    case 0x24: //CALL
        vmState->reg[7] = vmState->ip;
        vmState->ip = instruction.op1;
        break;
    case 0x25: //RETURN
        vmState->ip = vmState->reg[7];
        break;
    case 0x40: //JMP
        vmState->ip = instruction.op1;
        break;
    case 0x41: //JMP IF A!=B
        val = vmState->reg[instruction.op1] == vmState->reg[instruction.op2];
        if(!val){
            vmState->ip = instruction.op1;
        }
        break;
    case 0x42: //JMP IF A==B
        val = vmState->reg[instruction.op1] == vmState->reg[instruction.op2];
        if(val){
            vmState->ip = instruction.op1;
        }
        break;
    case 0x43: // CMP
        val = vmState->reg[instruction.op1] == vmState->reg[instruction.op2];
        vmState->zf = (val ? 1:0);
        break;    
    case 0x44: // CMPI 
        val = vmState->reg[instruction.op1] == instruction.op2;
        vmState->zf = (val ? 1:0);
        break;
    case 0x70: //LOAD
        vmState->reg[instruction.op1] = vmState->memory[instruction.op2];
        break;
    case 0x80: //STORE
        vmState->reg[instruction.op2] = vmState->memory[instruction.op1];
        break;
    case 0x90: // NOP
        break;
    case 0x91: // HALT
        vmState->halt = 1;
        break;

    
    default:
        break;
    }
}
