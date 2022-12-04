<?php
declare(strict_types=1);
$y = 10; // local variable in main body
function foo(int $x) : void {
$y = "hello"; // local variable in this function
if (1 === 1) {
$y = 42; // not new local variable
write($y); // prints 42
} else { }
write($y);
$i = 1; // local var for whole function
while ($i <= 10) {
$i = 5; // not new local variable just for this block
$i = $i + 1;
write($i); // looping condition is always true (always prints 6)
write("Insert non-empty input for quiting, otherwise next iteration.\n");
$x = reads(); // formal parameter as local variable (can be modified)
$inp = strlen($x);
if ($inp !== 0) { write($x); return; } else {} // stops looping
}
}
foo(7);
write($y); // prints 10, $y in main body is different from $y in foo