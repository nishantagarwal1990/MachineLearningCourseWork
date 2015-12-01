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
    string firstlabel = attrtable[0][labelcolumn];
    for (int i = 0i; i < attrtable.size(); i++) {
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
    double entropy = 0;
    int p = 0 ;
    int n = 0;
    int total = 0;
    
    int labelcolumn = (int)attrtable[0].size()-1;
    
    for(int i = 0; i < attrtable.size(); i++)
    {
        if(attrtable[i][index].compare(value) == 0)
        {
            ++total;
            if(attrtable[i][labelcolumn].compare(labels[0]) == 0)
                ++p;
            else
                ++n;
        }
    }
    if(total == 0)
        entropy = 0;
    else
        entropy = (-(p/total)*log2((p/total)) - (n/total)*log2((n/total)));
    return entropy;
}

int PredictBestAttribute(table &attrtable, table &attrinfotable)
{
    double minentropy = -1;      // minentropy means highest information gain
    int index = 0;;
    //vector<string>labels = GetDistinctLabels(attrtable);
    for(int i = 0; i<attrtable[0].size();i++)
    {
        double totalentropyofattribute = 0;
        for(int j = 0; j < attrinfotable[i].size(); j++)
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
   
    for (int i = 0; i < attrtable.size(); i++) {
        vector<string> subsetrow;
        if (attrtable[i][colindex].compare(value) == 0)
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

node* BuildDecisionTree(table &attrtable, node* nodeptr, table &attrinfotable)
{
    if(TableIsEmpty(attrtable))
        return NULL;
    
    if(AllLabelsAreSame(attrtable))
    {
        nodeptr->isleaf = true;
        nodeptr->label = attrtable[0][attrtable[0].size()-1];
        return nodeptr;
    }
    else
    {
        int predictedattributeindex = PredictBestAttribute(attrtable,attrinfotable);
        for (int i = 0; i < attrinfotable[predictedattributeindex].size(); i++) {
            node* newNode = (node*) new node;
            newNode->label = attrinfotable[predictedattributeindex][i];
            nodeptr->values.push_back(attrinfotable[predictedattributeindex][i]);
            newNode->isleaf = false;
            table subsettable = BuildSubsetTable(attrtable, predictedattributeindex, attrinfotable[predictedattributeindex][i]);
            nodeptr->links.push_back(BuildDecisionTree(subsettable, newNode,attrinfotable));
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
        cout << " Value: " << nodePtr->label << endl;
        for (int i = 0; i < nodePtr->links.size(); i++) {
            cout << "\t";
            PrintDecisionTree(nodePtr->links[i]);
        }
        return;
    } else {
        cout << "Label = " << nodePtr->label;
        return;
    }
}

int main(int argc, char*argv[])
{
	table attributetable;
    table attributeinfotable;
    node* root;
	printf("test\n");
	if(argc == 1)
		printf("Enter appropriate arguments\n");
	else
	{
		if(argc == 2)
			printf("Please enter filename in arguments.\n");
		else
		{
            if(strcmp(argv[1],"1") == 0)
            {
                for(int i = 2; i <= argc-1 ; ++i)
                {
                    printf("%s\n",argv[i]);
                    CreateAttributeTable(attributetable,argv[i]);
                    printtable(attributetable);                             //for debugging purposes
                }
                attributeinfotable = GenerateAttributeInfoTable(attributetable);
                printtable(attributeinfotable);
                root = new node;
                root = BuildDecisionTree(attributetable, root , attributeinfotable);
                PrintDecisionTree(root);
            }
		}
	}
	return 0;
}
