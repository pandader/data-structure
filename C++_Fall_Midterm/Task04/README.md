# FE 522 Fall 2019 Midterm Exam Task 4 (30 points)

Given the vector X of floating point numbers in the main.cpp file, design 2 functions that calculate the expected value and the variance of the sample contained in the vector, respectively.

The sample mean of a data set X = (x_1, x_2, ..., x_n) is  
E[X] = (x_1 + x_2 + ... + x_n) / n  

The sample variance of a data set X = (x_1, x_2, ..., x_n) is  
s^2 = (1 / n) * ((x_1 - E[X])^2 + (x_2 - E[X])^2 + ... + (x_n - E[X])^2)  

Put the function prototypes into a header file with name func.h. Put the function definitions into a source file called func.cpp. Include func.h in main.cpp and call the functions to output the results onto the standard output.

You will also have to modify your CMakeLists.txt file accordingly in the process.
