# DSA-proj23

To run the Project, follow these instructions.

1. ```$ gcc -o DSA-proj23.out DSA-proj23.c```  - This will compile the code.

2. ```$ ./DSA-proj23.out``` - This will run the executable and display outputs.

3. _(Optional) ```$ ./DSA-proj23.out 2>&1 | tee temp.log``` - This will run the executable, display outputs and log all outputs simultaneously to a file called ```temp.log```._

_Do not copy the ```$``` symbol. It is just to indicate that the command is to be run in the terminal._

By default this looks for a file called ```data.txt``` in the same directory as the code. To change this, edit the string ```"data.txt"``` in line 734 of ```DSA-proj23.c``` to the path of the file you want to use. We recommend you use the absolute path, and that the path has no spaces or special characters in it (If it does, please use the escape character ```\``` before the special character and/or deal with it appropriately).

## Dependencies
1. ```gcc``` - GNU Compiler Collection. This is used to compile the code. Tested on ```gcc version 11.3.0 (Ubuntu 11.3.0-1ubuntu1~22.04) ```.
To install gcc, run the following command in the terminal:
```$ sudo apt install gcc```
2. _Optional_ ```gdb``` - GNU Debugger. This is used to debug the code. To install gdb, run the following command in the terminal:
```$ sudo apt install gdb```

## Assumptions

1. The input file is in the same directory as the code.
2. The input file has points for only two dimensions, in the format ```"x y\n"```, except for the last line which is just ```"x y"```.
3. If you wish to use the R-Tree for N-Dimensional points, then\
    a. Make sure to go to DSA-proj23.c and change the value that dim is defined to the number of dimensions (line 8).\
    b. Create a new input file (for example 3D-data.txt), in the format:  ```"x y .. z\n"```, except for the last line which is just ```"x y .. z"```.\
    c. In DSA-proj23.c, go to line 737 (inside the main function), and replace "data.txt" with the name of your file in double quotes.