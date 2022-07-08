#include "simulator.h"
#include "metadataops.h"
#include "configops.h"

/*
Function Name: runSim
Algorithm: master Driver for simulator operations;
           conducts OS sumulation with varying scheduling strategies
           and varying numbers of processes
Precondition: given head pointer to config data and meta data
Postcondition; simulation is provided, file output is provided as confiured
Exceptions: none
Notes: none
*/
// First parameter is a complete ConfigDataType struct
// Second parameter is the beginning of LL of OpCodes
void runSim(ConfigDataType *configPtr, OpCodeType *metaDataMstrPtr)
   {
        // Initial Output
        printf( "Simulator Run\n" );
        printf( "-------------\n\n" );

        // Initialize Variables
            //Setting up POSIX Threads
                pthread_t pidValue;
                int *timePtr;
                int time;

                char timerTime[ STD_STR_LEN ];

            // Starting Timer
            accessTimer( ZERO_TIMER, timerTime );

            // Needed for main loop
            bool runFlag = true;

            // Need to declare when an operation has started/stopped
            char startEndFlag[ STD_STR_LEN ] = "start";

            // Initializes an OpCodeType for parsing
            OpCodeType *currentOpCode = metaDataMstrPtr;

            // Initializes a fileOutputType for building a linked list
            fileOutputType *outputHead = (fileOutputType *)malloc( sizeof( fileOutputType ) );

            // Start The Timer
            accessTimer( LAP_TIMER, timerTime );
            simulatorOutput( configPtr, metaDataMstrPtr, outputHead, startEndFlag );
            printf("\n");

            // Generates PCBs and sets a PCBType for parsing
            PCBType *headPCB = createPCBs(configPtr, metaDataMstrPtr, outputHead);
            PCBType *currentPCB = headPCB;
            printf("\n");

        while(runFlag)
            {
                // Find Next Process
                   // This switch case incorporates sheduling
                switch( configPtr->cpuSchedCode )
                    {
                    case CPU_SCHED_SJF_N_CODE:
                        // Shortest Job First Non-Preemptive

                        // Fall Back To FCFS-N
                        if( compareString( currentPCB->state, "EXIT") == STR_EQ)
                            {
                                currentPCB = currentPCB->nextNode;
                            }

                        break;

                    case CPU_SCHED_SRTF_P_CODE:
                        // Shortest Remaining Time First Preemptive

                        // Fall Back To FCFS-N
                        if( compareString( currentPCB->state, "EXIT") == STR_EQ)
                            {
                                currentPCB = currentPCB->nextNode;
                            }

                        break;

                    case CPU_SCHED_FCFS_P_CODE:
                        // First Come First Serve Preemptive (add priority)

                        // Fall Back To FCFS-N
                        if( compareString( currentPCB->state, "EXIT") == STR_EQ)
                            {
                                currentPCB = currentPCB->nextNode;
                            }

                        break;

                    case CPU_SCHED_RR_P_CODE:
                        // Round Robin Preemptive

                        // Fall Back To FCFS-N
                        if( compareString( currentPCB->state, "EXIT") == STR_EQ)
                            {
                                currentPCB = currentPCB->nextNode;
                            }

                        break;

                    case CPU_SCHED_FCFS_N_CODE:
                        // First Come First Server Non-Preemptive

                        if( compareString( currentPCB->state, "EXIT") == STR_EQ)
                            {   
                                currentPCB = currentPCB->nextNode;
                            }

                        break;
                    }

                // Find Next Op Code
                    // Not necessary for sim 2/3 (use process counter)

                // Check for Input
                if( compareString( currentOpCode->inOutArg, "in") == STR_EQ )
                    {
                        // Set flag that operation has begun
                        copyString( startEndFlag, "start");

                        // Call Timer / Calculate time using accessTimer
                        time = currentOpCode->intArg2 * configPtr->ioCycleRate;
                        timePtr = &time;
                        pthread_create( &pidValue, NULL, runTimer, (void *)timePtr );
                        pthread_join( pidValue, NULL );

                        // Call for outputting
                        simulatorOutput( configPtr, currentOpCode, outputHead, startEndFlag );

                        // Set flag that operation has ended
                        copyString( startEndFlag, "end");

                        // Call for outputting
                        simulatorOutput( configPtr, currentOpCode, outputHead, startEndFlag );
                    }

                // Check for Output
                if( compareString( currentOpCode->inOutArg, "out") == STR_EQ )
                    {
                        // Get The Current Time

                        // Set flag that operation has begun
                        copyString( startEndFlag, "start");

                        // Call Timer / Calculate time using accessTimer
                        time = currentOpCode->intArg2 * configPtr->ioCycleRate;
                        timePtr = &time;
                        pthread_create( &pidValue, NULL, runTimer, (void *)timePtr );
                        pthread_join( pidValue, NULL );

                        // Call for outputting
                        simulatorOutput( configPtr, currentOpCode, outputHead, startEndFlag );
                    
                        // Get The Current Time

                        // Set flag that operation has ended
                        copyString( startEndFlag, "end");

                        // Call for outputting
                        simulatorOutput( configPtr, currentOpCode, outputHead, startEndFlag );
                    }

                // Check for Idle
                if( compareString( currentOpCode->command, "idle") == STR_EQ )
                    {
                        // nothing for sim 2
                    }
                
                // Check for Mem
                if( compareString( currentOpCode->command, "mem") == STR_EQ )
                    {
                        // nothing for sim 2
                    }

                // Check for CPU   
                if( compareString( currentOpCode->command, "cpu") == STR_EQ )
                    {
                        // Get The Current Time

                        // Set flag that operation has begun
                        copyString( startEndFlag, "start");

                        // Call Timer / Calculate time using accessTimer
                        time = currentOpCode->intArg2 * configPtr->ioCycleRate;
                        timePtr = &time;
                        pthread_create( &pidValue, NULL, runTimer, (void *)timePtr );
                        pthread_join( pidValue, NULL );

                        // Call for outputting
                        simulatorOutput( configPtr, currentOpCode, outputHead, startEndFlag );
                    
                        // Get The Current Time

                        // Set flag that operation has ended
                        copyString( startEndFlag, "end");

                        // Call for outputting

                        simulatorOutput( configPtr, currentOpCode, outputHead, startEndFlag );
                    }

                // Check for App End
                if( compareString( currentOpCode->command, "app" ) == STR_EQ &&
                    compareString( currentOpCode->strArg1, "end" ) == STR_EQ )
                    {
                        // change states and output
                        processEnded( configPtr, currentPCB, outputHead );
                    }

                //Check for and resolve interrupts
                /*
                if( there is an interrupt as in Sim04)
                    {
                        // Nothing for sim 2
                    }
                */

                // If we are at sys end we are done
                if( compareString( currentOpCode->command, "sys" ) == STR_EQ &&
                    compareString( currentOpCode->strArg1, "end" ) == STR_EQ )
                    {
                        // stop running
                        runFlag = false;
                        printf("\n");
                        simulatorOutput( configPtr, currentOpCode, outputHead, startEndFlag );
                    }

                currentOpCode = currentOpCode->nextNode;

            // End Main Loop
            }

        // Stop The Timer
        accessTimer( STOP_TIMER, timerTime );

        //Spit linked list to file if needed
        if( configPtr->logToCode == LOGTO_FILE_CODE ||
            configPtr->logToCode == LOGTO_BOTH_CODE)
            {
                fileOutput( configPtr, outputHead );
            }

    // End Function
    }

