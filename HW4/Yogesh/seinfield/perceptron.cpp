#include<iostream>
#include"perceptron.h"
    // std::sort

// random generator function:
int myrandom (int i) { return std::rand()%i;}
int insertDataintotable( char* data, inputData &table, int noOfFeatures)
{
        char * pch;
		
        //cout<< "Splitting string"<<  data<<" into tokens:\n"<<"noOfFeatures="<<noOfFeatures<<endl;
        pch = strtok (data ," \n");
        vector<double> row;
		row.push_back((atof(pch) == 1) ? 1 : -1);
		int indexValue = -1;
//		double featureValue = 0;
		do       
        {
            pch = strtok (NULL, " \n");
			if(pch == NULL)
				break;
			char *colonValue = strstr(pch, ":");
			char temp[4] = "\0";
			strncpy (temp,pch,(colonValue - pch));
		//	cout<<" pch -->"<<pch<<"colonValue:-->"<<colonValue<<endl;
		//	cout<<"temp value --"<<temp<<endl;
			int index = atoi(temp);
		//	cout<<"element: "<<index<<endl;
			for(int i =indexValue; i < index -1 ; i++  ) //Push 1 for all the sparse values
			{
				row.push_back(1);
		//		cout<<"making way for sparse"<<endl;
			}
			colonValue++;
		//	double cvalue = (atof(colonValue));
		//	cout<<"cvalue --"<<cvalue<<endl;
			row.push_back(atof(colonValue));
			indexValue = index;
			
		} while (pch != NULL);
		for(int i =indexValue; i < noOfFeatures; i++  ) //Push 1 for all the sparse values
		{
				row.push_back(1);
				cout<<"making way for sparse don"<<endl;
		}
	//	cout<<"row size"<<row.size()<<endl;
        table.push_back(row);
	/*	for( size_t i =0; i < row.size(); i++)
        {
            cout<<"row: "<<row[i];
        }
	cout<<endl;*/
        return 0;
}
void shuffleData(inputData &inputTable)
{
	  // using built-in random generator:
  //std::random_shuffle ( inputTable.begin(), inputTable.end() );
  for(size_t i=0; i < inputTable.size(); ++i)
  {
	std::random_shuffle ( inputTable.begin(), inputTable.end() );	
  }
}

void  createtrainingSet(inputData &trainingSet , size_t indexTestSet,  vector<inputData> &combineTable)
{
	for(size_t i = 0; i < combineTable.size(); i++)
	{
		if(indexTestSet == i)
			continue;
		trainingSet.insert(trainingSet.end(), combineTable[i].begin(), combineTable[i].end());
	}
}

bool	initializerandomWeightVector(vector<double> &weightVectorobj, int noOfFeatures)
{
	for(int i =0; i <= noOfFeatures; i++)
	{
		weightVectorobj.push_back(0);
	}
	return true;
}
double calculateRate(ratevector &rateobbj)
{
	double rate = (rateobbj.HyperParameterC*rateobbj.rate) /(double)(rateobbj.HyperParameterC + 
					(rateobbj.valueOft *rateobbj.rate));
	return rate;
}

bool	applySGD(inputData &trainingSet, ratevector &rateobbj)
{
	
	for(int l =0; l < rateobbj.no_of_epochs; l++)
	{
		shuffleData(trainingSet); //shuffle the data
		for(size_t i = 0; i< trainingSet.size(); ++i)
		{
			double rate = calculateRate(rateobbj);
			rateobbj.valueOft++;
			double result = 0;
			double correctValue = trainingSet[i][0];
			for(size_t j = 0; j < rateobbj.weightVector.size(); ++j)
			{
				result +=  rateobbj.weightVector[j] * trainingSet[i][j+1];
			}
			result = result*correctValue;
			//Its on or below the margin
			for(size_t j = 0; j < rateobbj.weightVector.size(); ++j)
			{
				if(result <= 1)
				{
					rateobbj.weightVector[j] =  rateobbj.weightVector[j] - (rate*(rateobbj.weightVector[j] -
							(rateobbj.HyperParameterC*correctValue*(trainingSet[i][j+1]))));
				}
				else
					rateobbj.weightVector[j] = rateobbj.weightVector[j] - (rate* rateobbj.weightVector[j]);
			}
		}
	}
		return true;
}

bool Test(inputData &testSet, ratevector &rateobj)
{
	for(size_t i = 0; i < testSet.size(); ++i)
	{
		double result = 0;
		double correctValue = testSet[i][0];
		for(size_t j = 0; j < rateobj.weightVector.size(); ++j)
		{
			result +=  rateobj.weightVector[j] * testSet[i][j+1];
		}
		result = result*correctValue;
		if(result <= 0)
		{
			//Miss prediction
			rateobj.mist_test++;
		}
		else
		{
			rateobj.correct_test++;
		}
		rateobj.TotalExamples++;
		
	}
	return true;
}


/*
int calculatedistance( vector<string> &p1, vector<string> &t1, distanceIndex &obj)
{
	int distance = 0;
	for( size_t i = 0; i < p1.size() -1 ; i++)
	{
		if(p1[i].compare(t1[i]) != 0)
			distance++;
	}
	obj.distance = distance;
	obj.output = t1[t1.size()-1];
	return 0;
}
bool isDistancequalifies( vector<distanceIndex> &kdistances, distanceIndex &dist, size_t hyperParameter)
{
	if(kdistances.size() < hyperParameter)
	{
		kdistances.push_back(dist);
		return true;
	}
	else
	{
   	 	std::sort (kdistances.begin(),kdistances.end(), myfunction);
		if( dist.distance < kdistances[kdistances.size()-1].distance)
		{
			kdistances.pop_back();
			kdistances.push_back(dist);
			return true;
		}
			

	}
	return false;
}
void predictLabels( vector<distanceIndex> &kdistances, string &predictLabel, string mostfrequentlabel)
{
	int label = 0;
	for(size_t j = 0; j < kdistances.size(); j++)
	{
		if((kdistances[j].output).compare("positive") == 0)
			label++;
		else
			label--;
	}
	if(label == 0)
		predictLabel = mostfrequentlabel;
	if(label > 0)
		predictLabel = "positive";
	else
		predictLabel = "negative";

}
double calculateaccurates( inputData &trainingSet ,inputData  &predictionSet, size_t  hyperParameter, string mostfrequentlabel)
{
	int accurates = 0;
	for(size_t i = 0; i < predictionSet.size(); i++)
	{
		vector<distanceIndex> kdistances;
		for(size_t j = 0; j < trainingSet.size(); j++)
		{
			distanceIndex obj;
			int distance = calculatedistance(predictionSet[i], trainingSet[j], obj);
			isDistancequalifies( kdistances, obj, hyperParameter);
		}
		string predictLabel = "";
		predictLabels(kdistances, predictLabel, mostfrequentlabel);
		if(predictLabel.compare(predictionSet[i][predictionSet[i].size()-1]) == 0)
			accurates++;	
	}
	return (accurates/(double)predictionSet.size());
}

*/
