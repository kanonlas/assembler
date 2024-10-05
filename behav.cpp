
#include <bits/stdc++.h>
using namespace std;

int BinarytoDecimal(string binary, int len, int i = 0)
{
    if (i == len - 1)
        return (binary[i] - '0');
    int temp = binary[i] - '0';
    temp = temp << (len - i - 1); // Shift the binary digit
    temp = temp + BinarytoDecimal(binary, len, i + 1);
    return temp;
}

string DecimalToBinary(int decimal)
{
    return bitset<32>(decimal).to_string();
}

string split(string bit, int startpos, int endpos)
{
    string splitword;
    for (int i = startpos; i <= endpos; i++)
    {
        splitword += bit[i];
    }
    return splitword;
}

int nandOperation(int regA, int regB)
{
    return ~(regA & regB);
}

int twoComplement(int num)
{
    if (num > 32767)
        return ~(num & 1) - 1;
    else
        return num;
}

void printform(ofstream &outFile, int pos, vector<int> mem, int reg[], int size){
    cout << "@@@\nstate:\n";
    cout << "\tpc " << pos + 1 << "\n";
    cout << "\tmemory:\n";

    for (int i = 0; i < size; ++i)
    {
        cout << "\t\tmem[ " << i << " ] " << mem[i] << "\n";
    }

    cout << "\tregisters:\n";

    for (int i = 0; i < 8; ++i)
    { // Assuming 8 registers as in your example
        cout << "\t\treg[ " << i << " ] " << reg[i] << "\n";
    }
    cout << "end state\n\n";
    outFile << "@@@\nstate:\n";
    outFile << "\tpc " << pos + 1 << "\n";
    outFile << "\tmemory:\n";

    for (int i = 0; i < size; ++i)
    {
        outFile << "\t\tmem[ " << i << " ] " << mem[i] << "\n";
    }

    outFile << "\tregisters:\n";

    for (int i = 0; i < 8; ++i)
    { // Assuming 8 registers as in your example
        outFile << "\t\treg[ " << i << " ] " << reg[i] << "\n";
    }
    outFile << "end state\n\n";
}

vector<int> readMachineCode(){
    ifstream machineCodeFile("machineCode.txt");
    vector<int> machineCode;
    string line;

    if (!machineCodeFile.is_open()) { 
        cerr << "Error opening the file!" << endl;
        exit(1);
    }

    while(getline(machineCodeFile, line)){ // read all machine code in file
        machineCode.push_back(stoi(line));
    }

    return machineCode;
}

