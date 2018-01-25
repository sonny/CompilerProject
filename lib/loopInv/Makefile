DEBUG = ../../Debug/lib
# add optLoads.o and sameRhs.o to the following line when ready
OBJS = printCode.o loopInv.o Dominance.o LoopFind.o InstructionInfo.o LoopInvCM.o

$(DEBUG)/P3.so: $(OBJS)
	mkdir -p $(DEBUG)
	g++  $(OBJS) -shared -o $@ -fPIC

%.o: %.cpp
	g++ -c -fno-rtti `llvm-config --cppflags` -gdwarf-2 -fPIC $< -o $@

clean:
	rm -f $(DEBUG)/P3.so $(OBJS)
