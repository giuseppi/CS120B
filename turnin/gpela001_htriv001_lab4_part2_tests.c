# Test file for Lab2_introToAVR


# commands.gdb provides the following functions for ease:
#   test "<message>"
#       Where <message> is the message to print. Must call this at the beginning of every test
#       Example: test "PINA: 0x00 => expect PORTC: 0x01"
#   checkResult
#       Verify if the test passed or failed. Prints "passed." or "failed." accordingly, 
#       Must call this at the end of every test.
#   expectPORTx <val>
#       With x as the port (A,B,C,D)
#       The value the port is epected to have. If not it will print the erroneous actual value
#   setPINx <val>
#       With x as the port or pin (A,B,C,D)
#       The value to set the pin to (can be decimal or hexidecimal
#       Example: setPINA 0x01
#   printPORTx f OR printPINx f 
#       With x as the port or pin (A,B,C,D)
#       With f as a format option which can be: [d] decimal, [x] hexadecmial (default), [t] binary 
#       Example: printPORTC d
#   printDDRx
#       With x as the DDR (A,B,C,D)
#       Example: printDDRB

echo ======================================================\n
echo Running all tests..."\n\n

# Add tests below

test "Press nothing. Result: State = wait, PORTC = 0x07"
set state = start
setPINA 0x00
continue 2
expectPORTC 0x07
checkResult
expect state 1
checkResult

test "Add 1. Result: State = plus, PORTC = 0x08"
set state = start
setPINA 0x00
continue 2
setPINA 0x01
continue 2
expectPORTC 0x08
checkResult
expect state 2
checkResult

test "Subtract 1. Result: State = minus, PORTC = 0x06"
set state = start
setPINA 0x00
continue 2
setPINA 0x10
continue 2
expectPORTC 0x06
checkResult
expect state 3
checkResult

test "Press both. Result: State = reset, PORTC = 0x00"
set state = start
setPINA 0x00
continue 2
setPINA 0x03
continue 2
expectPORTC 0x00
checkResult
expect state 1
checkResult

# Report on how many tests passed/tests ran
set $passed=$tests-$failed
eval "shell echo Passed %d/%d tests.\n",$passed,$tests
echo ======================================================\n
