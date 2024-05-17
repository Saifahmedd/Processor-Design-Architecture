#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#define MAX_LINE_LENGTH 1000
#define MAX_NUM_TOKENS 100




#define UPPER_BITS_MASK 0xF0000000 // Mask for extracting upper 4 bits
#define LOWER_BITS_MASK 0x0FFFFFFF // Mask for extracting lower 28 bits


int registers[33];
char memory[2048][33 * sizeof(char)];
int memoryCount=0;
int PC=0;

int cycles=1;
char strings[200][1000];
char instruction2[100]; // Assuming a sufficiently large buffer
int temp = 1;
int temp2 =2;
int temp3 = 4;
int temp4 = 6;
int temp5 = 7;

const char* InstructionFormat(const char* instruction) {
    if (strcmp(instruction, "ADD") == 0) {
        return "R";
    } else if (strcmp(instruction, "SUB") == 0) {
        return "R";
    }else if (strcmp(instruction, "MUL") == 0) {
        return "R";
    }else if (strcmp(instruction, "MOVI") == 0) {
        return "I";
    }else if (strcmp(instruction, "JEQ") == 0) {
        return "I";
    }else if (strcmp(instruction, "AND") == 0) {
        return "R";
    }else if (strcmp(instruction, "XORI") == 0) {
        return "I";
    }else if (strcmp(instruction, "JMP") == 0) {
        return "J";
    }else if (strcmp(instruction, "LSL") == 0) {
        return "R";
    }else if (strcmp(instruction, "LSR") == 0) {
        return "R";
    }else if (strcmp(instruction, "MOVR") == 0) {
        return "I";
    }else if (strcmp(instruction, "MOVM") == 0) {
        return "I";
    }else{
         return "opcode doesn't exist";
    }
}

void WriteBack(int pc2,int regNum, int value, int opcode){
memset(instruction2, 0, sizeof(instruction2));
sprintf(instruction2, "Instruction %d will Write Back ", pc2 + 1);
strcat(strings[temp5], instruction2); // Use strcpy instead of strcat for the first string

// First WriteBack stage
if(opcode==4 || opcode==7){
    
    PC = value;

    //print en el pc value set to value
    
memset(instruction2, 0, sizeof(instruction2));
sprintf(instruction2, ",PC Value set to %d. ", PC);
strcat(strings[temp5], instruction2); // Use strcpy instead of strcat for the first string


}else{
    //print en el reg set to value
    registers[regNum] = value;
    
memset(instruction2, 0, sizeof(instruction2));
sprintf(instruction2, ",Register %i Value set to %d. ", regNum, value);
strcat(strings[temp5], instruction2); // Use strcpy instead of strcat for the first string


}
temp5++; // Increment cycles after adding the first string
temp5++;
}

void Memory(int instruction, int pc2 ,int calculation, int operandA, int opcode){
// First Memory stage
memset(instruction2, 0, sizeof(instruction2));
sprintf(instruction2, "Instruction %d will access Memory ", pc2 + 1);
strcat(strings[temp4], instruction2); // Use strcpy instead of strcat for the first string


int value=0;


if( instruction == 11){

    //store into memory
    memset(instruction2, 0, sizeof(instruction2));
    sprintf(instruction2, "Instruction %d will not Write Back ", pc2 + 1);
    strcat(strings[temp5], instruction2); // Use strcpy instead of strcat for the first string
    temp5++; // Increment cycles after adding the first string
    temp5++;

        char s[1000];
        sprintf(s, "%d", registers[operandA]);

        strcpy(memory[calculation], s);

        memset(instruction2, 0, sizeof(instruction2));
        sprintf(instruction2, ",Memory value of address %i set to %s. ", calculation, s);
        strcat(strings[temp4], instruction2); // Concatenate the message
}
else{
    //store from memory into register  
    value = atoi(memory[calculation]);

    WriteBack(pc2,operandA,value,opcode);
}

temp4++; // Increment cycles after adding the first string
temp4++;
}

