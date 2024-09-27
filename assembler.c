#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

#define MAXLINELENGTH 1000
#define MAXLABELS 100
#define LABEL_LENGTH 7
#define MEMORY_SIZE 65536
#define NUM_REGS 8

typedef struct {
    char label[LABEL_LENGTH];
    int address;
} Label;

Label labels[MAXLABELS];
int labelCount = 0;

int readAndParse(FILE *, char *, char *, char *, char *, char *);
int isNumber(char *);
int findLabelAddress(char *);
void addLabel(char *, int);
int getOpcode(char *);
void generateMachineCode(FILE *, char *, char *, char *, char *, char *, int);
int toOffset(int, int);

int main(int argc, char *argv[]) {
    char *inFileString, *outFileString;
    FILE *inFilePtr, *outFilePtr;
    char label[MAXLINELENGTH], opcode[MAXLINELENGTH], arg0[MAXLINELENGTH], arg1[MAXLINELENGTH], arg2[MAXLINELENGTH];
    int address = 0;

    if (argc != 3) {
        printf("error: usage: %s <assembly-code-file> <machine-code-file>\n", argv[0]);
        exit(1);
    }

    inFileString = argv[1];
    outFileString = argv[2];

    inFilePtr = fopen(inFileString, "r");
    if (inFilePtr == NULL) {
        printf("error in opening %s\n", inFileString);
        exit(1);
    }

    // First pass: Collect labels
    while (readAndParse(inFilePtr, label, opcode, arg0, arg1, arg2)) {
        if (strlen(label) > 0) {
            addLabel(label, address);
        }
        address++;
    }

    // Reset file pointer
    rewind(inFilePtr);
    address = 0;
    outFilePtr = fopen(outFileString, "w");
    if (outFilePtr == NULL) {
        printf("error in opening %s\n", outFileString);
        exit(1);
    }

    // Second pass: Generate machine code
    while (readAndParse(inFilePtr, label, opcode, arg0, arg1, arg2)) {
        generateMachineCode(outFilePtr, opcode, arg0, arg1, arg2, label, address);
        address++;
    }

    fclose(inFilePtr);
    fclose(outFilePtr);
    return 0;
}

int isNumber(char *str) {
    for (int i = 0; str[i] != '\0'; i++) {
        if (!isdigit(str[i]) && str[i] != '-') {
            return 0; // Not a number
        }
    }
    return 1; // Is a number
}

// Read a long line from the input file
int readLongLine(FILE *inFilePtr, char *line) {
    char buffer[MAXLINELENGTH];
    line[0] = '\0'; // Clear the initial value

    while (fgets(buffer, MAXLINELENGTH, inFilePtr)) {
        strcat(line, buffer); // Concatenate lines
        if (buffer[strlen(buffer) - 1] == '\n') {
            break; // Stop if there's a newline
        }
    }

    return strlen(line) > 0; // If there's data in line
}

// Read and parse a line of assembly code
int readAndParse(FILE *inFilePtr, char *label, char *opcode, char *arg0, char *arg1, char *arg2) {
    char line[MAXLINELENGTH];
    char *ptr = line;

    // Clear previous values
    label[0] = opcode[0] = arg0[0] = arg1[0] = arg2[0] = '\0';

    if (!readLongLine(inFilePtr, line)) return 0;

    // Check if the line is too long
    if (strlen(line) >= MAXLINELENGTH) {
        printf("error: line too long\n");
        exit(1);
    }

    ptr = line;


    // Check for label
    if (sscanf(ptr, "%[^\t\n ]", label)) {
        ptr += strlen(label);
        // Trim whitespace
        while (*ptr == ' ' || *ptr == '\t') ptr++;
    } else {
        label[0] = '\0'; // No label
    }

    // Parse opcode and arguments
    sscanf(ptr, "%s %s %s %s", opcode, arg0, arg1, arg2);

    // Debugging: ตรวจสอบค่าที่อ่านออกมา
    //printf("Parsed: '%s', '%s','%s', '%s','%s'\n", label, opcode, arg0, arg1, arg2);

    return 1;
}

// Add a label to the label table
void addLabel(char *label, int address) {
    if (labelCount >= MAXLABELS) {
        printf("error: too many labels\n");
        exit(1);
    }
    strcpy(labels[labelCount].label, label);
    labels[labelCount].address = address;
    labelCount++;
    printf("Added label: %s at address: %d\n", label, address); // Debugging
}

// Find the address of a label
int findLabelAddress(char *label) {
    for (int i = 0; i < labelCount; i++) {
        if (strcmp(labels[i].label, label) == 0) {
            return labels[i].address;
        }
    }
    printf("error: undefined label '%s'\n", label);
    exit(1);
}

// Convert opcode to binary representation
int getOpcode(char *opcode) {
    if (strcmp(opcode, "add") == 0) return 0b000;
    if (strcmp(opcode, "nand") == 0) return 0b001;
    if (strcmp(opcode, "lw") == 0) return 0b010;
    if (strcmp(opcode, "sw") == 0) return 0b011;
    if (strcmp(opcode, "beq") == 0) return 0b100;
    if (strcmp(opcode, "jalr") == 0) return 0b101;
    if (strcmp(opcode, "halt") == 0) return 0b110;
    if (strcmp(opcode, "noop") == 0) return 0b111;
    printf("error: invalid opcode %s\n", opcode);
    exit(1);
}