/*
Function Name: createPCBs
Algorithm: generates a PCB within each app start/stop.
           send it to addPCB to be added to LL
*/
PCBType *createPCBs( ConfigDataType *configDataPtr, OpCodeType *metaDataMstrPtr, fileOutputType *outputHead )
    {

    // Intialize variables
    int timeRemaining;                                          //Used to track time for ouput
    int pIndex = 0;                                             //Keep track of process index
    OpCodeType *currentOpCode = metaDataMstrPtr;                //The beginning of the opcodes
    PCBType *PCBHead = NULL;

    // create PCB node
    PCBType *currentPCB = (PCBType *)malloc( sizeof( PCBType ));

    bool runFlag = true;
    bool appEndFlag = true;

    // Loop across the metadata (technically all of the op codes passed to us)
    while(runFlag)
            {
            
                currentOpCode = currentOpCode->nextNode;

                // check for app start ( and maybe that we haven't hit end?? flag )
                if(compareString(currentOpCode->command, "app") == STR_EQ &&
                   compareString(currentOpCode->strArg1, "start") == STR_EQ)
                    {
                        // set data (pid, state, pointer to first opcode in block that is process)
                        currentPCB->pid = pIndex;
                        currentPCB->firstOpCodeNode = currentOpCode;
                        currentPCB->nextNode = NULL;

                        // set time remaining to zero
                        timeRemaining = 0;
                        appEndFlag = true;

                        // check for not app end
                        while(appEndFlag)
                            {
                                // if dev in/out
                                if(compareString(currentOpCode->command, "dev") == STR_EQ &&
                                    (compareString(currentOpCode->strArg1, "in") == STR_EQ  || 
                                     compareString(currentOpCode->strArg1, "out") == STR_EQ))
                                    {
                                        timeRemaining += currentOpCode->intArg2 * configDataPtr->ioCycleRate;
                                    }

                                // if cpu
                                else if(compareString(currentOpCode->command, "cpu") == STR_EQ &&
                                        compareString(currentOpCode->strArg1, "process") == STR_EQ)
                                    {
                                        timeRemaining += currentOpCode->intArg2 * configDataPtr->procCycleRate;
                                    }

                                // if mem
                                else if(compareString(currentOpCode->command, "mem") == STR_EQ &&
                                        compareString(currentOpCode->strArg1, "") == STR_EQ)
                                    {
                                        // Dont need for sim 2
                                    }


                                //Associates OpCodes with PCBs
                                currentOpCode->pid = pIndex;

                                // move to next opcode
                                currentOpCode = currentOpCode->nextNode;

                                if(compareString(currentOpCode->command, "app") == STR_EQ &&
                                    compareString(currentOpCode->strArg1, "end") == STR_EQ)
                                    {
                                        appEndFlag = false;
                                    }

                            }

                        // otherwise, assume app end
                        currentPCB->timeRemaining = timeRemaining;
                        changeState( configDataPtr, currentPCB, "READY", outputHead );
                        PCBHead = addPCB( PCBHead, currentPCB );
                        pIndex++;

                    }

                if(compareString(currentOpCode->command, "sys") == STR_EQ)
                    {
                        runFlag = false;
                    }

            // end loop across metadata
            }

        free(currentPCB);

        // end function
        return PCBHead;
    }

