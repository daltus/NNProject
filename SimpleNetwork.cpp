//
//  SimpleNetwork.cpp
//  NeuralNetworkV3
//
//  Created by Jason Schray on 8/7/13.
//  Copyright (c) 2013 Jason Schray. All rights reserved.
//

#include "SimpleNetwork.h"


SimpleNode* makeSimpleNode(NodeType ntype = INPUT, double biasvalue = ((double)rand())/RAND_MAX) {
    SimpleNode* nptr = (SimpleNode*) malloc(sizeof(SimpleNode));
    if(nptr == NULL) {
        printf("couldn't allocate a SimpleNode");
        exit(1);
    }
    nptr->nodeType = ntype;
    nptr->bias = biasvalue;
    nptr->curValue = 0;
    nptr->numOutEdges = 0;
    nptr->numInEdges = 0;
    nptr->inEdges = NULL;
    nptr->outEdges = NULL;
    
    return nptr;
}

SimpleEdge* makeSimpleEdge(SimpleNode* s = NULL, SimpleNode* e = NULL, double w = (((double)rand())/RAND_MAX * WEIGHT_CONSTANT) - WEIGHT_CONSTANT/2){
    SimpleEdge* eptr = (SimpleEdge*) malloc(sizeof(SimpleEdge));
    if(eptr == NULL) {
        printf("couldn't allocate a SimpleEdge");
        exit(1);
    }
    eptr->startNode = s;
    eptr->endNode = e;
    eptr->simpleWeight = w;
    eptr->nextWeight = w;
    
    return eptr;
}

SimpleNetwork* makeSimpleNetwork() {
    SimpleNetwork* ptr = (SimpleNetwork*) malloc(sizeof(SimpleNetwork));
    if(ptr == NULL) {
        printf("couldn't allocate a SimpleNetwork");
        exit(1);
    }
    //TODO we should make function for this error handler
    ptr->netWorkSize = 0;
    ptr->nodeList = NULL;
    ptr->inputSize = 0;
    ptr->inputList = NULL;
    ptr->hiddenSize = 0;
    ptr->hiddenList = NULL;
    ptr->outputSize = 0;
    ptr->outputList = NULL;
    
    return ptr;
}

int addEdgeToNodes(SimpleNode* start, SimpleNode* end, SimpleEdge* edge) {
    //Check for memory necessary for adding the edge to both nodes
    SimpleEdge ** tempOutEdgeList = (SimpleEdge**)realloc(start->outEdges, sizeof(SimpleEdge*)*(start->numOutEdges+1));
    if(tempOutEdgeList == NULL)
    {
        printf("couldn't allocate room for a new edge in a start node.");
        exit(1);
    }
    SimpleEdge ** tempInEdgeList = (SimpleEdge**)realloc(end->inEdges, sizeof(SimpleEdge*)*(end->numInEdges+1));
    if(tempInEdgeList == NULL)
    {
        printf("couldn't allocate room for a new edge in a end node.");
        exit(1);
    }
    
    // assign the memory to the nodes
    start->outEdges = tempOutEdgeList;
    end->inEdges = tempInEdgeList;
    
    //add edge to start node and update edge list length
    start->outEdges[start->numOutEdges] = edge;
    start->numOutEdges++;
    
    //add edge to end node and update edge list length
    end->inEdges[end->numInEdges] = edge;
    end->numInEdges++;
    
    
    return 0;
}

/**
 * createSimpleNetwork
 *
 * all hidden nodes in one layer, i.e. only three layers total: in, hidden, out
 * exhaustively creates edges between all nodes in adjacent layers with random weight values
 */
