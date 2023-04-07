# Welcome to Calculator
Calculator works for non-negative integers. It supports the following operations: +, *, ^, /, number system conversion. To describe an operation in an input file put an operation symbol (+, *, ^ or /) followed by a space and a number system. After that write two numbers in two separate lines with an empty line between them in the appropriate number system.
Example:

\+ 16

234BF4F7A

234BB6B7C3

You may put many operations in one file.

In order to perform a number system conversion put two number systems and a number written in the first one of the two systems. It will be
converted two the second number system.
Example:

10 2

49235349569

In order to run the program add the input file name after the execute file:
```
./calculator.exe in.txt
```
Each operation description will be rewritten to the output file with the result of the operation.
If you also add "manyFiles", each operation will be rewritten to a different file:
```
./calculator.exe in.txt manyFiles
```
