/*run command
    ./assembler input.txt output.txt
    gcc readAndParse.cpp -o assembler

    g++ demoAssembler.c -o demoAssembler
    ./demoAssembler input2.txt output2.txt
*/

/*  input 1:
     add     1        2        1        
     beq     0        1        2        

    output 1:
        655361
        16842754

    input 2:
        start add 1 2 1
        nand 1 2 3
     beq 0   1     2
        beq 0 0    start 

    output 2:
            655361
            4849667
            16842754
            16842749

    input 3:
        start    add     1        2        1        
                beq     0        1        2        
                beq     0        0        start    

                noop

        done    halt                                 
        five      .fill      5
        neg1    .fill      -1

    output 3:
        655361
        16842754
        16842749
        29360128
        25165824
        5
        -1
*/