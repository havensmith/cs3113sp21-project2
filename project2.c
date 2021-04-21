#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

struct Instruction
{
    char command[10];
    char subcommand[10];
    char ID[17];
    int size;
};

struct MemoryBlock
{
    char ID[17];
    int size;
};

struct Memory
{
    int size;
    struct MemoryBlock blocks[10000];
};

char algorithim[10];
int totalMemory;

int instructionCount = 0;
struct Instruction instructions[10000];

struct Memory memory;

void loadInstructions(FILE *file)
{
    struct Instruction instruction;
    char string[10];
    char buffer[100];

    while (fscanf(file, "%s", string) != EOF)
    {
        strcpy(instruction.command, string);
        if (strcmp(instruction.command, "#") == 0) //skip comments in input
        {
            fgets(buffer, 100, file);
        }
        else if (strcmp(instruction.command, "REQUEST") == 0)
        {
            fscanf(file, "%s", string);
            strcpy(instruction.ID, string);
            fscanf(file, "%s", string);
            instruction.size = atoi(string);
        }
        else if (strcmp(string, "RELEASE") == 0 || strcmp(string, "FIND") == 0)
        {
            fscanf(file, "%s", string);
            strcpy(instruction.ID, string);
        }
        else
        {
            fscanf(file, "%s", string);
            strcpy(instruction.subcommand, string);
        }

        if (strcmp(instruction.command, "#") != 0)
        {
            instructions[instructionCount] = instruction;
            instructionCount++;
        }
    }
}

void printBlocks() //debug method
{
    int sum = 0;
    for (int i = 0; i < memory.size; i++)
    {
        printf("%d %s - %d\n", i, memory.blocks[i].ID, memory.blocks[i].size);
        sum += memory.blocks[i].size;
    }
}

void consolidate() //method to join empty space
{
    bool consolidated = false;
    while (consolidated == false)
    {
        consolidated = true;
        for (int i = 0; i < memory.size - 1; i++) //traverse
        {
            if (strcmp(memory.blocks[i].ID, "EMPTY") == 0 && strcmp(memory.blocks[i].ID, memory.blocks[i + 1].ID) == 0) //search for consecutive emptyspace
            {
                consolidated = false;
                memory.blocks[i + 1].size += memory.blocks[i].size;
                for (int j = i; j < memory.size; j++) //delete memory block (it is combined)
                {
                    strcpy(memory.blocks[j].ID, memory.blocks[j + 1].ID);
                    memory.blocks[j].size = memory.blocks[j + 1].size;
                }
            }
        }
    }
}

