#include "decisiontree.hpp"


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

void printtable(table &attrtable)
{
    for (int i = 0; i < attrtable.size(); i++)
    {
        for (int j = 0; j < attrtable[i].size(); j++)
        {
            printf("%s\t", attrtable[i][j].c_str());
        }
        printf("\n");
    }

}

bool TableIsEmpty(table &attrtable)
{
    return (attrtable.empty());
}

bool AllLabelsAreSame(table &attrtable)
{
    int labelcolumn = (int)attrtable[0].size() - 1;
    string firstlabel = attrtable[1][labelcolumn];
    for (int i = 1; i < attrtable.size(); i++) {
        if (firstlabel != attrtable[i][labelcolumn]) {
            return false;
        }
    }
    return true;
}

/*vector<string> GetDistinctLabels(table attrtable)
{
    vector<string> labels;
    int labelcolumn = (int)attrtable[0].size()-1;
    
    for(int i = 0; i < attrtable.size(); i++)
    {
        if(labels.empty())
            labels.push_back(attrtable[i][labelcolumn]);
        else
        {
            bool notpresent = false;
            
            for(int j=0; j<labels.size(); j++)
            {
                if(attrtable[i][labelcolumn].compare(labels[j]) == 0)
                    notpresent = false;
                else
                    notpresent = true;
            }
            if(notpresent)
                labels.push_back(attrtable[i][labelcolumn]);
            
        }
    }
    return labels;
}*/

/*vector<string> GetDistinctValues(table attrtable, int index)
{
    vector<string> values;
    for(int i = 0; i < attrtable.size(); i++)
    {
        if(values.empty())
            values.push_back(attrtable[i][index]);
        else
        {
            bool notpresent = false;
            
            for(int j=0; j<values.size(); j++)
            {
                if(attrtable[i][index].compare(values[j]) == 0)
                    notpresent = false;
                else
                    notpresent = true;
            }
            if(notpresent)
                values.push_back(attrtable[i][index]);
            
        }
    }
    return values;
}*/

double GetEntropyofEachValue(table &attrtable, vector<string> &labels, string value, int index)
{
    double entropy = 0.0;
    double p = 0.0 ;
    double n = 0.0;
    double total = 0.0;
    
    int labelcolumn = (int)attrtable[0].size()-1;
    
    for(int i = 1; i < attrtable.size(); i++)
    {
        if(attrtable[i][index] == value)
        {
            ++total;
            if(attrtable[i][labelcolumn] == labels[1])
                ++p;
            else
                ++n;
        }
    }
    if(total == 0)
        entropy = 0.0;
    else
    {
        double pratio = p/total;
        double nratio = n/total;
        double tratio = total/(attrtable.size()-1);
        if(pratio == 0 && nratio !=0)
            entropy = tratio * (((double)(-1)) * ((nratio)*(log2(nratio))));
        else if(nratio == 0 && pratio != 0)
            entropy = tratio * (((double)(-1)) * ((pratio)*(log2(pratio))));
        else if(pratio == 0 && nratio == 0)
            entropy = 0.0;
        else
            entropy = tratio * (((double)(-1)) * ((((pratio)*(log2(pratio)))) + ((nratio)*(log2(nratio)))));
    }
    return entropy;
}

int PredictBestAttribute(table &attrtable, table &attrinfotable)
{
    double minentropy = 999;      // minentropy means highest information gain
    int index = 0;;
    //vector<string>labels = GetDistinctLabels(attrtable);
    for(int i = 0; i<attrtable[0].size()-1;i++)
    {
        double totalentropyofattribute = 0.0;
        for(int j = 1; j < attrinfotable[i].size(); j++)
        {
            totalentropyofattribute += GetEntropyofEachValue(attrtable,attrinfotable[attrtable[0].size()-1], attrinfotable[i][j],i);
        }
        if(totalentropyofattribute < minentropy)
        {
            minentropy = totalentropyofattribute;
            index = i;
        }
    }
    return index;
}
table BuildSubsetTable(table &attrtable, int colindex, string value)
{
    table subsettable;
    vector<string> colnames;
    
    for(int i = 0; i< attrtable[0].size();i++)
    {
        if(i != colindex)
            colnames.push_back(attrtable[0][i]);
    }
    subsettable.push_back(colnames);
   
    for (int i = 1; i < attrtable.size(); i++) {
        vector<string> subsetrow;
        if (attrtable[i][colindex] == value)
        {
            for (int j = 0; j < attrtable[i].size(); j++)
            {
                if(j != colindex)
                {
                    subsetrow.push_back(attrtable[i][j]);
                }
            }
            subsettable.push_back(subsetrow);
            subsetrow.clear();
        }
    }
    return subsettable;
}

