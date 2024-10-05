----- convert assembly to machine code -----
run g++ assembler.txt -o <name> to create <name>.exe
<name>.exe require 2 input <name of assembly file> machineCode.txt

----- simulation -----
run g++ behav.txt -o <name2> to create <name2>.exe
name2.exe will read machineCode.txt file and write simulation result file name simulationOutput.txt