void ALU(int operandA, int operandB, int operandC ,int instruction,int imm, int address, int SHAMT ){
    int pc2 = PC;
// First Execute stage
memset(instruction2, 0, sizeof(instruction2));
sprintf(instruction2, "Instruction %d is Executed ", PC + 1);
strcat(strings[temp3], instruction2); // Use strcpy instead of strcat for the first string
temp3++; // Increment cycles after adding the first string

// Second Execute stage
memset(instruction2, 0, sizeof(instruction2));
sprintf(instruction2, "Instruction %d is Executed ", PC + 1);
strcat(strings[temp3], instruction2); // Use strcpy instead of strcat for the second string
temp3++;
int res=0;

    if (instruction==0) {//buildo
        //add
        res = registers[operandB] + registers[operandC];

        if ((registers[operandB] > 0 && registers[operandC] > 0 && registers[operandA] < 0) || (registers[operandB] < 0 && registers[operandC] < 0 && registers[operandA] > 0)) {
                // Overflow occurred
                printf("Overflow occurred during addition operation\n");
            }
            
        
    } else if (instruction==1) {
        //subtract
        res = registers[operandB] - registers[operandC];

        
        if ((registers[operandB] > 0 && registers[operandC] < 0 && registers[operandA] < 0) || (registers[operandB] < 0 && registers[operandC] > 0 && registers[operandA] > 0)) {
                // Overflow occurred
                printf("Overflow occurred during subtraction operation\n");
            }
        
        
        
        
        
    }else if (instruction==2) {
        //multiplication
        res = registers[operandB] * registers[operandC];

        if (registers[operandB] != 0 && registers[operandC] != 0 &&  registers[operandA] / registers[operandB] != registers[operandC]) {
                // Overflow occurred
                printf("Overflow occurred during multiplication operation\n");
            }
        
        
        
       
    }else if (instruction==3) {
        //move immediate
        res = imm; 

        
        
        
        
    }else if (instruction==4) {
        //jump if equal
        int t = registers[operandA] - registers[operandB]; 

        //registers[PC] = (registers[operandA] == 0) ? registers[PC] + 1 + imm : registers[PC] + 1;
                //printf(" PC = %d \n",PC );
        
        if(t==0){
            res=PC+imm;
        }else{
            res=PC;
        }

        
    }else if (instruction==5) {
        //and
        
       res = registers[operandB] & registers[operandC];

        
        
        
        
    }else if (instruction==6) {//khaled
    //xor immediate
    res=(operandB ^ imm) ;

        
    }else if (instruction==7) {
     //jump   
    int upperbits = PC & UPPER_BITS_MASK;
        
        res=upperbits | address ;
        res--;
        res--;
        
        
    }else if (instruction==8) {
     //Shift left logical    
        // registers[operandA] = registers[operandB] << SHAMT;   
        //         printf("%d \n",registers[operandA] );
                res = registers[operandB] << SHAMT;


        
        
        
    }else if (instruction==9) {
     //Shift Right logical 
        // registers[operandA] = registers[operandB] >> SHAMT;
        //         printf("%d \n",registers[operandA] );

             res = registers[operandB] >> SHAMT;

        
        
    }else if (instruction==10) {
     //move to register
        // registers[operandA]= atoi(memory[registers[operandB]+imm]); //need to cast to int
        //         printf("%d \n",registers[operandA] );
        res = registers[operandB] + imm;

        
        
        
    }else if (instruction==11) {
     //move to memory  
    //    char s[1000]; // Assuming a maximum length for the string representation of the integer value
    
    // // Convert integer value to string
    //     sprintf(s, "%d", registers[operandA]);

    //     strcpy(memory[ registers[operandB] + imm ], s);
            res = registers[operandB] + imm;

        
        
    }
    if(instruction == 10 || instruction == 11){
        Memory(instruction, pc2,res,operandA,instruction);
    }
    else {
        
        memset(instruction2, 0, sizeof(instruction2));
        sprintf(instruction2, "Instruction %d will not access memory ", pc2 + 1);
        strcat(strings[temp4], instruction2); // Use strcpy instead of strcat for the first string
        temp4++; // Increment cycles after adding the first string
        temp4++;
        
        if( instruction == 11){
        memset(instruction2, 0, sizeof(instruction2));
        sprintf(instruction2, "Instruction %d will not Write Back ", pc2 + 1);
        strcat(strings[temp5], instruction2); // Use strcpy instead of strcat for the first string
        temp5++; // Increment cycles after adding the first string
        temp5++;
    }
    else{
        WriteBack(pc2,operandA,res,instruction);
    }
    }
}

