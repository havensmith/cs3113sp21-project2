all: project2.c
	gcc -o project2 printout.c
clean:
	rm project2
run:
	./project2 NEXTFIT 268435456 testscript.txt