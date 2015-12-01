
#include<iostream>
using namespace std;
#include"perceptron.h"
#define MAX_VALUE_HYPER_PARAMETER 5
//#define NO_OF_EPOCHS	10

bool myfunction (ratevector  i, ratevector j)
 { return (i.correct_test > j.correct_test); }
 
int main(int argc, char *argv[])
{
	FILE *fp;
	int noofDataFiles, noOfTrainingFiles;
	int NO_OF_EPOCHS = 10;
	char inputString[inputlength] = {"\0"};
	vector<ratevector> completeRatenMarginList, completeRateList;
	int numberOFDimensions = 0;
	double arrayofCs[NO_OF_C] = {0.001, 0.01, 0.1, 1, 10};
	double arrayofP[NO_OF_P] = {0.001, 0.01, 0.1, 1};
	string fileName, testFileName;
#ifndef VS2010
			testFileName = (string)argv[2];
						
			fileName = (string)argv[1];
#else
			testFileName = "D:\\ml-project\\svm\\astro\\original\\test";
			fileName = "D:\\ml-project\\svm\\astro\\original\\train";
#endif
			numberOFDimensions = atoi(argv[3]);
			int	NO_OF_ROWS = (numberOFDimensions == 10) ? 100 : 310 ;
			fp = fopen(fileName.c_str(), "r");
			if (fp == NULL )
			{
					cout << "unable to open file:" <<fileName.c_str()<<endl ;
					return -1;
			}
			vector<inputData> combineTable;
			for(int l =0; l < NO_OF_KFOLDS; l++)
			{
				inputData table;
				for(int m =0; m < NO_OF_ROWS; m++)
				{

					if (fgets(inputString, inputlength, fp))
					{
							insertDataintotable(inputString, table, numberOFDimensions);
					}
					else
						break;
				}
				combineTable.push_back(table);
			}
			inputData FinaltrainingSet;
			createtrainingSet(FinaltrainingSet , -1, combineTable);
			if(fp)
				fclose(fp);
			fp = NULL;
			
			//read test file
			fp = fopen(testFileName.c_str(), "r");
			if (fp == NULL )
			{
					cout << "unable to open file" ;
					return -1;
			}
			
			inputData FinaltestSet;
			while (fgets(inputString, inputlength, fp))
			{
				insertDataintotable(inputString, FinaltestSet, numberOFDimensions);
			}
			if(fp)
				fclose(fp);
			fp = NULL;
			//////
			double rate = 0;
			
			
			vector<ratevector> ratesandHyperParameterCList;
			vector<double> initialWeightVector; 
			initializerandomWeightVector(initialWeightVector, numberOFDimensions);
			for(int cloop = 0; cloop < NO_OF_C; cloop++) //
			{
				double hyperParameterC = arrayofCs[cloop];
				for(int rate = 0; rate < NO_OF_P; rate++)
				{
					double tempRate = arrayofP[rate];
					ratevector rateobbj;
					rateobbj.rate = tempRate;
					rateobbj.HyperParameterC =  hyperParameterC;
					for(int l =0; l < NO_OF_KFOLDS; l++) //Creating float table with 5 tables
					{
						rateobbj.weightVector = initialWeightVector;
						rateobbj.valueOft = 0;
						inputData trainingSet, testSet;
						cout<<"creating set"<<endl;
						createtrainingSet(trainingSet , l, combineTable);
						cout<<"finished set"<<endl;
						testSet = combineTable[l];
						applySGD(trainingSet, rateobbj);
						Test(testSet, rateobbj);
					}
					cout<< "Rate:"<< rateobbj.rate<<",rateobbj.HyperParameterC:"<< 
					rateobbj.HyperParameterC<<"correct:"<<rateobbj.correct_test<<
					"accuracy:"<<(rateobbj.correct_test/(double(rateobbj.TotalExamples)))<<endl;
					ratesandHyperParameterCList.push_back(rateobbj);
				}
			}
			sort (ratesandHyperParameterCList.begin(),ratesandHyperParameterCList.end(),myfunction);
			cout<<" best rate:"<< ratesandHyperParameterCList[0].rate<<",hyperParameterC:"
				<<ratesandHyperParameterCList[0].HyperParameterC<<endl;
			ratevector Finalrateobbj;
			Finalrateobbj.HyperParameterC = ratesandHyperParameterCList[0].HyperParameterC;
			Finalrateobbj.rate = ratesandHyperParameterCList[0].rate;
			Finalrateobbj.weightVector = initialWeightVector; 
			Finalrateobbj.no_of_epochs = 30;
			applySGD(FinaltrainingSet, Finalrateobbj);
			for(size_t w =0; w < Finalrateobbj.weightVector.size(); w++)
			{
				cout<< "  "<<Finalrateobbj.weightVector[w];
			}
			cout<<endl;
			Test(FinaltestSet, Finalrateobbj);
			cout<<"accuracy on final Test Set:"<<(Finalrateobbj.correct_test/double(FinaltestSet.size()))<<endl;
			
return 0;
}

