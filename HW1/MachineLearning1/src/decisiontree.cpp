#include "decisiontree.hpp"

//node * searchposition(node * root)
//{
//	node * temp = NULL;
//	return temp;
//}

//void addnode()
//{
//	if(root == NULL)
//		root = (node*) malloc(sizeof(node));
//	else
//	{
//		node *pos = searchposition(root);
//	}
//}

//void builddecisiontree()
//{
//
//}

void createattributetable(char ***attritable, char *filename, int *fvicount)
{
	FILE * f = fopen(filename,"r");
	char *line = (char*)malloc(sizeof(char)*256);
	
	while(fgets(line,sizeof(line),f))
	{
        char **values = nullptr;
		int count = 0;
		while (strlen(line) != 0 && strchr(line,',') != NULL)
		{
			char *pos;
			pos = strchr(line,',');
			values[count] = (char*)malloc(sizeof(char) * (pos - line)+1);
			memcpy(values[count],line,sizeof(char) * (pos - line));
			values[count][pos-line] = '\0';
			line = pos+1;
			++count;

		}
		values[count] = (char*)malloc(sizeof(char) * strlen(line)+1);
		memcpy(values[count],line,sizeof(char) * strlen(line));
                values[count][strlen(line)] = '\0';
        attritable[*fvicount] = values;
		free(values);
        ++(*fvicount);
	}
}

//void printattrtable(char ***attrtable,int fvcount)
//{
//	for(int i = 0;i<=fvcount; ++i)
//	{
//		printf("%s\n",*attrtable[i]);
//	}
//}

int main(int argc, char*argv[])
{
    char ***attrtable = nullptr;
	int fvcount = 0;
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
                	for(int i = 2; i<= argc-1 ; ++i)
			{
				printf("%s\n",argv[i]);
				createattributetable(attrtable,argv[i], &fvcount);
			} 
            //printattrtable(attrtable,fvcount);
		}
	}
	return 0;
}