void fetch() {
    while(PC < memoryCount){
        printf("pc fetch: %d \n",PC);
        
        memset(instruction2, 0, sizeof(instruction2));
        sprintf(instruction2, "Instruction %d is fetched ", PC + 1);
        strcat(strings[temp], instruction2);
        temp ++;
        temp ++;
        cycles++;
        char *instruction = memory[PC];
        printf("instruction fetch: %s \n",instruction);
        decode(instruction);
        PC++;
    }
}

void decode(char instruction[]) {
// First decode stage
memset(instruction2, 0, sizeof(instruction2));
sprintf(instruction2, "Instruction %d is decoded ", PC + 1);
strcat(strings[temp2], instruction2); // Use strcpy instead of strcat for the first string
temp2++; // Increment cycles after adding the first string

// Second decode stage
memset(instruction2, 0, sizeof(instruction2));
sprintf(instruction2, "Instruction %d is decoded ", PC + 1);
strcat(strings[temp2], instruction2); // Use strcpy instead of strcat for the second string
temp2++;

    // Extracting opcode
    int opcode = 0;
    for (int i = 0; i < 4; i++) {
        opcode |= (instruction[i] - '0') << (3 - i); // Assuming instruction is in binary string format
    }

    // Extracting rs
    int rs = 0;
    for (int i = 4; i < 9; i++) {
        rs |= (instruction[i] - '0') << (8 - i);
    }

    // Extracting rt
    int rt = 0;
    for (int i = 9; i < 14; i++) {
        rt |= (instruction[i] - '0') << (13 - i);
    }

    // Extracting rd
    int rd = 0;
    for (int i = 14; i < 19; i++) {
        rd |= (instruction[i] - '0') << (18 - i);
    }

    // Extracting shamt
    int shamt = 0;
    for (int i = 19; i < 32; i++) {
        shamt |= (instruction[i] - '0') << (31 - i);
    }
    
    int imm = 0;
    for (int i = 14; i < 32; i++) {
        imm |= (instruction[i] - '0') << (31 - i);
    }

    // Handle sign extension for immediate
    if (instruction[14] == '1') {
        imm |= 0xFFFFC000;
    }
    
    int address=0;
     for (int i = 4; i < 32; i++) {
        address |= (instruction[i] - '0') << (31 - i);
    }
    
    // Printing decoded values
    printf("Decoded values for instruction %d:\n", PC + 1);
    printf("opcode = %d\n", opcode);
    printf("rs = %d\n", rs);
    printf("rt = %d\n", rt);
    printf("rd = %d\n", rd);
    printf("imm = %d\n", imm);
    printf("address = %d\n", address);

    printf("shamt = %d\n", shamt);
    printf("\n");
    
    
    ALU(rs,rt,rd,opcode,imm,address,shamt);
}

void storeInstruction(const char *instruction, int address) {
    strcpy(memory[address], instruction);
    printf("Memory: %s\n", memory[address]);
    memoryCount++;
    printf(" memory : %d \n ",memoryCount);

}

const char* opcode(const char* instruction) {
    if (strcmp(instruction, "ADD") == 0) {
        return "0000";
    } else if (strcmp(instruction, "SUB") == 0) {
        return "0001";
    }else if (strcmp(instruction, "MUL") == 0) {
        return "0010";
    }else if (strcmp(instruction, "MOVI") == 0) {
        return "0011";
    }else if (strcmp(instruction, "JEQ") == 0) {
        return "0100";
    }else if (strcmp(instruction, "AND") == 0) {
        return "0101";
    }else if (strcmp(instruction, "XORI") == 0) {
        return "0110";
    }else if (strcmp(instruction, "JMP") == 0) {
        return "0111";
    }else if (strcmp(instruction, "LSL") == 0) {
        return "1000";
    }else if (strcmp(instruction, "LSR") == 0) {
        return "1001";
    }else if (strcmp(instruction, "MOVR") == 0) {
        return "1010";
    }else if (strcmp(instruction, "MOVM") == 0) {
        return "1011";
    }else{
         return "opcode doesn't exist";
    }
}

