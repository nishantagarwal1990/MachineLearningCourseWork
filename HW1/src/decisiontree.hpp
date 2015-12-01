
#ifndef DECISIONTREE_HPP
#define DECISIONTREE_HPP


#include <iostream>
#include <stdio.h>
#include <fstream>
#include <string>
#include <vector>
#include <math.h>
using namespace std;

typedef vector<vector<string> > table;

struct node
{
	string attributename;         //attribute name
	vector<string>values;         //name of links for decision
	vector<node*> links;           //pointer of links
	bool isleaf;                  //boolean to check if node is leaf
	string label;                 //if leaf then what is the label of the node
};



node* BuildDecisionTree(table &attrtable, node * nodeptr, table &attrinfotable);
bool TableIsEmpty(table &attrtable);
bool AllLabelsAreSame(table &attrtable);
void CreateAttributeTable(table &attrtable,string filename);
int PredictBestAttribute(table &attrtable);
double GetEntropyofEachValue(table &attrtable, vector<string> &labels, string value, int index);
table GenerateAttributeInfoTable(table &attrtable);

//for debugging
void printtable(table &attrtable);
#endif