void runMachineCode(ofstream &outFile, vector<int> machineCode)
{
    vector<int> address = machineCode;
    //int address.size() = address.size();
    int reg[8] = {0, 0, 0, 0, 0, 0, 0, 0};
    int inscount = 0;

    for (int i = 0; i < address.size(); ++i){
        cout << "memory[" << i << "]=" << address[i] << endl;
        outFile << "memory[" << i << "]=" << address[i] << endl;
    }
    
    cout << "\n\n";
    outFile << "\n\n";
    printform(outFile, -1, address, reg, address.size());
    for (int i = 0; i < address.size(); i++)
    {
        outFile << " Current line: " << i << endl;
        string bit = DecimalToBinary(address[i]);
        string opcode = split(bit, 7, 9);
        if (opcode == "000") // case:add
        {
            string rs = split(bit, 10, 12);
            string rt = split(bit, 13, 15);
            string rd = split(bit, 29, 31);
            int rsDec = BinarytoDecimal(rs, rs.length());
            int rtDec = BinarytoDecimal(rt, rt.length());
            int rdDec = BinarytoDecimal(rd, rd.length());
            reg[rdDec] = reg[rsDec] + reg[rtDec];
            outFile <<"add "<< reg[rdDec] << "\n\n";
            printform(outFile, i, address, reg, address.size());
            inscount++;
            
        }
        else if (opcode == "001") // case:nand
        {
            string rs = split(bit, 10, 12);
            string rt = split(bit, 13, 15);
            string rd = split(bit, 29, 31);
            int rsDec = BinarytoDecimal(rs, rs.length());
            int rtDec = BinarytoDecimal(rt, rt.length());
            int rdDec = BinarytoDecimal(rd, rd.length());
            reg[rdDec] = nandOperation(reg[rsDec], reg[rtDec]);
            outFile <<"nand "<< reg[rdDec] << "\n\n";
            printform(outFile, i, address, reg, address.size());
            inscount++;
            
        }
        else if (opcode == "010") // case:LW
        {
            string rt = split(bit, 13, 15);
            string rs = split(bit, 10, 12);
            string imm = split(bit, 16, 31);
            int resultRs = BinarytoDecimal(rs, rs.length());
            int resultImm = BinarytoDecimal(imm, imm.length());
            int resultaddr = reg[resultRs] + resultImm;
            int addrRt = BinarytoDecimal(rt, rt.length());
            if (resultaddr >= address.size())
            {
                throw invalid_argument("received out of address");
                break;
            }
            else
            {
                reg[addrRt] = address[resultaddr];
                printform(outFile, i, address, reg, address.size());
                inscount++;
            }

            
        }
        else if (opcode == "011") // case:SW
        {
            string rt = split(bit, 13, 15);
            string rs = split(bit, 10, 12);
            string imm = split(bit, 16, 31);
            int resultRs = BinarytoDecimal(rs, rs.length());
            int resultImm = BinarytoDecimal(imm, imm.length());
            int resultaddr = reg[resultRs] + resultImm; // address that is target need to load to reg
            int addrRt = BinarytoDecimal(rt, rt.length());
            if (resultaddr >= address.size())
            {
                throw invalid_argument("received out of address");
            }
            else
            {
                address[resultaddr] = reg[addrRt];
                printform(outFile, i, address, reg, address.size());
                inscount++;
            }
            
        }
        else if (opcode == "100") // case:beq
        {
            string rt = split(bit, 13, 15);
            string rs = split(bit, 10, 12);
            string imm = split(bit, 16, 31);
            int rtDec = BinarytoDecimal(rt, rt.length());
            int rsDec = BinarytoDecimal(rs, rs.length());
            int immDec = BinarytoDecimal(imm, imm.length());
            if (reg[rtDec] == reg[rsDec])
            {
                immDec = twoComplement(immDec);
                // cout<<"this is "<<immDec<<endl;
                i = i + immDec;
            }
            printform(outFile, i, address, reg, address.size());
            inscount++;
            
        }
        else if (opcode == "101") // case:jalr
        {
            string rt = split(bit, 13, 15);
            string rs = split(bit, 10, 12);
            int addrRt = BinarytoDecimal(rt, rt.length());
            int resultRs = BinarytoDecimal(rs, rs.length());
            reg[addrRt] = i + 1; 
            i = reg[resultRs]-1;
            printform(outFile, i, address, reg, address.size());
            inscount++;
            
        }
        else if (opcode == "110") // case:halt
        {
            // pos++;

            inscount++;
            cout << "machine halted" << endl;
            cout << "total of " << inscount << " instructions executed" << endl;
            cout << "final state of machine:\n\n";
            outFile << "machine halted" << endl;
            outFile << "total of " << inscount << " instructions executed" << endl;
            outFile << "final state of machine:\n\n";
            printform(outFile, i, address, reg, address.size());
            break;

           // case:noop
        }
        else if (opcode == "111") 
        {

            // unknown opcode
        }
        else
        {
            cout << "Unknown opcode!" << endl; // Maybe better if throwException
        }
    }
}

int main()
{
    
    ofstream outFile("outpt.txt");
    if (!outFile)
    {
        cerr << "Error opening file!" << endl;
        return 1;
    }
    runMachineCode(outFile, readMachineCode());
    return 0;
}