void parseInstructionR(const char *line) {
    char *inst;
    char instruction[20]; // Assuming maximum token length is 20 characters
    char operands[20];    // Assuming maximum token length is 20 characters
    char *operand;
    char registerNumber[4]; // Assuming the register number will not exceed 3 digits
    char binaryInstruction[33];

    inst = strtok(line, " ");
    if (inst != NULL) {
        strcpy(instruction, inst); // Store the first token (instruction)
        printf("Instruction: %s\n", instruction);
        printf("opcode:%s\n", opcode(instruction));
        bool CHECK=false;
        inst = strtok(NULL, ""); // Get the rest of the string as a single token
        if (inst != NULL) {
            strcpy(operands, inst); // Store the second token (operands)
            printf("Operands: %s\n", operands);
            int operandsLength = strlen(operands);
            printf("length of Operands: %d\n", operandsLength);

            // Tokenize the operands string based on ","
            operand = strtok(operands, ",");
            strcpy(binaryInstruction, opcode(instruction)); // Start with the opcode

            while (operand != NULL) {
                int index = 0;
                printf("Operand: %s\n", operand);
                 if (operand[0] != 'R') {
                     CHECK=true;
                     if (operandsLength==7){
                        for (int i = 0; i < 5; i++) {
                    strcat(binaryInstruction, "0"); // Concatenate 13 zeros
                }
                     }// If operand doesn't start with 'R', convert to 13 bits binary
                    int value = atoi(operand);
                    char binaryValue[14]; // 13 bits plus '\0'
                    for (int i = 12; i >= 0; i--) {
                        binaryValue[12 - i] = ((value >> i) & 1) + '0';
                    }
                    binaryValue[13] = '\0';
                    printf("13 bits binary value: %s\n", binaryValue);
                    strcat(binaryInstruction, binaryValue); // Concatenate the binary value to binary instruction
                } 
                else{
                for (int i = 0; i < strlen(operand); i++) {
                    if (isdigit(operand[i])) {
                        registerNumber[index++] = operand[i];
                    }
                }
                
                registerNumber[index] = '\0';
                printf("Number of register: %s\n", registerNumber);
                int registerValue = atoi(registerNumber);
                char registerBinary[6];
                for (int i = 4; i >= 0; i--) {
                    registerBinary[4 - i] = ((registerValue >> i) & 1) + '0';
                }
                registerBinary[5] = '\0';
                printf("%s\n", registerBinary);
                
                strcat(binaryInstruction, registerBinary); // Concatenate the register binary
                }
                operand = strtok(NULL, ",");
            }
             if ((strcmp(instruction, "ADD") == 0 || strcmp(instruction, "SUB") == 0 || strcmp(instruction, "MUL") == 0 || strcmp(instruction, "AND") == 0 || strcmp(instruction, "LSL") == 0 || strcmp(instruction, "LSR") == 0) && CHECK==false)  {
                strcat(binaryInstruction, "0000000000000");
            }
            printf("Binary Instruction: %s\n", binaryInstruction);
            
            storeInstruction(binaryInstruction, memoryCount);
           
        }
    }
    return;
}
void parseInstructionI(const char *line) {
    char *inst;
    char instruction[20]; // Assuming maximum token length is 20 characters
    char operands[20];    // Assuming maximum token length is 20 characters
    char *operand;
    char registerNumber[4]; // Assuming the register number will not exceed 3 digits
    char binaryInstruction[33];
    int i=0;
    inst = strtok(line, " ");
    if (inst != NULL) {
        strcpy(instruction, inst); // Store the first token (instruction)
        printf("Instruction: %s\n", instruction);
        printf("opcode: %s\n", opcode(instruction));

        inst = strtok(NULL, ""); // Get the rest of the string as a single token
        if (inst != NULL) {
            strcpy(operands, inst); // Store the second token (operands)
            printf("Operands: %s\n", operands);

            // Tokenize the operands string based on ","
            operand = strtok(operands, ",");
            strcpy(binaryInstruction, opcode(instruction)); // Start with the opcode

            while (operand != NULL) {
                int index = 0;
                printf("Operand: %s\n", operand);

                if (operand[0] == 'r' || operand[0] == 'R') {
                    printf("Register operand: %s\n", operand);

                    // Extract the register number from operand
                    for (int i = 0; i < strlen(operand); i++) {
                        if (isdigit(operand[i])) {
                            registerNumber[index++] = operand[i];
                        }
                    }
                    registerNumber[index] = '\0';
                    printf("Number of register: %s\n", registerNumber);

                    // Convert register number to binary
                    int registerValue = atoi(registerNumber);
                    char registerBinary[6];
                    for (int i = 4; i >= 0; i--) {
                        registerBinary[4 - i] = ((registerValue >> i) & 1) + '0';
                    }
                    registerBinary[5] = '\0';
                    printf("Register binary: %s\n", registerBinary);

                    // Concatenate the register binary to the binary instruction
                    strcat(binaryInstruction, registerBinary);
                } else {
                    printf("Immediate operand: %s\n", operand);
                    // Convert immediate value to binary
                    int immediate = atoi(operand);
                    char immediateBinary[19]; // 19 bits to hold 18-bit binary representation + null terminator
                    for (int i = 17; i >= 0; i--) {
                        immediateBinary[17 - i] = ((immediate >> i) & 1) + '0';
                    }
                    immediateBinary[18] = '\0'; // Null terminator at the end
                    printf("Immediate binary: %s\n", immediateBinary);
                    if(i==1){
                        strcat(binaryInstruction, "00000");
                        strcat(binaryInstruction, immediateBinary);
                    }
                    else{
                        // Concatenate the immediate binary to the binary instruction
                        strcat(binaryInstruction, immediateBinary);
                    }
                }
                // Get the next operand
                operand = strtok(NULL, ",");
                i++;
            }

            printf("Binary instruction: %s\n", binaryInstruction);
            storeInstruction(binaryInstruction, memoryCount);
        }
    }
}
            
