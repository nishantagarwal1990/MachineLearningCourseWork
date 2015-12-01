#!/bin/bash

echo -n "Enter 1 for ID3 and 2 for KNN:"
read value
if [ $value -eq 1 ]
then
	echo " Enter 1 to Compile ID3 , 2 to run the executable on assignment data :"
	read newval
	if [ $newval -eq 1 ] ; then 
		g++ -std=c++11 ID3/decisiontree.cpp -o ID3/ID3.out
	fi		
	if [ $newval -eq 2 ] ; then
		if [ -s ID3/ID3.out ] ; then 
			cd ID3
			./ID3.out ../Data/tic-tac-toe-train-1.txt ../Data/tic-tac-toe-train-2.txt ../Data/tic-tac-toe-train-3.txt ../Data/tic-tac-toe-train-4.txt ../Data/tic-tac-toe-train-5.txt ../Data/tic-tac-toe-train-6.txt ../Data/tic-tac-toe-test.txt
		else 
			g++ -std=c++11 ID3/decisiontree.cpp -o ID3/ID3.out
			cd ID3
			./ID3.out ../Data/tic-tac-toe-train-1.txt ../Data/tic-tac-toe-train-2.txt ../Data/tic-tac-toe-train-3.txt ../Data/tic-tac-toe-train-4.txt ../Data/tic-tac-toe-train-5.txt ../Data/tic-tac-toe-train-6.txt ../Data/tic-tac-toe-test.txt
		fi
	fi
else
	echo " Enter 1 to Compile KNN , 2 to run the executable on assignment data:"
	read secval
	if [ $secval -eq 1 ] ; then
		g++ -std=c++11 KNN/KNN.cpp -o KNN/KNN.out
	fi
	if [ $secval -eq 2 ]
	then
		if [ -s KNN/KNN.out ] 
		then
			cd KNN
			./KNN.out ../Data/tic-tac-toe-train-1.txt ../Data/tic-tac-toe-train-2.txt ../Data/tic-tac-toe-train-3.txt ../Data/tic-tac-toe-train-4.txt ../Data/tic-tac-toe-train-5.txt ../Data/tic-tac-toe-train-6.txt ../Data/tic-tac-toe-test.txt
		else
			g++ -std=c++11 KNN/KNN.cpp -o KNN/KNN.out
			cd KNN
			./KNN.out ../Data/tic-tac-toe-train-1.txt ../Data/tic-tac-toe-train-2.txt ../Data/tic-tac-toe-train-3.txt ../Data/tic-tac-toe-train-4.txt ../Data/tic-tac-toe-train-5.txt ../Data/tic-tac-toe-train-6.txt ../Data/tic-tac-toe-test.txt
		fi
	fi
fi
	
		
			