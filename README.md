# DSA-proj23

To run the Project, use the make file. 

1. ```$ make```  - This will clean the directory, compile the code and run the program. It also logs all outputs to ```temp.log```.

2. ```$ make clean``` - This will clean the directory. This is by default run when you run make, before new files are compiled.

_Do not copy the ```$``` symbol. It is just to indicate that the command is to be run in the terminal._

## Dependencies
1. ```gcc``` - GNU Compiler Collection. This is used to compile the code.
To install gcc, run the following command in the terminal:
```$ sudo apt install gcc```
2. ```make``` - This is used to run the make file. To install make, run the following command in the terminal:
```$ sudo apt install make```
3. _Optional_ ```gdb``` - GNU Debugger. This is used to debug the code. To install gdb, run the following command in the terminal:
```$ sudo apt install gdb```

## Assumptions

1. The input file is in the same directory as the code.
2. The input file has points for only two dimensions, in the format ```"x y\n"```, except for the last line which is just ```"x y"```.
3. If you wish to use the R-Tree for N-Dimensional points, then\
    a. Make sure to go to RTree.h and change the value that dim is defined to the number of dimensions\
    b. Create a new input file (for example 3D-data.txt), in the format:  ```"x y .. z\n"```, except for the last line which is just ```"x y .. z"```.\
    c. In main.c, go to line 18, and replace "data.txt" with the name of your file in double quotes.