The folder is divided into subfolders. 

1>The ID3 folder contains the source files for Decision Tree and ID3 implementation. 

2>The KNN folder contains source files for KNN and k-cross validation implementation.

3> The Homework1.pdf contains solutions as well as report for the programming assignments.

4> The data provided for assignment is in the Data folder.

Steps to execute:

1> Open a terminal and enter the directory where the folder is unzipped.

2> Run ./run.sh on the terminal for interactive details of what is expected.






ID3:

1> The executable assumes that the last argument passed to it is the test file.

e.g. ./ID3.out train1.txt train2.txt test.txt

2> The output of the file is printed to a file "DecisionTreeOutput.txt" in the same folder
as the executable.




KNN:

This is hardcoded to assume that we are given 6 training files and 1 final test file.

1> The executable assumes that the last argument entered is the final test file.

2> The initial files will be used for k-cross validation and then the test file will be used.

e.g. ./KNN.out train1.txt train2.txt test.txt

3> The average accuracy of each K hyperparamenter and final accuracy wil be printed into "KNNOutput.txt"