/*
Function Name: addPCB
Algorithm: Adds a given PCB to the linked list of PCBs
*/
PCBType *addPCB( PCBType *localPtr, PCBType *newNode )
    {
        // check for local pointer assigned to null
        if( localPtr == NULL )
            {
                // access memory for new link/node
                    // function: malloc
                localPtr = (PCBType *)malloc( sizeof( PCBType ) );

                // assign all values to newly created node

                // assign next pointer to null
                    // function: copyString
                localPtr->pid = newNode->pid;
                localPtr->timeRemaining = newNode->timeRemaining;
                copyString( localPtr->state, newNode->state );
                localPtr->firstOpCodeNode = newNode->firstOpCodeNode;
                localPtr->nextNode = NULL;

                // return current local pointer
                return localPtr;
            }

        // assume end of list not found yet
        // assign recursive function to current's next link
            // function: addNode
        localPtr->nextNode = addPCB( localPtr->nextNode, newNode );

        // return current local pointer
        return localPtr;
    }



/*
Function Name: simulatorOuput
Algorithm: formats and puts necessary output in a string
           sends it to addOutput to add to Linked List of output lines
*/
fileOutputType *simulatorOutput( ConfigDataType *configPtr, OpCodeType *currentOpCode, fileOutputType *outputHead, char* startEndFlag )
    {
        // initialize function/variables
        fileOutputType *currentLine = (fileOutputType *)malloc( sizeof( fileOutputType ) );

        // Current Time
        char timerTime[ STD_STR_LEN ];
        accessTimer(LAP_TIMER, timerTime);

        // Commands: sys, app, dev, cpu, mem
        char commandInput[ STD_STR_LEN ];

        // In/Out arguments: in, out
        char inOrOut[ STD_STR_LEN ];

        // First String Arguments:
        // start, end process, allocate, access, ethernet, hard drive, 
        // keyboard, monitor, serial, sound signal, usb, video signal.
        char firstStringArgument[ STD_STR_LEN ];

        // Temp string for storage during concatenation process
        char tempString[ STD_STR_LEN ];

        // For outputting to file
        char outputLine[ STD_STR_LEN ];

        // For whether the operation is starting or stopping
        char startOrStop[ STD_STR_LEN ];

        char temptemp[ STD_STR_LEN ];

        // If the opcode is of command SYS
        if( compareString( currentOpCode->command, "sys" ) == STR_EQ )
            {   
                copyString(commandInput, "OS");

                // If the opcode is sys start or end
                if( compareString(currentOpCode->strArg1, "start" ) == STR_EQ ||
                    compareString(currentOpCode->strArg1, "end" ) == STR_EQ )
                    {
                        copyString(firstStringArgument, "Simulator");
                    }

            }



        // If the opcode is of command APP
        else if( compareString( currentOpCode->command, "app" ) == STR_EQ )
            {   
                copyString(commandInput, "OS");

                // If the opcode is app start
                if(compareString(currentOpCode->strArg1, "start") == STR_EQ )
                    {   

                        copyString(firstStringArgument, "");
                    }

                // If the opcode is app end
                if(compareString(currentOpCode->strArg1, "end") == STR_EQ )
                    {
                        copyString(firstStringArgument, "");
                    }
            }

        // If the opcode is of command DEV
        else if( compareString( currentOpCode->command, "dev" ) == STR_EQ )
            {   
                copyString( commandInput, "Process" );
                
                // This may error because of types and using the second parameter
                copyString( firstStringArgument, currentOpCode->strArg1 );
            }

        // If the opcode is of command CPU
        else if( compareString( currentOpCode->command, "cpu" ) == STR_EQ )
            {   
                copyString( commandInput, "Process" );

                copyString( firstStringArgument, "cpu process operation" );
                
                sprintf(temptemp, "%s", " ");

                copyString( inOrOut, temptemp );
                
                
            }

        if( compareString( currentOpCode->inOutArg, "in" ) == STR_EQ )
            {
                copyString( inOrOut, "input operation " );
            }

        if( compareString( currentOpCode->inOutArg, "out" ) == STR_EQ )
            {
                copyString( inOrOut, "output operation " );
            }

        if( compareString( startEndFlag, "start" ) == STR_EQ )
            {
                copyString( startOrStop, "start" );
            }

        if( compareString( startEndFlag, "end" ) == STR_EQ )
            {
                copyString( startOrStop, "end" );
            }

        // Per Line Output For Process Output
        // This should use result of access timer
        
        // Adds the time for the beggining of the line
        sprintf(outputLine, "%s, ", timerTime);

        // Adds the "OS" or "Process"
        sprintf(tempString, "%s: ", commandInput);
        concatenateString( outputLine, tempString );

        // Adds the pid of the process
        if( compareString( currentOpCode->command, "sys" ) != STR_EQ )
            {
                sprintf(tempString, "%d, ", currentOpCode->pid);
                concatenateString( outputLine, tempString );
            }

        // Adds the label ( cpu process, hard drive output, etc. )
        sprintf(tempString, "%s ", firstStringArgument);
        concatenateString( outputLine, tempString );

        // Adds the input or output
        if( compareString( currentOpCode->command, "dev" ) == STR_EQ )
            {
                sprintf(tempString, "%s", inOrOut);
                concatenateString( outputLine, tempString );
            }

        // Adds if it is the start or stop of the opcode
        sprintf(tempString, "%s \n", startOrStop);
        concatenateString( outputLine, tempString );
        

        // Sets the line we made to the line of the node we are going to add
        copyString( currentLine->lineOfOutput, outputLine );

        if( configPtr->logToCode == LOGTO_MONITOR_CODE ||
            configPtr->logToCode == LOGTO_BOTH_CODE)
            {
                printf( "%s", outputLine );
            }

        if( configPtr->logToCode == LOGTO_FILE_CODE ||
            configPtr->logToCode == LOGTO_BOTH_CODE)
            {
                addOutput( outputHead, currentLine );
            }
        
        free(currentLine);

        return NULL;
    }

