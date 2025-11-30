import sys

OPCODES = {
    "movi" : 0x1,
    "mov" : 0x2,
    "add"  :  0x3,
    "addi" : 0x4,
    "sub" : 0x5,
    "subi": 0x6,
    "mul" : 0x7,
    "muli" : 0x8,
    "xor" : 0x9,
    "xori" : 0x10,
    "and" : 0x11,
    "andi" : 0x12,
    "or" : 0x13,
    "ori" : 0x14,
    "shl" : 0x15,
    "shli" : 0x16,
    "shr" : 0x17,
    "shri" : 0x18,
    "movs" : 0x19,
    "movk" : 0x20,
    "load" : 0x70,
    "store" : 0x80,
    "call" : 0x24,
    "ret" : 0x25,
    "jmp" : 0x40,
    "jnz"  : 0x41,
    "jz": 0x42,
    "njmp": 0x50,
    "cmp" : 0x43,
    "cmpi" : 0x44,
    "nop" : 0x90,
    "halt" : 0x91
}
byteCode = []
f = open("assembly.txt","r")
for line in f:
    instr = line.strip().split(" ")
    byteCode.append(hex(int(OPCODES[instr[0]])))
    byteCode += instr[1:]

for i in byteCode:
    print(i,end=",")
print()