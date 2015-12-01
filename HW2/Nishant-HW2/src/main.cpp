//
//  main.cpp
//  SimplePerceptron
//
//  Created by Nishant Agarwal on 9/25/15.
//  Copyright © 2015 Nishant Agarwal. All rights reserved.
//

#include <iostream>
#include <vector>
#include <random>
#include <cmath>
#include <fstream>
#include <algorithm>
using namespace std;
typedef vector<vector<double>> table;

void printweights(vector<double> &weights, ofstream &output)
{
    for(int i =0; i<weights.size();++i)
        output<<weights[i]<<"\t";
    output<<endl;
}

int SimplePerceptron(double rate, vector<double> &weights, table &trainset, bool test)
{
    int mistake = 0;
    double sum = 0.0;
    //bool y,y1;
    
    for(int i =0; i<trainset.size();++i)
    {
        sum = 0.0;
        for (int j=1;j<trainset[0].size();++j)
        {
            sum += weights[j-1]*trainset[i][j];
        }
        //sum=ceil(sum);
        //cout<<sum<<endl;
        sum *= trainset[i][0];
        //y1 = (sum <= 0);
        if(sum<=0)//y != y1)
        {
            if(!test)
            {
                for(int k = 0; k<weights.size();++k)
                {
                    if(trainset[i][0] <= 0)
                        weights[k] = weights[k]-(rate*trainset[i][k+1]);
                    else
                        weights[k] = weights[k]+(rate*trainset[i][k+1]);
                }
            }
            ++mistake;
            //printweights(weights);
        }
            
    }
    return mistake;
}

void Randomiz(vector<double> &weights, int iter)//,vector<double> &rate , vector<double> &margin, bool w, bool r, bool m)
{
    int i;
    double randval = 0.0;
    //random_device rd; // obtain a random number from hardware
    //mt19937 eng(rd()); // seed the generator
    //if (w){
    if(!weights.empty())
    {
        weights.clear();
        //vector<double>().swap(weights);
    }
    randval = (double)((rand()%100)/((double)100));
    if(rand()%2  == 0)
        randval = (-1)*randval;
    //uniform_real_distribution<double> distw(-1, 1);
    //randval = distw(eng);
    for(i =0;i<iter+1;++i)
    {
        weights.push_back(randval);//distw(eng));
    }
   // }
    /*
    if(r)
    {
        rate.clear();
        uniform_real_distribution<double> distr(0,1.0000001);
        for(i =0;i<10;++i)
        {
            rate.push_back(distr(eng));
        }
    }
    if(m)
    {
        margin.clear();
        uniform_real_distribution<double> distm(0,5.00000001);
        for(i =0;i<10;++i)
        {
            margin.push_back(distm(eng));
        }
    }*/
    
}

int MarginPerceptron(double rate, double margin,vector<double> &weights, table &trainset, bool test)
{
    int mistake = 0;
    double sum = 0.0;
    
    for(int i =0; i<trainset.size();++i)
    {
        sum = 0.0;
        for (int j=1;j<trainset[0].size();++j)
        {
            sum += weights[j-1]*trainset[i][j];
        }
        sum *= trainset[i][0];
        if(sum <= margin)
        {
            if(!test)
            {
                for(int k = 0; k<weights.size();++k)
                {
                    if(trainset[i][0] <= 0)
                        weights[k] = weights[k]-(rate*trainset[i][k+1]);
                    else
                        weights[k] = weights[k]+(rate*trainset[i][k+1]);
                }
            }
            ++mistake;
            //printweights(weights);
        }
        
    }
    return mistake;
}