string MostCommonLabel(table &attrtable, table & attrinfotable)
{
    int p=0;
    int n=0;
    for(int i = 1; i< attrtable.size(); i++)
    {
        if(attrtable[attrtable[0].size()-1][i] == attrinfotable[attrtable[0].size()-1][1])
            ++p;
        else
            ++n;
    }
    if(p > n)
        return attrinfotable[attrtable[0].size()-1][1];
    else
        return attrinfotable[attrtable[0].size()-1][2];
}
node* BuildDecisionTree(table &attrtable, node* nodeptr, table &attrinfotable,int index )
{
    if(TableIsEmpty(attrtable))
        return NULL;
    
    if(AllLabelsAreSame(attrtable))
    {
        nodeptr->isleaf = true;
        nodeptr->label = attrtable[1][attrtable[0].size()-1];
        return nodeptr;
    }
    else
    {
        int predictedattributeindex = PredictBestAttribute(attrtable,attrinfotable);
        nodeptr->spliton = stoi(attrinfotable[predictedattributeindex][0]);
        for (int i = 1; i < attrinfotable[predictedattributeindex].size(); i++) {
            node* newNode = (node*) new node;
            newNode->label = attrinfotable[predictedattributeindex][i];
            nodeptr->values.push_back(attrinfotable[predictedattributeindex][i]);
            newNode->isleaf = false;
            table subsettable = BuildSubsetTable(attrtable, predictedattributeindex, attrinfotable[predictedattributeindex][i]);
            table subsetinfotable = GenerateAttributeInfoTable(subsettable);
            if(subsettable.empty())
            {
                newNode->isleaf = true;
                newNode->label = MostCommonLabel(attrtable,attrinfotable);
                nodeptr->links.push_back(newNode);
            }
            else
            nodeptr->links.push_back(BuildDecisionTree(subsettable, newNode,subsetinfotable, predictedattributeindex));
        }
    }
    
    return nodeptr;
}

table GenerateAttributeInfoTable(table &attrtable)
{
    table tableinfo;
    vector<string> values;
    for(int i = 0; i < attrtable[0].size(); i++)
    {
        for(int j = 0; j < attrtable.size(); j++)
        {
            if(values.empty())
                values.push_back(attrtable[j][i]);
            else
            {
                bool notpresent = true;
            
                for(int k = 0; k < values.size(); k++)
                {
                    if(attrtable[j][i] == values[k])
                        notpresent = false;
                }
                if(notpresent)
                    values.push_back(attrtable[j][i]);
            }
        }
        tableinfo.push_back(values);
        values.clear();
    }
    return tableinfo;
}

void PrintDecisionTree(node* nodePtr)
{
    if(nodePtr == NULL) {
        return;
    }
    if (!nodePtr->links.empty()) {
        cout << "SplitOn: "<< nodePtr->spliton<<endl;
        for (int i = 0; i < nodePtr->links.size(); i++) {
            cout << "Link Name: " << nodePtr->values[i] << "\t"<<endl;
            PrintDecisionTree(nodePtr->links[i]);
        }
        return;
    } else {
        cout << "Label = " << nodePtr->label <<endl;
        return;
    }
}

/*string TestDataOnDecisionTree(vs &singleLine, node* nodePtr, vvs &tableInfo)
{
    string prediction;
    while (!nodePtr->isLeaf && !nodePtr->children.empty()) {
        int index = returnColumnIndex(nodePtr->splitOn, tableInfo);
        string value = singleLine[index];
        int childIndex = returnIndexOfVector(nodePtr->childrenValues, value);
        nodePtr = nodePtr->children[childIndex];
        if (nodePtr == NULL) {
            prediction = defaultClass;
            break;
        }
        prediction = nodePtr->label;
    }
    return prediction;
}*/

