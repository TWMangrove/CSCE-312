//NAME: Freddy Velasco
//UIN:  327004703

// File name: calc.asm

// The program develops a calculator application. 
// The operands a and b are integer numbers stored in RAM[0] (R0) and RAM[1] (R1), respectively.
// The operation choice c is stored in RAM[2] (R2), respectively
// if c == 1, do a + b
// if c == 2, do a - b
// if c == 3, do a * b
// if c == 4, do a / b
// For Addition and Subtraction operations the operands a and b can be positive or negative.
// For Multiplication operation only ONE operand can be negative.
// For Division operation BOTH operands must be positive and must be greater than 0.
// Store the final result (quotient for Division) in RAM[3] (R3). Only the Division operation 
// stores the remainder in RAM[4] (R4).


// Put your code below this line
    @R3 
    M = 0
    @R4
    M = 0
    @1024
    M = 0             // reset output just in case
    
    @R2
    D = M              // get c      
    
    @ADD
    D = D - 1  
    D; JEQ             // if c is 1 then jump to add branch
    
    @SUB
    D = D - 1
    D; JEQ             // if c is 2 then jump to sub branch   
    
    @MULT
    D = D - 1
    D; JEQ             // if c is 1 then jump to add branch
    
    @DIV
    D = D - 1
    D; JEQ             // if c is 2 then jump to sub branch

    @ERROR             // c is an invalid input
    0; JMP 

(ADD)
    @R0
    D = M
    @R1
    D = D + M          // add r0 and r1
    @R3
    M = D              // set sum to r3
    @END  
    0; JMP

(SUB)
    @R0
    D = M
    @R1
    D = D - M          // r0 - r1
    @R3
    M = D              // set diff to r3
    @END  
    0; JMP

(MULT)
    @i
    M=1

    @R1
    D = M
    @MULTLOOP
    D; JGT         // jump to loop if R1 is not negative
    @END 
    D; JEQ         // if R1 is 0 then end program 

    @R0         
    D = M
    @ERROR 
    D; JLT          // if both r1 and r2 are negative, then jump to error branch
    
    
    @R0            // if only r1 is negative swap the numbers, since loop will not work otherwise          
    D = M
    @temp            
    M = D           // temp = r0
    @R1 
    D = M          
    @R0 
    M = D            // r0 = r1
    @temp
    D = M 
    @R1 
    M = D            // r1 = temp

    (MULTLOOP)             // for loop that adds R0 to R3 until i = R1
        @i
        D=M
        @R1
        D=D-M
        @END
        D;JGT
        @R0
        D=M
        @R3
        M=M+D
        @i
        M=M+1
        @MULTLOOP
        0;JMP

(DIV)
    @R0         
    D = M
    @ERROR 
    D; JLT          // if both r0 is negative, then jump to error branch
    @END 
    D; JEQ          // if r0 is 0 then end program
    
    @R1
    D = M
    @ERROR
    D; JLT         //if both r1 is negative, then jump to error branch
    @END 
    D; JEQ         // if R1 is 0 then end program 

    @R4          
    M = D
    @R1 
    D = D - M 
    @END 
    D; JLT   // if R1 is greater than R0, can't divide so just asssume carry is R1

    @R0         
    D = M

    (DIVLOOP)        // while loop that subtracts r1 from value of r0 until value of r0 is negative; every loop adds 1 to r3
        @R1 
        D = D - M    
        @END 
        D; JLT
        @R3  
        M = M + 1
        @R4 
        M = D       // get carry just in case of correct output
        @DIVLOOP
        0; JMP   

(ERROR)
    D = -1
    @1024
    M = D
    @END  
    0;JMP

(END)
    @END  
    0; JMP