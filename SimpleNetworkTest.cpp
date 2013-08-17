//
//  SimpleNetworkTest.cpp
//  NeuralNetworkV3
//
//  Created by Jason Schray on 8/7/13.
//  Copyright (c) 2013 Jason Schray. All rights reserved.
//

#include "SimpleNetworkTest.h"
#include "SimpleNetwork.h"
#include <math.h>

#define MAX_SUB_TESTS 8

int testmakeSimpleNetwork(void);

int testupdateSimpleNetworkCurVals(void);

int testupdateSimpleNetworkWeights(void);

int testSimpleNetwork(void)
{
    int testerror = 0;
    
    
    // Logic for calling and interpreting unit tests for the makeSimpleNetwork() function
    testerror = testmakeSimpleNetwork();
    
    if(testerror == 0)
        printf("All makeSimpleNetwork tests passed.\n");
    else
    {
        printf("An error occured while testing the makeSimpleNetwork function.\n");
        for(int i = 0; i < MAX_SUB_TESTS; i ++)
        {
            if ((testerror & (1 << i)) != 0)
                printf("Test %d Failed.\n", i);
        }
    }
    
    testerror = testupdateSimpleNetworkCurVals();
    if(testerror == 0)
        printf("All updateSimpleNetworkCurVals tests passed.\n");
    else
    {
        printf("An error occured while testing the updateSimpleNetworkCurVals function.\n");
        for(int i = 0; i < MAX_SUB_TESTS; i ++)
        {
            if ((testerror & (1 << i)) != 0)
                printf("Test %d Failed.\n", i);
        }
    }
    
    testerror = testupdateSimpleNetworkWeights();
    if(testerror == 0)
        printf("All updateSimpleNetworkWeights tests passed.\n");
    else
    {
        printf("An error occured while testing the updateSimpleNetworkWeights function.\n");
        for(int i = 0; i < MAX_SUB_TESTS; i ++)
        {
            if ((testerror & (1 << i)) != 0)
                printf("Test %d Failed.\n", i);
        }
    }

    return 0;
}


int testmakeSimpleNetwork(void)
{
    int error = 0;
    
    SimpleNetwork * nn = makeSimpleNetwork();
    
    int numinputs = 20;
    int numhiddens = 20;
    int numoutputs = 10;
    
    createSimpleNetwork(nn, numinputs,numhiddens,numoutputs);
    
    // Test 0
    for(int i = 0; i < nn->inputSize; i ++)
    {
        for(int h = 0; h < nn->inputList[i]->numOutEdges; h++)
        {
            if(nn->inputList[i]->outEdges[h]->endNode!= nn->hiddenList[h])
                error |= (1 << 0);
                
        }
    }
    
    // Test 1
    for(int h = 0; h < nn->hiddenSize; h ++)
    {
        for(int i = 0; i < nn->hiddenList[h]->numInEdges; i++)
        {
            if(nn->hiddenList[h]->inEdges[i]->startNode != nn->inputList[i])
                error |= (1 << 1);
        }
    }
    
    // Test 2
    for(int h = 0; h < nn->hiddenSize; h ++)
    {
        for(int o = 0; o < nn->hiddenList[h]->numOutEdges; o++)
        {
            if(nn->hiddenList[h]->outEdges[o]->endNode != nn->outputList[o])
                error |= ( 1 << 2);
        }
    }
    
    // Test 3
    for(int o = 0; o < nn->outputSize; o++)
    {
        for(int h = 0; h < nn->outputList[o]->numInEdges; h++)
        {
            if(nn->outputList[o]->inEdges[h]->startNode != nn->hiddenList[h])
                error |= (1 << 3);
        }
    }
    
    // Test 4
    if(nn->inputSize != numinputs)
        error |= (1 << 4);
    
    // Test 5
    if(nn->hiddenSize != numhiddens)
        error |= (1 << 5);
    
    // Test 6
    if(nn->outputSize != numoutputs)
        error |= (1 << 6);
    
    return error;
}

