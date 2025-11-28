CFLAGS=-std=c++23
	
vm: vm.o main.o
	@$(CXX) $(CFLAGS) vm.o main.o -o vm

main.o: main.cpp
	@$(CXX) $(CFLAGS) -c main.cpp

vm.o: vm.h vm.cpp
	@$(CXX) $(CFLAGS) -c vm.cpp

clean:
	@rm -f *.o vm