#include "vm.h"
#include <cstring>

using namespace std;

VM::VM() {
    vmState = new state();
    memset(vmState, 0, sizeof(state));
    memset(&instruction, 0, sizeof(instr));
}
VM::~VM() {
    delete vmState;
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
        bool val = vmState->reg[instruction.op1] == vmState->reg[instruction.op2];
        if(!val){
            vmState->ip = instruction.op1;
        }
        break;
    case 0x42: //JMP IF A==B
        bool val = vmState->reg[instruction.op1] == vmState->reg[instruction.op2];
        if(val){
            vmState->ip = instruction.op1;
        }
        break;
    case 0x43: // CMP
        bool val = vmState->reg[instruction.op1] == vmState->reg[instruction.op2];
        vmState->zf = (val ? 1:0);
        break;    
    case 0x44: // CMPI 
        bool val = vmState->reg[instruction.op1] == instruction.op2;
        vmState->zf = (val ? 1:0);
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

void VM::loadProgram(vector<int> prog){
    for(int i=0;i<prog.size();i++){
        vmState->memory[98+i] = prog[i];
    }
}

void VM::run(){
    vmState->ip -=3;
    while(!vmState->halt){
        fetch();
        decode();
        execute();
    }
}