int AggressivePerceptron(double margin,vector<double> &weights, table &trainset,bool test)
{
    int mistake = 0;
    double sumw = 0.0;
    double sumf = 0.0;
    double rate = 0.0;
    
    for(int i =0; i<trainset.size();++i)
    {
        sumw = 0.0;
        sumf = 0.0;
        for (int j=1;j<trainset[0].size();++j)
        {
            sumw += weights[j-1]*trainset[i][j];
            sumf += trainset[i][j]*trainset[i][j];
        }
        sumw *= trainset[i][0];
        if(sumw <= margin)
        {
            rate = ((margin - sumw)/(sumf+1));
            //cout<< rate<<endl;
            for(int k = 0; k<weights.size();++k)
            {
                if(trainset[i][0] <= 0)
                    weights[k] = weights[k]-(rate*trainset[i][k+1]);
                else
                    weights[k] = weights[k]+(rate*trainset[i][k+1]);
            }
            ++mistake;
            //printweights(weights);
        }
        
    }
    return mistake;
}
void generatetable(table &trainset, string filename, int iter=0)
{
    ifstream file;
    file.open(filename);
    
    string line;
    size_t colpos;
    string index;
    while(getline(file,line))
    {
        vector<double> columnvalues(iter+2);
        size_t sz;
        while (line.length() !=0 && line.find(' ') != string::npos)
        {
            size_t position;
            string value;
           
            
            position = line.find_first_of(' ');
            value = line.substr(0, position);
            colpos = value.find_first_of(':');
            if(colpos == string::npos)
            {
                columnvalues[0]= stod(value,&sz);
            }
            else
            {
                index = value.substr(0, colpos);
                value.erase(0,colpos+1);
                columnvalues[stoi(index,&sz)+1]= stod(value,&sz);
            }
            line.erase(0, position+1);
        }
        colpos = line.find_first_of(':');
        index = line.substr(0, colpos);
        line.erase(0,colpos+1);
        columnvalues[stoi(index,&sz)+1]= stod(line,&sz);
        trainset.push_back(columnvalues);
        columnvalues.clear();
        
    }
    file.close();
}

void copytable(table &traindata, table &trainset, table &testdata,int iter)
{
    for(int i = 0; i<trainset.size();++i)
    {
        if((i>=(100*iter)) && i<(100*(iter+1)))
        {
            testdata.push_back(trainset[i]);
        }
        else
            traindata.push_back(trainset[i]);
    }
}

