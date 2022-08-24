//Starter Test stimulus file for FastRCA12 

load FastRCA12.hdl,
compare-to FastRCA12.cmp,
output-file FastRCA12.out,
output-list a%B3.12.3 b%B3.12.3 out%B3.12.3 carry%B3.1.3;

set a %B000000000000,
set b %B000000000000,
eval,
output;