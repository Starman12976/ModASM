note    Define macros
replace exitCode with 0

note   Section print outputs "Hello, world!" to the terminal
define section print:
    note Create a new message and save its length
    let  message        = "Hello, world!"
    let  messageLength  = 14

    note Write the message to the terminal
    move sysWrite       to sysArg1
    move stdOut         to sysArg2
    move message        to sysArg3
    move messageLength  to sysArg4
    syscall

    note Return to last position
    return.

note   Section exit exits the program successfully with a code of 0.
define section exit:
    note Exit the program
    move sysExit    to sysArg1
    move exitCode   to sysArg2
    syscall.

note   Section start is the entry point of the program.
define section start:
    note Print to the terminal
    jump to print

    note Exit the program
    jump to exit.

note Begin the program.
jump to start.
