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


/* head - function */
//อ่านบรรทัดไฟล์
    int readAndParse(FILE *, char *, char *, char *, char *, char *);  
//ตรวจสอบว่าข้อความเป็นตัวเลขไหม
    int isNumber(char *); 
//เปลี่ยนคำเป็นเลข
    string convertToBinary( char*);



/* main function */
int main(int argc, char *argv[])
{
    char *inFileString, *outFileString;
    FILE *inFilePtr, *outFilePtr;
    char label[MAXLINELENGTH], opcode[MAXLINELENGTH], arg0[MAXLINELENGTH],
         arg1[MAXLINELENGTH], arg2[MAXLINELENGTH];

    if (argc != 3) {  // argc = argument count (ชื่อโปรเเกรม, ชื่อไฟล์ assembly, ชื่อไฟล์ machine code)
        printf("error: usage: %s <assembly-code-file> <machine-code-file>\n",
            argv[0]);
        exit(1);   // ถ้าไม่ถูกกระโดด exit เลย 
    }

    inFileString = argv[1];
    outFileString = argv[2];

    inFilePtr = fopen(inFileString, "r"); // เปิดไฟล์ Input เพื่ออ่าน 
        if (inFilePtr == NULL) { // ไฟล์ว่าง exit 
            printf("error in opening %s\n", inFileString);
            exit(1); 
        }
    outFilePtr = fopen(outFileString, "w"); // output file เพื่อ เขียน
        if (outFilePtr == NULL) {
            printf("error in opening %s\n", outFileString);
            exit(1);
        }

    std::string opcodeNum;
    // labelNum, opcodeNum, arg0Num, arg1Num, arg2Num;  //เอาไว้เก็บเลขเอาไปใช้ต่อ 

    while (readAndParse(inFilePtr, label, opcode, arg0, arg1, arg2))
    {
        //arg0 = dest 
        //arg1 = rs1
        //arg2 = rs2

        /* Gutto's part */
         if (strcmp(opcode, "add") == 0) {
        int instructionAdd = 0;
        int regA = atoi(arg0);
        int regB = atoi(arg1);
        int destReg = atoi(arg2);
        int op = getOpcode(opcode);
        instructionAdd |= (op << 22); // Set opcode
        instructionAdd |= (regA << 19) | (regB << 16) | destReg; // Mask destReg to ensure it fits in 3 bits
        fprintf(outFilePtr, "%d\n", instructionAdd);
    } else if (strcmp(opcode, "nand") == 0) {
        int instructionNand = 0;
        int regA = atoi(arg0);
        int regB = atoi(arg1);
        int destReg = atoi(arg2);
        int op = getOpcode(opcode);
        instructionNand |= (op << 22); // Set opcode
        instructionNand |= (regA << 19) | (regB << 16) | (destReg << 13) ; // Mask destReg to ensure it fits in 3 bits
        fprintf(outFilePtr, "%d\n", instructionNand);
    
        }if (!strcmp(opcode, "lw")) { //Load regB จาก memory และ memory address หาได้จากการเอา offsetField บวกกับค่าใน regA
        
        }if (!strcmp(opcode, "sw")) { //Store regB ใน memory และ memory address หาได้จากการเอา offsetField บวกกับค่าใน regA
        
        }
        
        /* Kaewtar's part */

        if (!strcmp(opcode, "beq")) { // ถ้า ค่าใน regA เท่ากับค่าใน regB ให้กระโดดไปที่ address PC+1+offsetField ซึ่ง PC คือ address ของ beq instruction
        
        }if (!strcmp(opcode, "jalr")) { //เก็บค่า PC+1 ไว้ใน regB ซึ่ง PC คือ address ของ jalr instruction และกระโดดไปที่ address ที่ถูกเก็บไว้ใน regA แต่ถ้า regA และ regB คือ register ตัวเดียวกัน ให้เก็บ PC+1 ก่อน และค่อยกระโดดไปที่ PC+1
        
        }if (!strcmp(opcode, "halr")) { //เพิ่มค่า PC เหมือน instructions อื่นๆ และ halt เครื่อง นั่นคือให้ simulator รู้ว่าเครื่องมีการ halted เกิดขึ้น
        
        }if (!strcmp(opcode, "noop")) { //เพิ่มค่า PC เหมือน instructions อื่นๆ และ halt เครื่อง นั่นคือให้ simulator รู้ว่าเครื่องมีการ halted เกิดขึ้น
        
        }
        
    }

    // rewind(inFilePtr); //อ่านไฟล์อีกรอบ    
    return(0);

}



/* detail - function */
int readAndParse(FILE *inFilePtr, char *label, char *opcode, char *arg0,char *arg1, char *arg2){
    char line[MAXLINELENGTH];
    char *ptr = line;

    /* delete prior values */
        label[0] = opcode[0] = arg0[0] = arg1[0] = arg2[0] = '\0';

    /* read the line from the assembly-language file */
        if (fgets(line, MAXLINELENGTH, inFilePtr) == NULL) {
	        /* reached end of file */
            return(0);
        }

    /* check for line too long (by looking for a \n) */
        if (strchr(line, '\n') == NULL) {
            /* line too long */
            printf("error: line too long\n");
            exit(1);
        }

    /* is there a label? */
        ptr = line;
        if (sscanf(ptr, "%[^\t\n ]", label)) {
	        /* successfully read label; advance pointer over the label */
            ptr += strlen(label);
        }

    /*
     * Parse the rest of the line.  Would be nice to have real regular
     * expressions, but scanf will suffice.
     */
        sscanf(ptr, "%*[\t\n ]%[^\t\n ]%*[\t\n ]%[^\t\n ]%*[\t\n ]%[^\t\n ]%*[\t\n ]%[^\t\n ]",
            opcode, arg0, arg1, arg2);
        return(1);
}

int isNumber(char *string)
{
    /* return 1 if string is a number */
    int i;
    return( (sscanf(string, "%d", &i)) == 1);
}
