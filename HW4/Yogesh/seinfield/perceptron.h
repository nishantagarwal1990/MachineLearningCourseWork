#ifndef PERCEPTRON_H
#define PERCEPTRON_H
//#define NO_OF_ROWS 		310
#define NO_OF_KFOLDS 10
#define NO_OF_C	5
#define NO_OF_P 4
#define NO_OF_EPOCHS_CV  10
#define NO_OF_EPOCHS_TEST  30
#include<vector>
#include<map>
#include<string>
#include <string.h>
#include<math.h>
#include <fstream>
#include<cstdio>
#include <ctime>        // std::time
#include <cstdlib> 
#include <algorithm> 
using namespace std;
#define inputlength 1024*200

typedef struct ratevector_t
{
	double rate;
	double HyperParameterC;
	int mist_train;
	int  mist_test;
	int correct_test;
	int  correct_train;
	double accuracyonTesting;
	double accuracyonTraining;
	vector<double> weightVector;
	int dimensions;
	int valueOft;
	int no_of_epochs;
	int TotalExamples;
	ratevector_t()
	{
		rate = 0;
		mist_train = 0;
		mist_test = 0;
		accuracyonTesting = 0;
		accuracyonTraining = 0;
		HyperParameterC = 0;
		correct_test = 0;
		correct_train = 0;
		dimensions = 0;
		valueOft = 0;
		no_of_epochs =10;
		TotalExamples = 0;
	}
}ratevector;

typedef vector<vector<double> > inputData;
void  createtrainingSet(inputData &trainingSet , size_t indexTestSet,  vector<inputData> &combineTable);
int insertDataintotable( char* data, inputData &table, int noOfFeatures);
void shuffleData(inputData &inputTable);
bool	initializerandomWeightVector(vector<double> &weightVectorobj, int noOfFeatures);
bool	applySGD(inputData &trainingSet, ratevector &rateobbj);		
bool Test(inputData &testSet, ratevector &rateobj);
//double calculateaccurates( inputData &trainingSet ,inputData  &predictionSet, size_t hyperParameter, string mostfrequentlabel);
#endif
