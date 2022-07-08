// protect from multiple compiling
#ifndef DATATYPES_H
#define DATATYPES_H

// header files
#include <stdio.h>
#include <stdbool.h>

// GLOBAL CONSTANTS - may be used in other files
typedef enum { False, True } Boolean;

// config data structure
typedef struct ConfigDataType
    {
        double version;
        char metaDataFileName[ 100 ];
        int cpuSchedCode;
        int quantumCycles;
        int memAvailable;
        int procCycleRate;
        int ioCycleRate;
        int logToCode;
        char logToFileName[ 100 ];
    } ConfigDataType;

typedef struct OpCodeType
    {
        int pid;                        // pid, added when PCB is created
        char command[ 5 ];              // three letter command quantity
        char inOutArg[ 5 ];             // for device in/out
        char strArg1[ 15 ];             // arg 1 descriptor, up to 12 chars
        int intArg2;                    // cycles or memory, assumes 4 byte int
        int intArg3;                    // memory, assumes 4 byte int
                                        // also non/premption indicator
        double opEndTime;               // size of time string returned from accessTimer
        struct OpCodeType *nextNode;    // pointer to next node as needed
    } OpCodeType;

//PCB data structure
typedef struct PCBType
    {
        int pid;                             //Process Identification Number
        int timeRemaining;                   //
        char state[ 10 ];                    //Which state the process is in
        struct OpCodeType *firstOpCodeNode;  //Set pointer to its first opcode
        struct PCBType *nextNode;            //pointer to next node as needed
    } PCBType;

//File output data structure
typedef struct fileOutputType
    {
        char lineOfOutput[ 64 ];
        struct fileOutputType *nextLine;
    } fileOutputType;

#endif  // DATATYPES_H