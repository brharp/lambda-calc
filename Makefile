
OBJECTS = eval.o exp.o read.o print.o env.o mystdlib.o char.o
CFLAGS = -g 

a.out: $(OBJECTS)
	$(CC) $(OBJECTS)

clean:
	rm -f *.o

dist: lambda
	tar -czf lambda-calc.tgz -C .. lambda-calc

test: a.out
	a.out < test.l

tags: *.c *.h
	ctags *.c *.h