/*
Function Name: fileOutput
Algorithm: moves through linked list of strings and puts to file
*/
void fileOutput( ConfigDataType *configInfo, fileOutputType *outputHead )
    {
        // initialize function/variables
        FILE *fileToOutput;

        fileToOutput = fopen( configInfo->logToFileName, "w" );

        // Initial Output
        fprintf(fileToOutput, "=========================\n");
        fprintf(fileToOutput, "Simulator Log File Header\n");

        fprintf(fileToOutput, "File Name                       : %s\n", configInfo->metaDataFileName);
        fprintf(fileToOutput, "CPU Scheduling                  : %d\n", configInfo->cpuSchedCode);
        fprintf(fileToOutput, "Quantum Cycles                  : %d\n", configInfo->quantumCycles);
        fprintf(fileToOutput, "Memory Available (KB)           : %d\n", configInfo->memAvailable);
        fprintf(fileToOutput, "Processor Cycle Rate (ms/cycle) : %d\n", configInfo->procCycleRate);
        fprintf(fileToOutput, "I/O Cycle Rate (ms/cycle)       : %d\n", configInfo->ioCycleRate);

        fprintf(fileToOutput, "================\n");
        fprintf(fileToOutput, "Begin Simulation\n");

        // Run through LL of strings and dump
        // access LL head
        fileOutputType *currentLine = outputHead->nextLine;

        while( currentLine->nextLine != NULL )
            {
                fprintf( fileToOutput,"%s", currentLine->lineOfOutput );
                currentLine = currentLine->nextLine;
            }

        fclose( fileToOutput );
    }

