//
//  SimpleNetwork.h
//  NeuralNetworkV3
//
//  Created by Jason Schray on 8/7/13.
//  Copyright (c) 2013 Jason Schray. All rights reserved.
//

#ifndef NeuralNetworkV3_SimpleNetwork_h
#define NeuralNetworkV3_SimpleNetwork_h

#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>


using namespace std;

typedef enum {INPUT, HIDDEN, OUTPUT} NodeType;

const double WEIGHT_CONSTANT = 12;
const double RELAX_CONSTANT = .1;
const int MAX_STEPS = 10;

typedef struct SimpleNode
{
    NodeType nodeType; // possilby delete later if superfluous
    double bias;
    double curValue;
    int numInEdges;
    int numOutEdges;
    struct SimpleEdge ** inEdges;
    struct SimpleEdge ** outEdges;
    
    int arr[5] = {1,2,3,4,5};
    
    SimpleNode(double biasvalue = ((double)rand())/RAND_MAX) {
        curValue = 0;
        bias = biasvalue;
        numOutEdges = 0;
        numOutEdges = 0;
        inEdges = NULL;
        outEdges = NULL;
        
    }
    
}SimpleNode;

typedef struct SimpleEdge
{
    SimpleNode * startNode;
    SimpleNode * endNode;
    double simpleWeight;
    double nextWeight;
}SimpleEdge;

typedef struct SimpleNetwork
{
    int netWorkSize;
    SimpleNode ** nodeList = NULL;
    
    int inputSize;
    SimpleNode ** inputList = NULL;
    
    int hiddenSize;
    SimpleNode ** hiddenList = NULL;
    
    int outputSize;
    SimpleNode ** outputList = NULL;
    
    SimpleNetwork(int inputNodes, int hiddenNodes, int outputNodes)
    {
        netWorkSize = (inputNodes+hiddenNodes+outputNodes);
        nodeList = (SimpleNode**)malloc(netWorkSize*sizeof(SimpleNode*));
        inputSize = inputNodes;
        
        hiddenSize = hiddenNodes;
        outputSize = outputNodes;
    }
    
}SimpleNetwork;

SimpleNode* makeSimpleNode(NodeType ntype, double biasvalue);

SimpleEdge* makeSimpleEdge(SimpleNode* s, SimpleNode* e, double w);

SimpleNetwork* makeSimpleNetwork();

int createWeight(SimpleNode startNode, SimpleNode endNode, int weight);

int createSimpleNetwork(SimpleNetwork* nn, int inputNodes = 2, int hiddenNodes = 2, int outputNodes = 1);

int updateSimpleNetworkCurVals(SimpleNetwork * nn, double * inputs, int numInputs);

double updateSimpleNetworkWeights(SimpleNetwork * nn, double * desiredInputs, int numDesiredInputs, double * desiredOutputs, int numDesiredOutputs);

double weightDelta(double error, double m);

double hardSigmoid(double x);

double sigmoid(double x);

double sigmoidDerivative(double x, int n);


#endif
