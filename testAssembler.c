/*run command
    -version 1- by Kanolas -
    ./assembler input.txt output.txt
    gcc readAndParse.cpp -o assembler

    -version 2- by Kaewtar -
    g++ demoAssembler.c -o demoAssembler
    ./demoAssembler input2.txt output2.txt

    -version 3 - by Thiranut -
        ถ้ามี assembler.exe แล้วก็
        assembler.exe mult.txt machineCode.txt
        g++  behav.txt -o behav
        behav.exe 
        ไฟล์ผลลัพธ์จะออกที่ simulationOutput.txt

    -version 4 - by Thiranut -
    1. g++ assembler.cpp -o ชื่อไรก็ได้มันจะได้ไฟล์ ชื่อนั้น.exe
    2. .\ชื่อ.exe ชื่อไฟล์ assemblyที่เขียน.txt machineCode.txt
    3. ไปกดรันไฟล์ behavior
    4. output ออกไฟล์ simulationOutput.txt

    *** warning ***
    1.ทำครั้งเดียวพอ
    2.ค่อยทำใหม่ถ้าโค้ด assembly เปลี่ยน
    ระวังเรื่องlabel ยาวเกิน7ตัวด้วยนะ

    -version 5 - by Kaewtar -
    g++ assembler.cpp -o assembler    
    ./assembler.exe mult.txt machineCode.txt
    ./behav behav.txt simulationOutput.txt
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