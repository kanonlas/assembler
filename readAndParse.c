/////////////////////////////////////////////// template Teacher //////////////////////////////////////////////////////
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define MAXLINELENGTH 1000

int readAndParse(FILE *, char *, char *, char *, char *, char *);  // function อ่านบรรทัดไฟล์ 
int isNumber(char *); //ตรวจสอบว่าข้อความเป็นตัวเลขไหม

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



while (readAndParse(inFilePtr, label, opcode, arg0, arg1, arg2))
{
    /* test the opcode */
    if (!strcmp(opcode, "add")) { // บวก ค่าใน regA ด้วยค่าใน regB และเอาไปเก็บใน destReg
       
    }

}

    
    
    // rewind(inFilePtr); //อ่านไฟล์อีกรอบ

    
    return(0);

}

///////////////////////////////////////////////////////////////////////////////////////////////////////
/*
 * Read and parse a line of the assembly-language file.  Fields are returned
 * in label, opcode, arg0, arg1, arg2 (these strings must have memory already
 * allocated to them).
 *
 * Return values:
 *     0 if reached end of file
 *     1 if all went well
 *
 * exit(1) if line is too long.
 */

int readAndParse(FILE *inFilePtr, char *label, char *opcode, char *arg0,
    char *arg1, char *arg2)
{
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
