all: manager.c
	gcc -o manager manager.c
clean:
	rm manager
run:
	./manager NEXTFIT 268435456 testscript.txt