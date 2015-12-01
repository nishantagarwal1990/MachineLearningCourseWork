
#ifndef DECISIONTREE_HPP
#define DECISIONTREE_HPP


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct node
{
	char feature[8];
	char **linkvalue;
	node * link;
	bool isleaf;
	char label[4];
};


//void addnode();				//adds a node to the tree
//void printdecisiontree();   //for debugging purposes only
//node* searchposition(node* root);
//void builddecisiontree(node * nodeptr);
//void destroy();				//destroy tree
void createattributetable(char ***attritable,char *filename, int *fvicount);

#endif
