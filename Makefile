run: clean DSA-proj23.out
	./DSA-proj23.out 2>&1 | tee temp.log
DSA-proj23.out: DSA-proj23.c
	gcc -g -o DSA-proj23.out DSA-proj23.c
clean:
	rm -f *.o
	rm -f -- DSA-proj23