int testupdateSimpleNetworkCurVals(void)
{
    int error = 0;
    
    SimpleNetwork * nn = makeSimpleNetwork();
    
    createSimpleNetwork(nn, 2, 2, 1);
    
    nn->nodeList[0]->bias = -2;
    nn->nodeList[1]->bias = -2;
    nn->nodeList[2]->bias = -3;
    nn->nodeList[3]->bias = -3;
    nn->nodeList[4]->bias = -3;
    
    nn->nodeList[0]->outEdges[0]->simpleWeight = 5;
    nn->nodeList[0]->outEdges[1]->simpleWeight = -5;
    nn->nodeList[1]->outEdges[0]->simpleWeight = -5;
    nn->nodeList[1]->outEdges[1]->simpleWeight = 5 ;
    nn->nodeList[2]->outEdges[0]->simpleWeight = 8;
    nn->nodeList[3]->outEdges[0]->simpleWeight = 8;

    double input[2];
    
    input[0] = 0;
    input[1] = 0;
    
    updateSimpleNetworkCurVals(nn, input, sizeof(input)/sizeof(input[0]));
    
    if(hardSigmoid(nn->nodeList[nn->netWorkSize-1]->curValue) != 0)
    {
        error |= (1 << 0);

    }
    
    
    input[0] = 1;
    input[1] = 1;
    
    updateSimpleNetworkCurVals(nn, input, sizeof(input)/sizeof(input[0]));
    
    if(hardSigmoid(nn->nodeList[nn->netWorkSize-1]->curValue) != 0)
    {
        printf("%f\n", (nn->nodeList[2]->curValue));
        error |= (1 << 1);
    }

    input[0] = 0;
    input[1] = 1;
    
    updateSimpleNetworkCurVals(nn, input, sizeof(input)/sizeof(input[0]));
    
    if(hardSigmoid(nn->nodeList[nn->netWorkSize-1]->curValue) != 1)
    {
        error |= (1 << 2);
    }
    
    input[0] = 1;
    input[1] = 0;
    
    updateSimpleNetworkCurVals(nn, input, sizeof(input)/sizeof(input[0]));
    
    if(hardSigmoid(nn->nodeList[nn->netWorkSize-1]->curValue) != 1)
    {
        error |= (1 << 3);
    }

    // Tori was here
    
    return error;
}

int testupdateSimpleNetworkWeights(void)
{
    int error = 0;
 
    SimpleNetwork * nn = makeSimpleNetwork();
    createSimpleNetwork(nn, 2,30,1);

    double inputs[2] = {0,0};
    double outputs[1] = {1};
    double networkError;
    

    int temperror;
    int i;
    for(i = 0; i < 50000; i ++)
    {
        temperror = 0;
        
        inputs[0] = 0;
        inputs[1] = 0;
        outputs[0] = 0;
        networkError = updateSimpleNetworkWeights(nn, inputs, 2, outputs, 1);
        updateSimpleNetworkCurVals(nn, inputs, 2);
        
        if (networkError != -1)
        {
            temperror = 1;
        }
    
        inputs[0] = 0;
        inputs[1] = 1;
        outputs[0] = 1;
        networkError = updateSimpleNetworkWeights(nn, inputs, 2, outputs, 1);
        updateSimpleNetworkCurVals(nn, inputs, 2);
        
        if (networkError != -1)
        {
            temperror = 1;
        }
        
        inputs[0] = 1;
        inputs[1] = 1;
        outputs[0] = 0;
        networkError = updateSimpleNetworkWeights(nn, inputs, 2, outputs, 1);
        updateSimpleNetworkCurVals(nn, inputs, 2);
        
        if (networkError != -1)
        {
            temperror = 1;
        }

        inputs[0] = 1;
        inputs[1] = 0;
        outputs[0] = 1;
        networkError = updateSimpleNetworkWeights(nn, inputs, 2, outputs, 1);
        updateSimpleNetworkCurVals(nn, inputs, 2);

        if (networkError != -1)
        {
            temperror = 1;
        }
        
        if (temperror == 0)
        {
            break;
        }
    }
    
    // the following statements test for the error of the system to make sure that it is close to the desired outputs

    printf("The number of iterations was: %d\n", i);
    
    // Test 0
    inputs[0] = 0;
    inputs[1] = 0;
    updateSimpleNetworkCurVals(nn, inputs, 2);
    
    if(pow(hardSigmoid(nn->outputList[0]->curValue) - 0, 2) > .001)
        error |= (1 << 0);
    
    // Test 1
    inputs[0] = 1;
    inputs[1] = 1;
    updateSimpleNetworkCurVals(nn, inputs, 2);
    
    if(pow(hardSigmoid(nn->outputList[0]->curValue) - 0, 2) > .001)
        error |= (1 << 1);
    
    // Test 2
    inputs[0] = 0;
    inputs[1] = 1;
    updateSimpleNetworkCurVals(nn, inputs, 2);
    
    if(pow(hardSigmoid(nn->outputList[0]->curValue) - 1, 2) > .001)
        error |= (1 << 2);
    
    // Test 3
    inputs[0] = 1;
    inputs[1] = 0;
    updateSimpleNetworkCurVals(nn, inputs, 2);
    
    if(pow(hardSigmoid(nn->outputList[0]->curValue) - 1, 2) > .001)
        error |= (1 << 3);
    
    
    return error;
}