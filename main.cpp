//
//  main.cpp
//  NeuralNetwork
//
//  Created by Jason Schray on 7/31/13.
//  Copyright (c) 2013 Jason Schray. All rights reserved.
//

#include "SimpleNetwork.h"
#include "SimpleNetworkTest.h"


int main(int argc, const char * argv[])
{
    testSimpleNetwork();
    
       return 0;
}

/*
int createWeight(SimpleNode * startNode, SimpleNode * endNode, double weight = ((double)rand())/RAND_MAX)
{
    
    SimpleNode ** temparcstart = (SimpleNode**)realloc(startNode->outNodes, (startNode->numOutWeights+1)*sizeof(SimpleNode*));
    if(temparcstart == NULL)
    {
        return 1;
    }
    startNode->outNodes= temparcstart;
    SimpleNode ** temparcend = (SimpleNode**)realloc(endNode->inNodes, (startNode->numInWeights+1)*sizeof(SimpleNode*));
    if(temparcend == NULL)
    {
        return 1;
    }
    double * tempweight = (double*)realloc(startNode->outWeights, (startNode->numOutWeights+1)*sizeof(double));
    if(tempweight == NULL)
        return 1;
    startNode->outWeights = tempweight;
    tempweight = (double*)realloc(endNode->inWeights, (endNode->numInWeights+1)*sizeof(double));
    if(tempweight == NULL)
        return 1;
    endNode->inWeights = tempweight;
    
    startNode->outWeights[startNode->numOutWeights] = weight;
    endNode->inWeights[endNode->numInWeights] = weight;
    startNode->numOutWeights++;
    startNode->numInWeights++;
    
    return 0;
}
*/
/*
int addNode(SimpleNode** nodeList, int* oldSize, ) {
    SimpleNode * newNodePtr = makeSimpleNode();
    realloc(nodeList, (*oldSize) + 1)
    nodeList[(*oldSize)] = newNodePtr;
    (*oldSize)++;
    return 0;
}*/