int createSimpleNetwork(SimpleNetwork* nn, int inputNodes, int hiddenNodes, int outputNodes){
    nn->nodeList = (SimpleNode**) malloc(sizeof(SimpleNode*)*(inputNodes + hiddenNodes + outputNodes));
    nn->netWorkSize = inputNodes + hiddenNodes + outputNodes;
    
    //TODO - make these for loops more readable by doing one for loop with case
    int i;
    for(i = 0; i < inputNodes; i++)
    {
        nn->nodeList[i] = makeSimpleNode(INPUT);
    }
    
    for (int j = 0; j < hiddenNodes; j++, i++)
    {
        nn->nodeList[i] = makeSimpleNode(HIDDEN);
    }
    
    for(int j = 0; j < outputNodes; j++, i++)
    {
        nn->nodeList[i] = makeSimpleNode(OUTPUT);
    }
    
    //decompose these into functions
    //ASSUMES nodes put into list in order
    nn->inputSize = inputNodes;
    nn->inputList = &(nn->nodeList[0]);
    
    nn->hiddenSize = hiddenNodes;
    nn->hiddenList = &(nn->nodeList[inputNodes]);
    
    nn->outputSize = outputNodes;
    nn->outputList = &(nn->nodeList[hiddenNodes+inputNodes]);
    
    //adding edges
    for(i = 0; i < nn->inputSize; i++) {
        for(int j = 0; j < nn->hiddenSize; j++) {
            addEdgeToNodes(nn->inputList[i], nn->hiddenList[j], makeSimpleEdge(nn->inputList[i],nn->hiddenList[j]));
        }
    }
    for(i = 0; i < nn->hiddenSize; i++) {
        for(int j = 0; j < nn->outputSize; j++) {
            addEdgeToNodes(nn->hiddenList[i], nn->outputList[j], makeSimpleEdge(nn->hiddenList[i],nn->outputList[j]));
        }
    }
    
    return 0;
}

/*
 * This function updates the curvalues in the network. Note this functions devines the output of input nodes as input values
 * into the network.
 */
int updateSimpleNetworkCurVals(SimpleNetwork * nn, double * inputs, int numInputs){
    
    if(nn->inputSize != numInputs)
    {
        printf("The number of inputs didn't match the number of input nodes of the network.");
        exit(1);
    }
    
    int i;
    for(i = 0; i  < nn->inputSize; i++)
    {
        nn->inputList[i]->curValue = inputs[i];
    }
    
    int j;
    double tempcurval;
    for(i = 0; i < nn->hiddenSize; i  ++)
    {
        tempcurval = 0;
        for(j = 0; j < nn->hiddenList[i]->numInEdges; j++)
        {
            
            tempcurval += nn->hiddenList[i]->inEdges[j]->simpleWeight*(nn->hiddenList[i]->inEdges[j]->startNode->curValue);
        }
        
        nn->hiddenList[i]->curValue = tempcurval + nn->hiddenList[i]->bias;
    }
    
    for(i = 0; i < nn->outputSize; i  ++)
    {
        tempcurval = 0;
        for(j = 0; j < nn->outputList[i]->numInEdges; j++)
        {
            
            tempcurval += nn->outputList[i]->inEdges[j]->simpleWeight*hardSigmoid(nn->outputList[i]->inEdges[j]->startNode->curValue);


        }

        nn->outputList[i]->curValue = tempcurval+nn->outputList[i]->bias;
    }
    
    
    return 0;
}

/*
 * This function updates the weights of the network based on an input-output pair. The weights are adjusted once.
 * For full training, this function must be called multiple times until it returns -1 as the network error.
 */
