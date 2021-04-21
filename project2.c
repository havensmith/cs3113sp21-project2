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
    struct MemoryBlock blocks[200];
};

char algorithim[20];
int totalMemory;

int instructionCount = 0;
struct Instruction instructions[200];

struct Memory memory;


void loadInstructions(FILE *file)
{
    struct Instruction instruction;
    char string[10];
    char buffer[100];

    while (fscanf(file, "%s", string) != EOF)
    {
        strcpy(instruction.command, string);
        if (strcmp(instruction.command, "#") == 0) {
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

        if (strcmp(instruction.command, "#") != 0) {
            instructions[instructionCount] = instruction;
            instructionCount++;
        }
        
    }
}

void printBlocks() {
    int sum = 0;
    for(int i = 0; i < memory.size; i++) {
        printf("%d %s - %d\n", i, memory.blocks[i].ID, memory.blocks[i].size);
        sum+=memory.blocks[i].size;
    }
    printf("%d\n", sum);
}

void consolidate() {
    bool consolidated = false;
    while(consolidated == false) {
        consolidated = true;
        for(int i = 0; i < memory.size - 1; i++) {
            if(strcmp(memory.blocks[i].ID, "EMPTY") == 0 && strcmp(memory.blocks[i].ID, memory.blocks[i+1].ID) == 0) {
                consolidated = false;
                memory.blocks[i+1].size += memory.blocks[i].size;
                for(int j = i; j < memory.size; j++) {
                    strcpy(memory.blocks[j].ID, memory.blocks[j+1].ID);
                    memory.blocks[j].size = memory.blocks[j+1].size;
                }
            }
        }
    }
    
}

void runInstructions() {
    memory.size = 0;
    strcpy(memory.blocks[memory.size].ID, "EMPTY");
    memory.blocks[memory.size].size = totalMemory;
    memory.size ++;
    int nextFitIndex = 0;

    for(int i = 0; i < instructionCount; i++) {
        if(strcmp(instructions[i].command, "REQUEST") == 0) {
            bool allocated = false;
            if(strcmp(algorithim, "FIRSTFIT") == 0) {
                for(int j = 0; j < memory.size && allocated == false; j++){
                    if(strcmp(memory.blocks[j].ID, "EMPTY") == 0 && memory.blocks[j].size >= instructions[i].size) {
                        //insert memory block before empty space
                        for(int k = memory.size - 1; k >= j; k--) {
                            strcpy(memory.blocks[k+1].ID, memory.blocks[k].ID);
                            memory.blocks[k+1].size = memory.blocks[k].size;
                        }
                        strcpy(memory.blocks[j].ID, instructions[i].ID);
                        memory.blocks[j].size = instructions[i].size;
                        memory.size ++;

                        //shrink empty space
                        memory.blocks[j+1].size -= instructions[i].size;
                        allocated = true;
                        int memoryLocation = 0;
                        for(int k = 0; k < j; k++) {
                            memoryLocation += memory.blocks[k].size;
                        }
                        printf("ALLOCATED %s %d\n", instructions[i].ID, memoryLocation);
                    }
                }
                if(allocated == false) {
                    printf("FAIL REQUEST %s %d\n", instructions[i].ID, instructions[i].size);
                }
            }
            else if(strcmp(algorithim, "BESTFIT") == 0) {
                int bestSize = 2147483647;
                int bestIndex;
                for(int j = 0; j < memory.size; j++) {
                    if(strcmp(memory.blocks[j].ID, "EMPTY") == 0 && memory.blocks[j].size >= instructions[i].size) {
                        if(memory.blocks[j].size < bestSize){
                            bestSize = memory.blocks[j].size;
                            bestIndex = j;
                        }

                    }
                }
                //insert memory block before empty space
                for(int k = memory.size - 1; k >= bestIndex; k--) {
                    strcpy(memory.blocks[k+1].ID, memory.blocks[k].ID);
                    memory.blocks[k+1].size = memory.blocks[k].size;
                }
                strcpy(memory.blocks[bestIndex].ID, instructions[i].ID);
                memory.blocks[bestIndex].size = instructions[i].size;
                memory.size ++;

                //shrink empty space
                memory.blocks[bestIndex + 1].size -= instructions[i].size;
                allocated = true;
                int memoryLocation = 0;
                for(int k = 0; k < bestIndex; k++) {
                    memoryLocation += memory.blocks[k].size;
                }
                printf("ALLOCATED %s %d\n", instructions[i].ID, memoryLocation);
            }
            else if(strcmp(algorithim, "WORSTFIT") == 0) {
                int bestSize = 0;
                int bestIndex;
                for(int j = 0; j < memory.size; j++) {
                    if(strcmp(memory.blocks[j].ID, "EMPTY") == 0 && memory.blocks[j].size >= instructions[i].size) {
                        if(memory.blocks[j].size > bestSize){
                            bestSize = memory.blocks[j].size;
                            bestIndex = j;
                        }

                    }
                }
                //insert memory block before empty space
                for(int k = memory.size - 1; k >= bestIndex; k--) {
                    strcpy(memory.blocks[k+1].ID, memory.blocks[k].ID);
                    memory.blocks[k+1].size = memory.blocks[k].size;
                }
                strcpy(memory.blocks[bestIndex].ID, instructions[i].ID);
                memory.blocks[bestIndex].size = instructions[i].size;
                memory.size ++;

                //shrink empty space
                memory.blocks[bestIndex + 1].size -= instructions[i].size;
                allocated = true;
                int memoryLocation = 0;
                for(int k = 0; k < bestIndex; k++) {
                    memoryLocation += memory.blocks[k].size;
                }
                printf("ALLOCATED %s %d\n", instructions[i].ID, memoryLocation);
            }
            else if(strcmp(algorithim, "NEXTFIT") == 0) {
                for(int j = nextFitIndex; (j < memory.size + nextFitIndex) && allocated == false; j++) {
                    int index = j % memory.size;
                    if(strcmp(memory.blocks[index].ID, "EMPTY") == 0 && memory.blocks[index].size >= instructions[i].size) {
                        //insert memory block before empty space
                        for(int k = memory.size - 1; k >= index; k--) {
                            strcpy(memory.blocks[k+1].ID, memory.blocks[k].ID);
                            memory.blocks[k+1].size = memory.blocks[k].size;
                        }
                        strcpy(memory.blocks[index].ID, instructions[i].ID);
                        memory.blocks[index].size = instructions[i].size;
                        memory.size ++;

                        //shrink empty space
                        memory.blocks[index+1].size -= instructions[i].size;
                        allocated = true;
                        int memoryLocation = 0;
                        for(int k = 0; k < index; k++) {
                            memoryLocation += memory.blocks[k].size;
                        }
                    }
                }
            }
        }
        else if(strcmp(instructions[i].command, "RELEASE") == 0) {
            int freedMemory;
            int memoryLocation = 0;
            bool found = false;
            for(int j = 0; j < memory.size && found == false; j++) {
                if(strcmp(memory.blocks[j].ID, instructions[i].ID) == 0) {
                    found = true;
                    strcpy(memory.blocks[j].ID, "EMPTY");
                    freedMemory = memory.blocks[j].size;
                }
                else {
                    memoryLocation += memory.blocks[j].size;
                }
                consolidate();
            }
            printf("FREE %s %d %d\n", instructions[i].ID, freedMemory, memoryLocation);
        }
        else if(strcmp(instructions[i].command, "LIST") == 0) {
            if(strcmp(instructions[i].subcommand, "AVAILABLE") == 0) {
                int memoryLocation = 0;
                for(int j = 0; j < memory.size; j++) {
                    if(strcmp(memory.blocks[j].ID, "EMPTY") == 0) {
                        printf("(%d, %d) ", memory.blocks[j].size, memoryLocation);
                    }
                    memoryLocation += memory.blocks[j].size;
                }
                printf("\n");
            }
            if(strcmp(instructions[i].subcommand, "ASSIGNED") == 0) {
                int memoryLocation = 0;
                for(int j = 0; j < memory.size; j++) {
                    if(strcmp(memory.blocks[j].ID, "EMPTY") != 0) {
                        printf("(%s, %d, %d) ", memory.blocks[j].ID, memory.blocks[j].size, memoryLocation);
                    }
                    memoryLocation += memory.blocks[j].size;
                }
                printf("\n");
            }
        }
        else if(strcmp(instructions[i].command, "FIND") == 0) {
            int memoryLocation = 0;
            bool found = false;
            for(int j = 0; j < memory.size && found == false; j++) {
                if(strcmp(memory.blocks[j].ID, instructions[i].ID) == 0) {
                    found = true;
                    printf("(%s, %d, %d) ", memory.blocks[j].ID, memory.blocks[j].size, memoryLocation);
                }
                memoryLocation += memory.blocks[j].size;
            }
            if(found == false) {
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