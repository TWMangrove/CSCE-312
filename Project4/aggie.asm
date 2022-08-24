//NAME: Freddy Velasco 
//UIN: 327004703

// File name: aggie.asm

// The program runs an infinite loop that listens to the keyboard input. As soon as a 
// key press is detected, the program exits the wait loop and proceeds to draw the 
// graphic as described in the project document. 
// You will need to draw it pixel by pixel because HACK asm has no 
// utilities for drawing shapes.
// State any assumptions made in your program 

// Assumptions (as applicable)

// I assumed that after drawing the program the program should go into an endless loop.
// I also assummed that the screen was already clear prior to the program running.  Checking to 
// see if the screen was clear would double the already-lengthy running time. If this were 
// to be a feature of the program, it would be implemented similarly to the DRAW loop, except 
// the loop would exit to END if a pixel was not 0, otherwise it would go into an endless loop
// and await input. 



// Put your code below this line

(LOOP)
    @24576
    D = M
    @DRAW  
    D; JNE
    @LOOP 
    0; JMP            // Loop until keyboad is pressed
(DRAW) 
   
    //  x = SCREEN  
    //  for(i=0; i < 128; ++i)           // width  of black box (half of 256 )
    //    for (j=0; j < 16;++j)          // length of black box (half of 525/16 or half of 32)
    //      RAM[x] = 1
    //      ++x;             
    //    x += 256                      // skip second quadrant
    
    @SCREEN 
    D = A
    @x 
    M = D             // x = screen
    
    @i
    M = 0             // i = 0
    (LOOPWIDTH)   
        @i
        D = M
        @128
        D = D - A 
        @END 
        D; JGE         //  while i < 128  
        
        @i
        M = M + 1      // ++i

        // ADD 256

        @j  
        M = 0               // j = 0
        (LOOPLENGTH)
            @j
            D = M 
            @16
            D = D - A 
            @ADD16 
            D; JGE             // while j < 16
  
            @x
            A = M
            M = -1        // RAM[x] = -1

            @x 
            M = M +1            // ++x

            @j 
            M = M + 1           // ++i

            @LOOPLENGTH
            0; JMP 
            
            (ADD16)
                @16 
                D = A 
                @x 
                M = M + D     // x + 16
                @LOOPWIDTH
                0;JMP
(END) 
    @END 
    0;JMP
                












