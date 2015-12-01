//
//  KNN.cpp
//  ML2
//
//  Created by Nishant Agarwal on 9/16/15.
//  Copyright (c) 2015 Nishant Agarwal. All rights reserved.
//

#include "KNN.hpp"

bool comparator ( const pair<int,string>& l, const pair<int,string>& r)
{ return l.first < r.first; }

void CreateAttributeTable(table &attrtable, string filename)
{
    ifstream file;
    file.open(filename);
    
    string line;
    while(getline(file,line))
    {
        vector<string> columnvalues;
        while (line.length() !=0 && line.find(',') != string::npos)
        {
            size_t position;
            string value;
            
            position = line.find_first_of(',');
            value = line.substr(0, position);
            columnvalues.push_back(value);
            line.erase(0, position+1);
        }
        columnvalues.push_back(line);
        attrtable.push_back(columnvalues);
        columnvalues.clear();
        
    }
    file.close();
}
string SortNGetLabel(vector<pair<int,string>> &distnlabel, vector<string> &labels, int k)
{
    int p=0,n=0;
    sort(distnlabel.begin(),distnlabel.end(), comparator);
    
    for(int i =0;i < k; ++i)
    {
        if(labels[0] == distnlabel[i].second)
            ++p;
        else
            ++n;
    }

    if(p>n)
        return labels[0];
    else
        return labels[1];
    
}

double CalculateAccuracy(vector<string> &givenlabels, vector<string> &predictedlabels)
{
    double correct = 0.0;
    for (int i = 0; i < givenlabels.size(); ++i) {
        
        if (givenlabels[i] == predictedlabels[i])
            ++correct;
    }
    return (double) (correct/(double)(givenlabels.size()))* 100;
}

double CalculateKNNAccuracy(table &traindata,table &testdata,vector<string> &labels,int k)
{
    vector<pair<int,string>> distnlabel;
    pair<int,string> distlbpair;
    vector<string> predictedlabels;
    vector<string> givenlabels;
    //vector<string> singleinstance;
    
    
    for(int i=0;i<testdata.size();++i)
    {
        for(int j = 0; j < traindata.size(); ++j)
        {
            int dist = 0;
            for(int l = 0; l < traindata[0].size()-1; ++l)
            {
                if(traindata[j][l] != testdata[i][l])
                    ++dist;
            }
            distlbpair.first = dist;
            distlbpair.second = traindata[j][traindata[0].size()-1];
            distnlabel.push_back(distlbpair);
        }
        predictedlabels.push_back(SortNGetLabel(distnlabel,labels,k));
        distnlabel.clear();
    }
    
    for(int n = 0; n<testdata.size(); ++n)
    {
        givenlabels.push_back(testdata[n][testdata[0].size()-1]);
    }
    
    return CalculateAccuracy(givenlabels, predictedlabels);
}

void GetDistinctLabels(table &traindata, vector<string> &labels)
{
    for(int i =0; i<traindata.size(); ++i)
    {
        if(labels.empty())
            labels.push_back(traindata[i][traindata[0].size()-1]);
        else
        {
            bool notpresent = true;
            
            for(int k = 0; k < labels.size(); ++k)
            {
                if(traindata[i][traindata[0].size()-1] == labels[k])
                    notpresent = false;
            }
            if(notpresent)
                labels.push_back(traindata[i][traindata[0].size()-1]);
        }
    }
}

int main(int argc,char **argv)
{
    
    table testdata, traindata;
    vector<string> labels;
    vector<vector<double>> accuracy(5);
    double finalaccuracy;
    
    int Kvalue = 0;
    double sum = 0.0;
    double avg = 0.0;
    double min = -1;
    
    //6-fold cross validation
    for(int i = 6; i>=1; --i)
    {
        CreateAttributeTable(testdata, argv[i]);
        for(int j =1; j<= 6; ++j)
        {
            if(j != i)
                CreateAttributeTable(traindata,argv[j]);
            
        }
        GetDistinctLabels(traindata,labels);
        for(int k = 1; k<=5; ++k)
            accuracy[k-1].push_back(CalculateKNNAccuracy(traindata,testdata,labels,k));
        labels.clear();
        traindata.clear();
        testdata.clear();
    }
    ofstream outputfile;
    outputfile.open("KNNOutput.txt",ios::app);
    
    for(int i = 0; i<accuracy.size(); ++i)
    {
        sum = 0.0;
        for(int j = 0; j<accuracy[0].size();++j)
        {
            sum += accuracy[i][j];
        }
        avg = sum/((double)(accuracy[0].size()));
        outputfile<<endl<<"K value:"<<i+1<<"\t\t"<<avg<<endl;
        
        if(avg > min)
        {
            min = avg;
            Kvalue = i+1;
        }
    }
    
    outputfile<<"\n\n"<<"Final K value is:"<<Kvalue<<" with avg accuracy "<<min<<endl;
    
    //Final KNN run
    
    for(int i = 1; i<=6; ++i)
        CreateAttributeTable(traindata,argv[i]);
    
    CreateAttributeTable(testdata,argv[argc-1]);
    
    GetDistinctLabels(traindata,labels);
    
    finalaccuracy = CalculateKNNAccuracy(traindata,testdata,labels,Kvalue);
    
    outputfile<<"\n\nThe Final Accuracy using K="<<Kvalue<<" is "<<finalaccuracy<<"%"<<endl;
    
    outputfile.close();
    labels.clear();
    traindata.clear();
    testdata.clear();
    
    return 0;
}