void parseInstructionJ(const char *line) {
    char instruction[20]; // Assuming maximum token length is 20 characters
    char binaryInstruction[33]; // For 32-bit binary representation plus null terminator
    char address[30]; // Adjust size based on expected address length

    char *inst = strtok((char *)line, " ");
    if (inst != NULL) {
        strcpy(instruction, inst); // Store the first token (instruction)
        printf("Instruction: %s\n", instruction);
        printf("opcode: %s\n", opcode(instruction));

        inst = strtok(NULL, ""); // Get the rest of the string as a single token
        if (inst != NULL) {
            strcpy(address, inst); // Store the second token (address)
            printf("Address: %s\n", address);

            strcpy(binaryInstruction, opcode(instruction)); // Start with the opcode
            int binaryAddress = atoi(address);

            char AddressBinary[29]; // 28 bits for address + 1 for null terminator
            for (int i = 27; i >= 0; i--) {
                AddressBinary[27 - i] = ((binaryAddress >> i) & 1) + '0';
            }
            AddressBinary[28] = '\0';
            printf("Address Binary: %s\n", AddressBinary);

            strcat(binaryInstruction, AddressBinary);

            printf("Binary Instruction: %s\n", binaryInstruction);
            storeInstruction(binaryInstruction, memoryCount);
        }
    }
    return;
}


void parseInstruction(const char *line) {
    char *inst;
    char* a [150];
    char instruction[20]; // Assuming maximum token length is 20 characters
    char operands[20];    // Assuming maximum token length is 20 characters
    char *operand;
    char registerNumber[4]; // Assuming the register number will not exceed 3 digits
    char binaryInstruction[33];
    strcpy(a,line);

    inst = strtok(a, " ");
    printf("inst : %s \n",inst);

    if (inst != NULL) {
        strcpy(instruction, inst);
        
    // Store the first token (instruction)
        const char* Type = InstructionFormat(instruction);
        
        if(strcmp(Type, "R") == 0){
            printf("R");
            parseInstructionR(line);
        }
        else if(strcmp(Type, "I") == 0){
            printf("I");
            parseInstructionI(line);
        }
        else if(strcmp(Type, "J") == 0){
            printf("J");
            parseInstructionJ(line);
        }
    }
}

int main() {
    int counter =0;
    registers[1]=2; //op=5
    registers[2]=2;
    registers[3]=3;
    
    
    registers[4]=0; //op=1
    registers[5]=6;
    registers[6]=5;
    
    FILE *file;
    char line[MAX_LINE_LENGTH];

    // Open the file
    file = fopen("Assembly.txt", "r");
    if (file == NULL) {
        perror("Error opening file");
        return 1;
    }

    // Read lines from the file until the end
    while (fgets(line, sizeof(line), file) != NULL) {
        counter ++;
        parseInstruction(line);
    }

    fclose(file);
    // Close the file
    fetch();
    int size =  7 + ((counter - 1) * 2);
    for(int i=1; i<size+1; i++){
        if(strings[i][0] != '\0'){
            printf("Cycle %d\n", i); // Print the value of i
            if (strlen(strings[i]) > 0) {
                printf("%s\n", strings[i]);
            }
        }
        else{
            break;
        }
    }
    
    

    return 0;
}