int IndexofLink(vector<string> &values, string value)
{
    for (int i = 0; i < values.size(); i++) {
        if (values[i] == value)	{
            return i;
        }
    }
    return -1;
}

string TestDecisionTree(vector<string> &testline, node* nodeptr)
{
    string prediction;
    while (!nodeptr->isleaf && !nodeptr->links.empty()) {
        string value = testline[nodeptr->spliton - 1];
        int childIndex = IndexofLink(nodeptr->values, value);
        nodeptr = nodeptr->links[childIndex];
    }
    prediction = nodeptr->label;
    return prediction;
}

double PrintPredictionsAndAccuracy(vector<string> &givenlabels, vector<string> &predictedlabels, int traindatasize, int testdatasize)
{
    ofstream outputFile;
    outputFile.open("DecisionTreeOutput.txt");
    double correct = 0;
    outputFile << setw(3) << "#" << setw(16) << "Given Class" << setw(31) << right << "Predicted Class" << endl;
    outputFile << "--------------------------------------------------" << endl;
    for (int i = 0; i < givenlabels.size(); i++) {
        outputFile << setw(3) << i+1 << setw(16) << givenlabels[i];
        if (givenlabels[i] == predictedlabels[i]) {
            correct++;
            outputFile << "  ------------  ";
        } else {
            outputFile << "  xxxxxxxxxxxx  ";
        }
        outputFile << predictedlabels[i] << endl;
    }
    outputFile << "--------------------------------------------------" << endl;
    outputFile << "Total number of instances in test data = " << givenlabels.size() << endl;
    outputFile << "Number of correctly predicted instances = " << correct << endl;
    outputFile.close();
    return (double) (correct/(double)(testdatasize))* 100;
}


int main(int argc, char*argv[])
{
	table attributetable;
    table attributeinfotable;
    int traindatasize = 0;
    int testdatasize = 0;
    node* root;
    int index = -1;
	printf("test\n");
	if(argc == 1)
		printf("Enter appropriate arguments\n");
	else
	{
		//if(argc == 2)
		//	printf("Please enter filename in arguments.\n");
		//else
		//{
           // if(strcmp(argv[1],"1") == 0)
           // {
                vector<string> colnames = {"1","2","3","4","5","6","7","8","9","Labels"};
                attributetable.push_back(colnames);
                for(int i = 1; i < argc-1 ; ++i)
                {
                    printf("%s\n",argv[i]);
                    CreateAttributeTable(attributetable,argv[i]);
                    printtable(attributetable);                             //for debugging purposes
                }
               attributeinfotable = GenerateAttributeInfoTable(attributetable);
               //printtable(attributeinfotable);
               root = new node;
               root = BuildDecisionTree(attributetable, root , attributeinfotable, index);
               //PrintDecisionTree(root);
          //  }
        //traindatasize = (int)attributetable.size()-1;
        attributetable.clear();
        attributeinfotable.empty();
        attributetable.push_back(colnames);
        CreateAttributeTable(attributetable,argv[argc-1]);
        //printtable(attributetable);
        vector<string> predictedlabels;
        vector<string> givenlabels;
        
        for (int i = 1; i < attributetable.size(); i++)
        {
            string data = attributetable[i][attributetable[0].size()-1];
            givenlabels.push_back(data);
        }
        for (int i = 1; i < attributetable.size(); i++)
        {
            string label = TestDecisionTree(attributetable[i], root);
            predictedlabels.push_back(label);
        }
        testdatasize = (int)attributetable.size()-1;
        attributetable.clear();
        
        ofstream outputFile;
        outputFile.open("DecisionTreeOutput.txt", ios::app);
        outputFile << endl << "--------------------------------------------------" << endl;
        double accuracy = PrintPredictionsAndAccuracy(givenlabels, predictedlabels, traindatasize, testdatasize);
        outputFile << "Accuracy of decision tree classifier = " << accuracy << "%";

		//}
	}
	return 0;
}
