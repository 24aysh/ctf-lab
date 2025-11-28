#include "vm.h"
#include <iostream>
#include <string>
using namespace std;

int main() {
    string input;
    if (!(cin >> input)) return 0;
    if(input.size() > 90){
        cout << "the flag is not that long bruh...\n";
    }

    VM vm;
    vm.loadInput(input);
cout << (int)vm.vmState->memory[5] << endl;    return 0;
}
