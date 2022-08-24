// Filename = arm.tst

load arm.asm,
output-file arm.out,
compare-to arm.cmp,
output-list RAM[0]%D2.6.2 RAM[1]%D2.6.2 RAM[2]%D2.6.2;

set RAM[0] 370,	// Set test arguments
set RAM[1] 0;		// Test that program initialized to 0
set RAM[2] 0;		// Test that program initialized to 0
repeat 1500 {
  ticktock;
}
set RAM[0] 370,   	// Restore arguments in case program used them as loop counter
output;

set PC 0,
set RAM[0] 4,	// Set test arguments
set RAM[1] 0;		// Test that program initialized to 0
set RAM[2] 0;		// Test that program initialized to 0
repeat 1500 {
  ticktock;
}
set RAM[0] 4,   	// Restore arguments in case program used them as loop counter
output;


set PC 0,
set RAM[0] 0,	// Set test arguments
set RAM[1] 0;		// Test that program initialized to 0
set RAM[2] 0;		// Test that program initialized to 0
repeat 1500 {
  ticktock;
}
set RAM[0] 0,   	// Restore arguments in case program used them as loop counter
output;

set PC 0,
set RAM[0] 331,	// Set test arguments
set RAM[1] 0;		// Test that program initialized to 0
set RAM[2] 0;		// Test that program initialized to 0
repeat 1500 {
  ticktock;
}
set RAM[0] 331,   	// Restore arguments in case program used them as loop counter
output;


// Write your own testcases below: