/*
 * c Standard : c11
 * date : 2020.04.20
 */
#include <stdlib.h>
#include <stdio.h>

int  *SAFESEQUENCE; // global pointer for safe-sequences array
int saveSequenceIndex = 0; // the index for the array


_Bool isRsrcAvailable(int id, int need[][3], int rsrcAvailable[]){
    // check : Are there enough resources for current process?
    for (int i = 0; i < 3; i++)
    {   // check each resource is adequate or not 
        if ( need[id][i] > rsrcAvailable[i] )
        {   // if it is not enough -> break loop -> return false
            return 0;
        }
    }
    return 1; // each resource is sufficient-> return true
}
/*
 * findSafeSequence is a recursive function that check each process.
 */
void findSafeSequence(int rsrcAllocation[][3], int need[][3], int rsrcAvailable[], _Bool flag[]){
    for (int i = 0; i < 4; i++) // loop to check each process
    { 
        if (!flag[i] && isRsrcAvailable(i, need, rsrcAvailable))
        {   // if the process is not finished(not flagged) and the resources are enough.
            flag[i] = 1; // finish the current process, and flag it as isFinished
            for (int j = 0; j < 3; j++)
            {
                rsrcAvailable[j] += rsrcAllocation[i][j]; // release all resources from current process
            }
            SAFESEQUENCE[saveSequenceIndex] = i+1; // record the order of process
            if (saveSequenceIndex != 4) saveSequenceIndex++; // move to next index

            findSafeSequence(rsrcAllocation, need, rsrcAvailable, flag); // recursive to find other order of process

            if(saveSequenceIndex > 0) saveSequenceIndex--; //for recursive , release index one by one
            flag[i] = 0; // release flag one by one
            for(int k = 0; k < 3; k++) rsrcAvailable[k] -= rsrcAllocation[i][k]; // claim back resources for recursive

        }
        
    }
    if (saveSequenceIndex == 3) // if the index reaches 3, that means we found a safe sequence!
    {
        for (int i = 0; i < 4; i++)
        {   // print out the order of sequence
            printf("P%d", SAFESEQUENCE[i]);
            if (i != 3) printf("->");
        }
        printf("\n");        
    }    
}

int main(int argc, char const *argv[])
{   
#pragma region initial input data
    int rsrcAllocation[4][3] = {
                                0, 1, 0,
                                2, 0, 0,
                                3, 0, 2,
                                2, 1, 1
        }; // resource allocation
    int max[4][3] = {
                     7, 5, 3,
                     3, 2, 2,
                     9, 0, 2,
                     2, 2, 2
    }; // maxima need
    int rsrcAvailable[3] = { 10, 5, 7 }; // resource
#pragma endregion initial input data

    int need[4][3] = { 0 }; // array to store ( max - allocation )
    _Bool isFinished[3] = { 0 }; // the finish state of each process

    SAFESEQUENCE = (int*)calloc( 4, sizeof(int)); // calloc a 4 int array
    
    // calculate need and resources that are available
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            need[i][j] = max[i][j] - rsrcAllocation[i][j];
            rsrcAvailable[j] -= rsrcAllocation[i][j];
        }
    }
    findSafeSequence(rsrcAllocation, need, rsrcAvailable, isFinished); // call the recursive function to find all safe sequences
    free(SAFESEQUENCE); // all things are done, release the memory
    return 0;
}