//We need to think about scheduling so we need a way to keep the next PCB in mind
    // ONLY WORKS FOR FCFS-N 
    // will need to incorporate nextNode for which should be up next based on schedulinh
void processEnded( ConfigDataType *configPtr, PCBType *currentPCB, fileOutputType *fileOutputNode )
    {
        // Initialize Variables
        fileOutputType *currentLine = (fileOutputType *)malloc( sizeof( fileOutputType ) );

        char lineOne[ STD_STR_LEN ];
        char lineTwo[ STD_STR_LEN ];
        char lineThree[ STD_STR_LEN ];
        char lineFour[ STD_STR_LEN ];

        char timerTime[ STD_STR_LEN ];

        // Current Process Has Completed
        accessTimer(LAP_TIMER, timerTime);
        sprintf( lineOne, "\n%s, OS: Process %d ended\n", timerTime, currentPCB->pid );
        
        // It is set to EXIT
        copyString( currentPCB->state, "EXIT" );
        accessTimer(LAP_TIMER, timerTime);
        sprintf( lineTwo, "%s, OS: Process %d set to %s\n", timerTime, currentPCB->pid, currentPCB->state );

        
        

        // Next process is set to running
        if(currentPCB->nextNode != NULL)
            {
                // Next Process Is Selected
                accessTimer(LAP_TIMER, timerTime);
                sprintf( lineThree, "%s, OS: Process %d selected with %d ms remaining\n", timerTime, currentPCB->nextNode->pid, currentPCB->nextNode->timeRemaining );

                copyString( currentPCB->nextNode->state, "RUNNING" );
                accessTimer(LAP_TIMER, timerTime);
                sprintf( lineFour, "%s, OS: Process %d set from READY to RUNNING\n\n", timerTime, currentPCB->nextNode->pid );
            }

        // If we are logging to monitor or both, we just need to print
        if( configPtr->logToCode == LOGTO_MONITOR_CODE ||
            configPtr->logToCode == LOGTO_BOTH_CODE)
            {
                printf( "%s", lineOne );
                printf( "%s", lineTwo );
                printf( "%s", lineThree );
                printf( "%s", lineFour );
            }

        // If we want to log to file we need to add the four lines to the LL of strings
        if( configPtr->logToCode == LOGTO_FILE_CODE ||
            configPtr->logToCode == LOGTO_BOTH_CODE)
            {
                copyString( currentLine->lineOfOutput, lineOne );
                addOutput( fileOutputNode, currentLine );

                copyString( currentLine->lineOfOutput, lineTwo );
                addOutput( fileOutputNode, currentLine );

                copyString( currentLine->lineOfOutput, lineThree );
                addOutput( fileOutputNode, currentLine );

                copyString( currentLine->lineOfOutput, lineFour );
                addOutput( fileOutputNode, currentLine );
            }

        free( currentLine );
    }

