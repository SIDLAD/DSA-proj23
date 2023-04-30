DSA-proj23: clean Project
	./Project 2>&1 | tee temp.log
debug: Project
	gdb Project
Project: main.c RTree.o Algorithms.o Helper.o
	gcc -g main.c Helper.o Algorithms.o RTree.o -o Project
# main.o: main.c
# 	gcc -g -c main.c
RTree.o: RTree.c
	gcc -g -c RTree.c
Algorithms.o: Algorithms.c
	gcc -g -c Algorithms.c
Helper.o: Helper.c
	gcc -g -c Helper.c
clean:
	rm -f *.o
	rm -f -- Project