int main(int argc, const char * argv[]) {
    
    int i,j,k;
    vector<string> folders;
    string n;
    string filepath,trainfilename,testfilename;
    vector<double> weights;
    vector<double> initweights;
    vector<double> rate = {0.1,0.001,0.2,0.0001,0.3,1.0,0.4,0.6,0.05,0.5};
    vector<double> margin = { 0.1, 1.0,0.25,3.5,4.0,0.05, 0.2, 0.4, 0.3, 0.5};
    vector<vector<double>> avgacc(10,vector<double>(10,0.0));
    vector<double> avgaccsimple(10,0.0);
    double maxacc = -1.0;
    double maxaccsimple = -1.0;
    double acc = 0.0;
    double accsimple = 0.0;
    table trainset2;
    table subsettrain;
    table subsettest;
    table testdata;
    int select = 0;;
    double bestrate = 0.0;
    double bestratesimple = 0.0;
    double bestmargin = 0.0;
    int mistake;
    int minmistake = 999;
    table trainset1 = {{0,1,0,1,1,0},
        {0, 1,1, 1, 1, 0},
        {1, 1,0, 1, 1, 1},
        {1, 1,1, 0, 1, 1},
        {0, 1,0, 1, 1, 0},
        {1, 1,1, 1, 0, 1}};
    ofstream outputfile;
    while(select != 5)
    {
        cout<<"Select any one of the following:"<<endl;
        cout<<"1.Simple Perceptron with Table 2 data"<<endl;
        cout<<"2.Simple and Margin Perceptron with a single trainset and testset"<<endl;
        cout<<"3.Batch Perceptron"<<endl;
        cout<<"4.Aggressive Perceptron with margin"<<endl;
        cout<<"5.Exit"<<endl;
        cin >> select;
        
        if (select == 1)
        {
            outputfile.open("output1.txt");
            //test for tuning rate
            Randomiz(weights,4);
            initweights = weights;
            for ( i =1; i<=10; i++)
            {
                mistake = SimplePerceptron(rate[i-1],weights,trainset1,false);
                if(mistake<minmistake){
                    minmistake = mistake;
                    bestrate = rate[i-1];
                }
                weights = initweights;
            }
            mistake = SimplePerceptron( bestrate, weights, trainset1,false);
            outputfile << " Experiment 1"<<endl;
            outputfile << " Weights: "<<endl;
            printweights(weights,outputfile);
            outputfile<<"Mistakes: "<<mistake<<"\n\n\n\n\n"<<endl;
            mistake = SimplePerceptron( bestrate, weights, trainset1,true);
            outputfile.close();
            
        }
        
        if(select == 2)
        {
            outputfile.open("output2.txt");
            outputfile<<"Experiment 2"<<endl;
            generatetable(trainset2, "data0/train0.10",10);
            Randomiz(weights,10);
            initweights = weights;
            for(k=0;k<=9;++k)
            {
                //Randomiz(weights,10);
                copytable(subsettrain,trainset2,subsettest,k);
                for(i = 0; i<10; ++i)
                {
                    for(j=0; j<10; ++j)
                    {
                        MarginPerceptron(rate[i], margin[j], weights, subsettrain,false);
                        mistake = MarginPerceptron(rate[i], margin[j], weights, subsettest, true);
                        avgacc[i][j]+=(double)((double)(subsettest.size()-mistake)/subsettest.size());
                        weights = initweights;
                    }
                    SimplePerceptron(rate[i], weights, subsettrain, false);
                    mistake = SimplePerceptron(rate[i], weights, subsettest, true);
                    avgaccsimple[i]+=(double)((double)(subsettest.size()- mistake)/subsettest.size());
                    weights = initweights;
                }
                subsettrain.clear();
                subsettest.clear();
                vector<vector<double>>().swap(subsettest);
                vector<vector<double>>().swap(subsettrain);
            }
            
            for(i=0;i<10;++i)
            {
                for(j = 0 ; j<10; ++j)
                {
                    acc = avgacc[i][j]/10.0;
                    if(acc >= maxacc)
                    {
                        maxacc = acc;
                        bestrate = rate[i];
                        bestmargin = margin[j];
                    }
                }
                accsimple = avgaccsimple[i]/10.0;
                if(accsimple > maxaccsimple)
                    bestratesimple = rate[i];
            }
            generatetable(testdata, "data0/test0.10",10);
            mistake = SimplePerceptron(bestratesimple, weights, trainset2, false);
            outputfile<<"Simple Perceptron Weights:"<<endl;
            printweights(weights,outputfile);
            outputfile<<"Updates made by Simple Perceptron: "<<mistake<<endl;
            mistake = SimplePerceptron(bestratesimple, weights, trainset2, true);
            acc = (double)((double)(trainset2.size()-mistake)/trainset2.size());
            outputfile<<"Simple Perceptron Accuracy(On training data): "<<acc*100<<endl;
            mistake = SimplePerceptron(bestratesimple, weights, testdata, true);
            acc = (double)((double)(testdata.size()-mistake)/testdata.size());
            outputfile<<"Simple Perceptron Accuracy(On test data): "<<acc*100<<endl;
            weights = initweights;
            mistake = MarginPerceptron(bestrate, bestmargin, weights, trainset2, false);
            outputfile<<"Margin Perceptron Weights:"<<endl;
            printweights(weights,outputfile);
            outputfile<<"Updates made by Margin Perceptron: "<<mistake<<endl;
            mistake = MarginPerceptron(bestrate, bestmargin, weights, trainset2, true);
            acc = (double)((double)(trainset2.size()-mistake)/trainset2.size());
            outputfile<<"Margin Perceptron Accuracy (On training data): "<<acc*100<<endl;
            mistake = MarginPerceptron(bestrate, bestmargin, weights, testdata, true);
            acc = (double)((double)(testdata.size()-mistake)/testdata.size());
            outputfile<<"Margin Perceptron Accuracy (On test data): "<<acc*100<<endl;
            avgaccsimple.clear();
            avgaccsimple.resize(10,0.0);
            for(int g = 0 ; g<avgacc.size();++g)
            {
                avgacc[g].clear();
                avgacc[g].resize(10,0.0);
            }
            trainset2.clear();
            testdata.clear();
            subsettest.clear();
            subsettrain.clear();
            initweights.clear();
            outputfile<<"\n\n\n\n\n"<<endl;
            outputfile.close();
        }
        
        if(select == 3)
        {
            outputfile.open("output3.txt");
            outputfile<<"Experiment 3"<<endl;
            folders = {"data0","data1"};
            for(int z = 0; z < folders.size(); ++z)
            {
                if(folders[z] == "data0")
                    filepath = "data0/";
                if(folders[z] == "data1")
                    filepath = "data1/";
                for(int y = 10; y<= 100; y+=10)
                {
                    if(filepath == "data0/")
                    {
                        trainfilename = filepath+"train0."+to_string(y);
                        testfilename = filepath+"test0."+to_string(y);
                    }
                    else
                    {
                        trainfilename = filepath+"train1."+to_string(y);
                        testfilename = filepath+"test1."+to_string(y);
                    }
                    
                    generatetable(trainset2, trainfilename, y);
                    
                    Randomiz(weights,y);
                    initweights = weights;
                    for(k=0;k<=9;++k)
                    {
                        copytable(subsettrain,trainset2,subsettest,k);
                        
                        for(i = 0; i<10; ++i)
                        {
                            for(j=0; j<10; ++j)
                            {
                                MarginPerceptron(rate[i], margin[j], weights, subsettrain,false);
                                mistake = MarginPerceptron(rate[i], margin[j], weights, subsettest, true);
                                avgacc[i][j]+=(double)((double)(subsettest.size()-mistake)/subsettest.size());
                                weights = initweights;
                            }
                            SimplePerceptron(rate[i], weights, subsettrain, false);
                            mistake = SimplePerceptron(rate[i], weights, subsettest, true);
                            avgaccsimple[i]+=(double)((double)(subsettest.size()- mistake)/subsettest.size());
                            weights = initweights;
                        }
                        subsettrain.clear();
                        subsettest.clear();
                    }
            
                    for(i=0;i<10;++i)
                    {
                        for(j = 0 ; j<10; ++j)
                        {
                            acc = avgacc[i][j]/10.0;
                            if(acc>=maxacc)
                            {
                                maxacc = acc;
                                bestrate = rate[i];
                                bestmargin = margin[j];
                            }
                        }
                        accsimple = avgaccsimple[i]/10.0;
                        if(accsimple > maxaccsimple)
                            bestratesimple = rate[i];
                    }
                    
                    generatetable(testdata, testfilename,y);
                    outputfile<<"\n\nExperiment for "<<trainfilename<<" "<<testfilename<<"\n\n\n"<<endl;
                    double accs = 0.0;
                    acc = 0.0;
                    int mistakes = 0;
                    int totmistakes = 0;
                    int totmistake = 0;
                    vector<double> weightsimple = initweights;
                    vector<double> weightmargin = initweights;
                    for(int x = 0; x<10; ++x)
                    {
                        random_shuffle(trainset2.begin(),trainset2.end());
                        totmistakes += SimplePerceptron(bestratesimple, weightsimple, trainset2, false);
                        totmistake += MarginPerceptron(bestrate, bestmargin, weightmargin, trainset2, false);

                        //random_shuffle(trainset2.begin(),trainset2.end());
                    }
                    
                    outputfile<<"Simple Perceptron Mistakes: "<<totmistakes<<endl;
                    mistakes = SimplePerceptron(bestratesimple, weightsimple, testdata, true);
                    accs = (double)((double)(testdata.size()-mistakes)/testdata.size());
                    outputfile<<"Simple Perceptron Accuracy: "<<accs*100<<endl;
                    outputfile<<"Margin Perceptron Mistakes: "<<totmistake<<endl;
                    mistake = MarginPerceptron(bestrate, bestmargin, weightmargin, testdata, true);
                    acc = (double)((double)(testdata.size()-mistake)/testdata.size());
                    outputfile<<"Margin Perceptron Accuracy: "<<acc*100<<endl;
                    avgaccsimple.clear();
                    avgaccsimple.resize(10,0.0);
                    for(int g = 0 ; g<avgacc.size();++g)
                    {
                        avgacc[g].clear();
                        avgacc[g].resize(10,0.0);
                    }
                    trainset2.clear();
                    testdata.clear();
                    subsettest.clear();
                    subsettrain.clear();
                    weightmargin.clear();
                    weightsimple.clear();
                    initweights.clear();
                }
            }
            outputfile<<"\n\n\n\n\n"<<endl;
            outputfile.close();
        }
        
        if(select == 4)
        {
            outputfile.open("output4.txt");
            outputfile<<"Experiment 4"<<endl;
            folders = {"data0","data1"};
            for(int z = 0; z < folders.size(); ++z)
            {
                if(folders[z] == "data0")
                    filepath = "data0/";
                if(folders[z] == "data1")
                    filepath = "data1/";
                for(int y = 10; y<= 100; y+=10)
                {
                    if(filepath == "data0/")
                    {
                        trainfilename = filepath+"train0."+to_string(y);
                        testfilename = filepath+"test0."+to_string(y);
                    }
                    else
                    {
                        trainfilename = filepath+"train1."+to_string(y);
                        testfilename = filepath+"test1."+to_string(y);
                    }
                    
                    generatetable(trainset2, trainfilename, y);
                    
                    Randomiz(weights,y);
                    initweights = weights;
                    for(k=0;k<=9;++k)
                    {
                        copytable(subsettrain,trainset2,subsettest,k);
                        
                        for(i = 0; i<10; ++i)
                        {
                            AggressivePerceptron(margin[i], weights, subsettrain, false);
                            mistake = AggressivePerceptron( margin[i], weights, subsettest, true);
                            avgaccsimple[i]+=(double)((double)(subsettest.size()-mistake)/subsettest.size());
                            weights = initweights;
                        }
                        subsettrain.clear();
                        subsettest.clear();
                    }
                    
                    for(i=0;i<10;++i)
                    {
                        accsimple = avgaccsimple[i]/10.0;
                        if(accsimple > maxaccsimple)
                            bestmargin = margin[i];
                    }
                    generatetable(testdata, testfilename,y);
                    outputfile<<"\n\nExperiment for "<<trainfilename<<" "<<testfilename<<"\n\n\n"<<endl;
                    double accs = 0.0;
                    acc = 0.0;
                    int mistakes = 0;
                    int totmistakes = 0;
                    weights = initweights;
                    for(int x = 0; x<10; ++x)
                    {
                        random_shuffle(trainset2.begin(),trainset2.end());
                        totmistakes += AggressivePerceptron(bestmargin, weights, trainset2, false);
                        //random_shuffle(trainset2.begin(),trainset2.end());
                    }
                    //mistakes = AggressivePerceptron(bestmargin, weights, trainset2, true);
                    //acc = (double)((double)(trainset2.size()-mistakes)/trainset2.size());
                    //cout<<acc*100<<endl;
                    mistakes = AggressivePerceptron(bestmargin, weights, testdata, true);
                    accs = (double)((double)(testdata.size()-mistakes)/testdata.size());
                    outputfile<<"Aggressive Perceptron Mistakes: "<<totmistakes<<endl;
                    outputfile<<"Aggressive Perceptron Accuracy: "<<accs*100<<endl;
                    avgaccsimple.clear();
                    avgaccsimple.resize(10,0.0);
                    for(int g = 0 ; g<avgacc.size();++g)
                    {
                        avgacc[g].clear();
                        avgacc[g].resize(10,0.0);
                    }
                    trainset2.clear();
                    testdata.clear();
                    subsettest.clear();
                    subsettrain.clear();
                    initweights.clear();
                }
            }
            outputfile<<"\n\n\n\n\n"<<endl;
            outputfile.close();
        }
    }
    outputfile.close();
    return 0;
}