//A bit of future proofing for when we change to waiting instead of just end
void changeState( ConfigDataType *configPtr, PCBType *currentPCB, char* state, fileOutputType *fileOutputNode )
    {
        // Initialize Variable
        fileOutputType *currentLine = (fileOutputType *)malloc( sizeof( fileOutputType ) );

        char timerTime[ STD_STR_LEN ];
        char outputString[ STD_STR_LEN ];

        copyString( currentPCB->state, state );

        accessTimer(LAP_TIMER, timerTime);
        sprintf(outputString, "%s, OS: Process %d set to %s\n", timerTime, currentPCB->pid, currentPCB->state );

        copyString( currentLine->lineOfOutput, outputString);

         if( configPtr->logToCode == LOGTO_MONITOR_CODE ||
            configPtr->logToCode == LOGTO_BOTH_CODE)
            {
                printf(outputString);
            }

        if( configPtr->logToCode == LOGTO_FILE_CODE ||
            configPtr->logToCode == LOGTO_BOTH_CODE)
            {
                addOutput( fileOutputNode, currentLine );
            }

        free( currentLine );
    }

/*
Function Name: addOuput
Algorithm: adds line node to end of linked list
*/
fileOutputType *addOutput( fileOutputType *localPtr, fileOutputType *newNode )
    {
    
        // check for local pointer assigned to null
        if( localPtr == NULL )
            {

                // access memory for new link/node
                    // function: malloc
                localPtr = (fileOutputType *)malloc( sizeof( fileOutputType ) );

                // Set fields
                copyString( localPtr->lineOfOutput, newNode->lineOfOutput );
                localPtr->nextLine = NULL;

                // return current local pointer
                return localPtr;
            }

        // assume end of list not found yet
        // assign recursive function to current's next link
            // function: addNode
        localPtr->nextLine = addOutput( localPtr->nextLine, newNode );

        // return current local pointer
        return localPtr;
    }

//Methods to free generated linked lists
PCBType *clearPCBList( PCBType *localPtr )
    {
        // check for local pointer not set to null (list not empty)
        if( localPtr != NULL )
            {
            
                // call recursive function with next pointer
                    // function: clearMetaDataList
                clearPCBList( localPtr->nextNode );

                // after recursive call, release memory to OS
                    // function: free
                free( localPtr );

                // set local pointer to null
                localPtr = NULL;

            }

        // return null to calling function
        return NULL;
    }

fileOutputType *clearFileOutputList( fileOutputType *localPtr )
    {
        // check for local pointer not set to null (list not empty)
        if( localPtr != NULL )
            {
            
                // call recursive function with next pointer
                    // function: clearMetaDataList
                clearFileOutputList( localPtr->nextLine );

                // after recursive call, release memory to OS
                    // function: free
                free( localPtr );

                // set local pointer to null
                localPtr = NULL;

            }

        // return null to calling function
        return NULL;
    }

