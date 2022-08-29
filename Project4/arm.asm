

// File name: arm.asm

// The program develops an Armstrong number checker application. 
// The input to the program is a number between 0..999 and is stored in RAM[0] (R0).
// Number A is a positive integer.

// Program functions as follows: 
// Extract the individual digits from number A and store the cumulative sum of the cubes of individual digits in R2.
// Result of 1 is stored in R1 if the number A is Armstrong else result of 0 is stored in R1

// Put your code below this line

@R0 
D = M 
@a
M = D

@R2           // reset ouputs just in case
M = 0 
@R1 
M = 0  

(MAINLOOP)
    @c 
    M = D      
    @a 
    D = M 
    
    (MOD10)          //  c = a % 10     
        @10
        D = D - A     
        @SUB     
        D; JLT  
        @c   
        M = D       
        @MOD10
        0; JMP
    
    (SUB)              // a = a - c
        @c 
        D = M 
        @a 
        M = M - D
    
    @i
    M=1
    (POW2)              // temp =C*C 
        @i
        D=M
        @c
        D=D-M
        @NEXT
        D;JGT
        @c
        D=M
        @temp 
        M=M+D
        @i
        M=M+1
        @POW2
        0;JMP  
    
    (NEXT) 
        @i
        M=1
    
    (POW3)             // temp1 = temp*c 
        @i
        D=M
        @c
        D=D-M
        @STORE
        D;JGT
        @temp
        D=M
        @temp1 
        M=M+D
        @i
        M=M+1
        @POW3 
        0;JMP

    (STORE)           // store RAM[2] = RAM[2] + c*c*c 
        @temp1 
        D = M 
        @R2 
        M = M + D 
    
    @a          
    D = M
    (DIV10)           //  diff = a / 10 
        @10
        D = D - A    
        @FINAL   
        D; JLT
        @diff   
        M = M + 1
        @DIV10
        0; JMP
    (FINAL) 
        @diff
        D = M 
        @a          
        M = D          // a = diff

        @temp
        M = 0
        @temp1 
        M = 0
        @diff 
        M = 0
        
        @MAINLOOP     // if a > 0 loop back 
        D; JGT  

        @R2            // check if r2 and r0 are equal 
        D = M 
        @R0
        D = D - M
        @END 
        D; JNE   

        @R1           // if equal set r1 to 1 
        M = 1 

(END)
    @END  
    0; JMP
    
