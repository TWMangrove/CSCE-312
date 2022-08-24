//Starter Test stimulus file for AddSub10 

load AddSub10.hdl,
output-file AddSub10.out,
compare-to AddSub10.cmp,
output-list a%B1.10.1 b%B1.10.1 sub%B1.1.1 out%B1.10.1 overflow%B5.1.5;

set a %B0000000000,
set b %B0000000000,
set sub 0,
eval,
output;

set a %B0000000001,
set b %B0000000101,
set sub 0,
eval,
output;

set a %B0000100101,
set b %B0001001000,
set sub 1,
eval,
output;

//fill in more test cases here to thoroughly check correctness of your chip