// Generate machine code for the given assembly instruction
void generateMachineCode(FILE *outFilePtr, char *opcode, char *arg0, char *arg1, char *arg2, char *label, int address) {

    // Handle .fill command
    if (strcmp(opcode, ".fill") == 0) {
        int value;
        if (isNumber(arg0)) {
            value = atoi(arg0);  // If it's a number, use it directly
        } else {
            value = findLabelAddress(arg0);  // If it's a label, use its address
        }
        fprintf(outFilePtr, "%d\n", value);  // Write value to output file
        return;  // End processing .fill
    }
     // Debugging: ตรวจสอบค่าที่ได้รับ
   // printf("Generating machine code for  %s, %s %s %s, %s\n", opcode, arg0, arg1, arg2, label);

    // R-type instructions
    // if (strcmp(opcode, "add") == 0 || strcmp(opcode, "nand") == 0) {
    //     int regA = atoi(arg0);
    //     int regB = atoi(arg1);
    //     int destReg = atoi(arg2);
    //     instruction |= (regA << 19) | (regB << 16) | destReg;
    //     instruction &= 0xFFFFFFFC; // Clear bits 2-0
    // }
    printf("%s\n",opcode);

    if (!strcmp(opcode, "add")) {
        int instructionAdd = 0;
        int regA = atoi(arg0);
        int regB = atoi(arg1);
        int destReg = atoi(arg2);
        int op = getOpcode(opcode);
        instructionAdd |= (op << 22); 
        instructionAdd |= (regA << 19) | (regB << 16) | destReg;
        fprintf(outFilePtr, "%d\n", instructionAdd);
    } else if(!strcmp(opcode, "nand")){
        int instruction = 0;
        int op = getOpcode(opcode);
        int regA = atoi(arg0);
        int regB = atoi(arg1);
        int destReg = atoi(arg2);

        instruction |=(op << 22) | (regA << 19) | (regB << 16) | destReg;

        if(!isNumber(arg0) || !isNumber(arg1)|| !isNumber(arg2)){
            exit(1);
        }

        fprintf(outFilePtr, "%d\n", instruction);
    }  else if (!strcmp(opcode, "lw")) { 
        int instructionLOADW = 0;
        int op = getOpcode(opcode);
        int regA = atoi(arg0);
        int regB = atoi(arg1);

        if(arg2)
        int offset = atoi(arg2);
        instructionLOADW |= (op << 22) | (regA << 19) | (regB << 16) | offset;

        if( offset <= -32768 || offset >= 32767 ){ exit(1);}
        if(!isNumber(arg0) || !isNumber(arg1)/*|| !isNumber(arg2)*/){ exit(1);}
        if( offset )
        fprintf(outFilePtr, "%d\n", instructionLOADW);
    } else if(!strcmp(opcode, "beq")) {
        int opCode = getOpcode(opcode); 
        int regA = atoi(arg0);
        int regB = atoi(arg1);
        int offsetfield = isNumber(arg2) ? atoi(arg2) : (int)findLabelAddress(arg2)-(address+1);

        if( offsetfield < -32768 || offsetfield > 32767 ){ 
            exit(1);
        }

        int instruction = 0 | (opCode << 22) | (regA << 19) | (regB << 16) | (offsetfield & 0b1111111111111111);

        fprintf(outFilePtr, "%d\n", instruction);
    }

    // Write instruction to output file
    // Ensure you write the instruction after it has been constructed

    //  if(strcmp(opcode, "nand") == 001){
        
    //     int instruction = 0;
    //     int regA = atoi(arg0);
    //     int regB = atoi(arg1);
    //     int destReg = atoi(arg2);
    //     instruction |= (1 << 23) | (regA << 19) | (regB << 16) | (destReg & 0x7);
    //     if(!isNumber(arg0) || !isNumber(arg1)|| !isNumber(arg2)){
    //         exit(1);
    //     }
    // }

}


//     // I-type instructions
//     else if (strcmp(opcode, "lw") == 0 || strcmp(opcode, "sw") == 0 || strcmp(opcode, "beq") == 0) {
//         int regA = atoi(arg0);
//         int regB = atoi(arg1);
//         int offset;

//         if (strcmp(opcode, "beq") == 0) {
//             offset = findLabelAddress(arg2) - (instruction / 4 + 1); // PC + 1
//         } else {
//             offset = atoi(arg2);
//         }

//         instruction |= (regA << 19) | (regB << 16) | toOffset(offset, 16);
//     }
//     // J-type instructions
//     else if (strcmp(opcode, "jalr") == 0) {
//         int regA = atoi(arg0);
//         int regB = atoi(arg1);
//         instruction |= (regA << 19) | (regB << 16);
//     }

//     // Write instruction to output file
//     fprintf(outFilePtr, "%d\n", instruction);
// }

// // Convert a value to a 16-bit signed offset
// int toOffset(int value, int bits) {
//     if (value < -(1 << (bits - 1)) || value >= (1 << (bits - 1))) {
//         printf("error: offset out of range\n");
//         exit(1);
//     }
//     return value & ((1 << bits) - 1); // Mask to bits
// }