double updateSimpleNetworkWeights(SimpleNetwork * nn, double * desiredInputs, int numDesiredInputs, double * desiredOutputs, int numDesiredOutputs)
{
    
    if ( numDesiredOutputs != nn->outputSize)
    {
        printf("The number of desired outputs doesn't match the number of outputs in the network. Consider revising.\n");
        exit(1);
    }
    
    if(numDesiredInputs != nn->inputSize)
    {
        printf("The number of desired inputs doesn't match the number of inputs in the network. Consider revising.\n");
        exit(1);
    }

    int o;
    int h;
    int i;
    double outputError;
    outputError = 0;
    
    updateSimpleNetworkCurVals(nn, desiredInputs, numDesiredInputs);
    
    //calculate error of the outputs of the network
    for(o = 0; o < nn->outputSize; o++)
    {
        outputError += pow((hardSigmoid(nn->outputList[o]->curValue)-desiredOutputs[o]), 2);
    }
    
    if(outputError < .001)
        return -1;
    
    // update weights and biases of network
    for(o = 0; o < nn->outputSize; o++)
    {
        nn->outputList[o]->bias +=(-1)*RELAX_CONSTANT*2*(hardSigmoid(nn->outputList[o]->curValue) - desiredOutputs[o])*sigmoidDerivative(nn->outputList[o]->curValue, 1);
        
        for(h = 0; h < nn->outputList[o]->numInEdges; h++)
        {
            nn->outputList[o]->inEdges[h]->nextWeight += (-1)*RELAX_CONSTANT*2*(hardSigmoid(nn->outputList[o]->curValue) - desiredOutputs[o])*sigmoidDerivative(nn->outputList[o]->curValue, 1)*hardSigmoid(nn->outputList[o]->inEdges[h]->startNode->curValue);
        }
    }
    
    for(o = 0; o < nn->outputSize; o++)
    {
        for(h = 0; h < nn->outputList[o]->numInEdges; h++)
        {
            nn->outputList[o]->inEdges[h]->startNode->bias += (-1)*RELAX_CONSTANT*2*(hardSigmoid(nn->outputList[o]->curValue) - desiredOutputs[o])*sigmoidDerivative(nn->outputList[o]->curValue, 1)*nn->outputList[o]->inEdges[h]->simpleWeight*sigmoidDerivative(nn->outputList[o]->inEdges[h]->startNode->curValue, 1);

            for(i = 0; i < nn->outputList[o]->inEdges[h]->startNode->numInEdges; i++)
            {
                nn->outputList[o]->inEdges[h]->startNode->inEdges[i]->nextWeight += (-1)*RELAX_CONSTANT*2*(hardSigmoid(nn->outputList[o]->curValue) - desiredOutputs[o])*sigmoidDerivative(nn->outputList[o]->curValue, 1)*nn->outputList[o]->inEdges[h]->simpleWeight*sigmoidDerivative(nn->outputList[o]->inEdges[h]->startNode->curValue, 1)*hardSigmoid(nn->outputList[o]->inEdges[h]->startNode->inEdges[i]->startNode->curValue);
            }
        }
    }
    
    for(o = 0; o < nn->outputSize; o++)
    {
        for(h = 0; h < nn->outputList[o]->numInEdges; h++)
        {
            nn->outputList[o]->inEdges[h]->simpleWeight = nn->outputList[o]->inEdges[h]->nextWeight;
        }
    }
    
    
    for(o = 0; o < nn->outputSize; o++)
    {
        for(h = 0; h < nn->outputList[o]->numInEdges; h++)
        {
            for(i = 0; i < nn->outputList[o]->inEdges[h]->startNode->numInEdges; i++)
            {
                nn->outputList[o]->inEdges[h]->startNode->inEdges[i]->simpleWeight = nn->outputList[o]->inEdges[h]->startNode->inEdges[i]->nextWeight;
            }
        }
    }
    
    return outputError;
}

/*
 * This function applies a hardlimited sigmoid functions to an input argument.
 */
double hardSigmoid(double x){
    
    double y = (1/(1+exp(-1*x)));
    
    if(y < .05)
        y =  0;
    else if( y > .95)
        y = 1;
    return y;
    
}

/*
 * This function applies a normal sigmoid function to the input.
 */
double sigmoid(double x){
    double y = (1/(1+exp(-1*x)));
    return y;
}

/*
 * This function applies the derivative of the sigmoid to the input.
 */
double sigmoidDerivative(double x, int n){
    double y = sigmoid(x)*(1-sigmoid(x));
    return y;
}
