all: project2.c
	gcc -o project2 printresults.c
clean:
	rm project2
run:
	./project2 NEXTFIT 268435456 testscript.txt