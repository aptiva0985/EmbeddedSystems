@ hello.S: Hello world! in assembly
@
@ Authors: Yuanshuo Cui <ycui@andrew.cmu.edu>
@          Yujing Zhang <yujingz@andrew.cmu.edu>
@          Liyu Ma      <liyum@andrew.cmu.edu>
@ Date:    18:51, 10/13/2013

#include <bits/swi.h>
#include <bits/fileno.h>

    .file    "hello.S"
    .text

    .global    _start
_start:
    mov r0, #STDOUT_FILENO   @ write's first arg: the stdout
    adr r1, hello_str        @ write's second arg: "Helllo world!" string
    mov r2, #hello_size      @ write's third arg: the length of the string
    SWI WRITE_SWI            @ call the write syscall
    mov r0, #0               @ set exit status code
    SWI EXIT_SWI             @ call the exit syscall

hello_str:
    .ascii "Hello world!\n"
    .set hello_size, .-hello_str