void runInstructions()
{
    strcpy(memory.blocks[0].ID, "EMPTY");
    memory.blocks[0].size = totalMemory;
    memory.size = 1;
    int nextFitIndex = 0;

    for (int i = 0; i < instructionCount; i++)
    {
        if (strcmp(instructions[i].command, "REQUEST") == 0)
        {
            bool allocated = false;
            if (strcmp(algorithim, "FIRSTFIT") == 0)
            {
                for (int j = 0; j < memory.size && allocated == false; j++)
                {
                    if (strcmp(memory.blocks[j].ID, "EMPTY") == 0 && memory.blocks[j].size >= instructions[i].size) //find a fit
                    {
                        //insert memory block before empty space
                        for (int k = memory.size - 1; k >= j; k--)
                        {
                            strcpy(memory.blocks[k + 1].ID, memory.blocks[k].ID);
                            memory.blocks[k + 1].size = memory.blocks[k].size;
                        }
                        strcpy(memory.blocks[j].ID, instructions[i].ID);
                        memory.blocks[j].size = instructions[i].size;
                        memory.size++;

                        //shrink empty space
                        memory.blocks[j + 1].size -= instructions[i].size;
                        allocated = true;
                        int memoryLocation = 0;
                        for (int k = 0; k < j; k++)
                        {
                            memoryLocation += memory.blocks[k].size;
                        }
                        printf("ALLOCATED %s %d\n", instructions[i].ID, memoryLocation);
                    }
                }
                if (allocated == false)
                {
                    printf("FAIL REQUEST %s %d\n", instructions[i].ID, instructions[i].size);
                }
            }
            else if (strcmp(algorithim, "BESTFIT") == 0)
            {
                int bestSize = 2147483647; //largest possible integer value
                int bestIndex;
                for (int j = 0; j < memory.size; j++)
                {
                    if (strcmp(memory.blocks[j].ID, "EMPTY") == 0 && memory.blocks[j].size >= instructions[i].size) //find good fits
                    {
                        if (memory.blocks[j].size < bestSize) // keep track of best fit
                        {
                            bestSize = memory.blocks[j].size;
                            bestIndex = j;
                        }
                    }
                }
                //insert memory block before empty space
                for (int k = memory.size - 1; k >= bestIndex; k--)
                {
                    strcpy(memory.blocks[k + 1].ID, memory.blocks[k].ID);
                    memory.blocks[k + 1].size = memory.blocks[k].size;
                }
                strcpy(memory.blocks[bestIndex].ID, instructions[i].ID);
                memory.blocks[bestIndex].size = instructions[i].size;
                memory.size++;

                //shrink empty space
                memory.blocks[bestIndex + 1].size -= instructions[i].size;
                allocated = true;
                int memoryLocation = 0;
                for (int k = 0; k < bestIndex; k++)
                {
                    memoryLocation += memory.blocks[k].size;
                }
                printf("ALLOCATED %s %d\n", instructions[i].ID, memoryLocation);
            }
            else if (strcmp(algorithim, "WORSTFIT") == 0)
            {
                int bestSize = 0;
                int bestIndex; //keeps track of largest memory
                for (int j = 0; j < memory.size; j++)
                {
                    if (strcmp(memory.blocks[j].ID, "EMPTY") == 0 && memory.blocks[j].size >= instructions[i].size) //find acceptable fits
                    {
                        if (memory.blocks[j].size > bestSize)
                        {
                            bestSize = memory.blocks[j].size;
                            bestIndex = j;
                        }
                    }
                }
                //insert memory block before empty space
                for (int k = memory.size - 1; k >= bestIndex; k--)
                {
                    strcpy(memory.blocks[k + 1].ID, memory.blocks[k].ID);
                    memory.blocks[k + 1].size = memory.blocks[k].size;
                }
                strcpy(memory.blocks[bestIndex].ID, instructions[i].ID);
                memory.blocks[bestIndex].size = instructions[i].size;
                memory.size++;

                //shrink empty space
                memory.blocks[bestIndex + 1].size -= instructions[i].size;
                allocated = true;
                int memoryLocation = 0;
                for (int k = 0; k < bestIndex; k++)
                {
                    memoryLocation += memory.blocks[k].size;
                }
                printf("ALLOCATED %s %d\n", instructions[i].ID, memoryLocation);
            }
            else if (strcmp(algorithim, "NEXTFIT") == 0)
            {
                for (int j = nextFitIndex; (j < memory.size + nextFitIndex) && allocated == false; j++) //traverse
                {
                    int index = j % memory.size;                                                                            // modulus to loop array
                    if (strcmp(memory.blocks[index].ID, "EMPTY") == 0 && memory.blocks[index].size >= instructions[i].size) //search for acceptable fits
                    {
                        //insert memory block before empty space
                        for (int k = memory.size - 1; k >= index; k--)
                        {
                            strcpy(memory.blocks[k + 1].ID, memory.blocks[k].ID);
                            memory.blocks[k + 1].size = memory.blocks[k].size;
                        }
                        strcpy(memory.blocks[index].ID, instructions[i].ID);
                        memory.blocks[index].size = instructions[i].size;
                        memory.size++;

                        //shrink empty space
                        memory.blocks[index + 1].size -= instructions[i].size;
                        allocated = true;
                        int memoryLocation = 0;
                        for (int k = 0; k < index; k++)
                        {
                            memoryLocation += memory.blocks[k].size;
                        }
                        nextFitIndex = index;
                        printf("ALLOCATED %s %d\n", instructions[i].ID, memoryLocation);
                    }
                }
            }
            if(allocated == false) {
                printf("FAIL REQUEST %s %d\n", instructions[i].ID, instructions[i].size);
            }
        }
        else if (strcmp(instructions[i].command, "RELEASE") == 0) // "release" command
        {
            int freedMemory;
            int memoryLocation = 0;
            bool found = false;
            for (int j = 0; j < memory.size && found == false; j++) // traverse
            {
                if (strcmp(memory.blocks[j].ID, instructions[i].ID) == 0) //find ID
                {
                    found = true;
                    strcpy(memory.blocks[j].ID, "EMPTY"); //release memory
                    freedMemory = memory.blocks[j].size;
                }
                else
                {
                    memoryLocation += memory.blocks[j].size; //track location
                }
                consolidate(); //combine empty memory
            }
            if (found == false)
            {
                printf("FAIL RELEASE %s\n", instructions[i].ID);
            }
            else
            {
                printf("FREE %s %d %d\n", instructions[i].ID, freedMemory, memoryLocation);
            }
        }

        else if (strcmp(instructions[i].command, "LIST") == 0) // "list" command
        {
            if (strcmp(instructions[i].subcommand, "AVAILABLE") == 0) // "list available"
            {
                int memoryLocation = 0;
                for (int j = 0; j < memory.size; j++) //traverse memory
                {
                    if (strcmp(memory.blocks[j].ID, "EMPTY") == 0) //look for unallocated memory
                    {
                        printf("(%d, %d) ", memory.blocks[j].size, memoryLocation);
                    }
                    memoryLocation += memory.blocks[j].size; //keep track of location
                }
                printf("\n");
            }
            if (strcmp(instructions[i].subcommand, "ASSIGNED") == 0) // "list assigned"
            {
                int memoryLocation = 0;
                for (int j = 0; j < memory.size; j++) //traverse
                {
                    if (strcmp(memory.blocks[j].ID, "EMPTY") != 0) //look for unassigned (nonempty)
                    {
                        printf("(%s, %d, %d) ", memory.blocks[j].ID, memory.blocks[j].size, memoryLocation);
                    }
                    memoryLocation += memory.blocks[j].size; //track location
                }
                printf("\n");
            }
        }
        else if (strcmp(instructions[i].command, "FIND") == 0) //"find" command
        {
            int memoryLocation = 0;
            bool found = false;
            for (int j = 0; j < memory.size && found == false; j++) //traverse memory
            {
                if (strcmp(memory.blocks[j].ID, instructions[i].ID) == 0) //look for ID
                {
                    found = true;
                    printf("(%s, %d, %d) ", memory.blocks[j].ID, memory.blocks[j].size, memoryLocation);
                }
                memoryLocation += memory.blocks[j].size; //keep track of location
            }
            if (found == false) //not found error
            {
                printf("FAULT");
            }
            printf("\n");
        }
    }
}

int main(int argc, char **argv)
{
    strcpy(algorithim, argv[1]);
    totalMemory = atoi(argv[2]);
    FILE *file = fopen(argv[3], "r");
    loadInstructions(file);
    runInstructions();
    //printBlocks();
    return 0;
}