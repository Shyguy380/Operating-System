#ifndef SIMULATOR_H
#define SIMULATOR_H

#include <stdio.h>
#include <pthread.h>
#include "StringUtils.h"
#include "datatypes.h"
#include "OS_SimDriver.h"

void runSim( ConfigDataType *configPtr, OpCodeType *metaDataMstrPtr );
void fileOutput( ConfigDataType *configInfo, fileOutputType *outputHead );
void processEnded( ConfigDataType *configPtr, PCBType *currentPCB, fileOutputType *fileOutputNode );
void changeState( ConfigDataType *configPtr, PCBType *currentPCB, char* state, fileOutputType *fileOutputNode );

PCBType *createPCBs( ConfigDataType *configDataPtr, OpCodeType *metaDataMstrPtr, fileOutputType *outputHead );
PCBType *addPCB( PCBType *localPtr, PCBType *newNode );
PCBType *clearPCBList( PCBType *localPtr );

fileOutputType *simulatorOutput ( ConfigDataType *configInfo, OpCodeType *currentOpCode, fileOutputType *outputHead, char* startEndFlag );
fileOutputType *addOutput( fileOutputType *localPtr, fileOutputType *newNode );
fileOutputType *clearFileOutputList( fileOutputType *localPtr );

#endif