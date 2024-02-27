#COSC 3360: HW 1


##Description
This code desgined to read two .txt files. Specifying input varaibles and the arthmetic operations to be performed
to be put into or using the internal variables(pipes).

The program uses the argument manager for the .txt files to parse and extract the input variables, it's operations and 
the internal varaibles its being sent to or the internal varaibles operations to another internal variable.

The program uses internal communication with inter-process communication using pipes using pipes with the child process
containing the output and the parent proces to execute and assgin the desired arithmetic operations.

All of this is being used with the pipe() class system for Linux-based system.

##How to run
1. Complie the code my entering inside the terminal: 'g++ -o test main.cpp'
2. Execting the compiled program in the terminal with the input files. Ex: './test input1-1.txt s1-1.txt'

##Credits
