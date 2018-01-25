# Retain intermediate bitcode files
.PRECIOUS: %.bc

# The default target builds the plugin
plugin:
	make -C lib/p3

# create .bc from source
%.bc:	%.c
	clang -emit-llvm -O0 -c $*.c -o $*.bc

# create human-readable assembly (.ll) from .bc
%.ll: %.bc
	llvm-dis -f $^

# create executable from .bc
%.exe: %.bc
	llc $^
	gcc $*.s -o $*.exe

# create bitcode optimized to keep vars in registers
%.mem2reg: %.bc
	opt -mem2reg $^ -o $@
#	mv $*.mem2reg $*.bc

# run printCode on a .bc file
%.printCode: %.bc plugin
	opt -load Debug/lib/P2.so -printCode $*.bc > /dev/null
#	mv $*.loopInv $*.bc

# clean up generated files
clean:
	make -C lib/p3 clean
	rm -f *.bc *.exe *.ll *.s *.mem2reg *.loopInv
