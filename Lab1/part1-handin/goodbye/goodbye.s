@ Group 32: goodbye.s
@ Yuanshuo Cui (ycui)
@ Yujing Zhang (yujingz)
@ Liyu Ma (liyum)

    .file     "hello.c"
    .section  .rodata
    .align    2
.LC0:
    .ascii    "Hello world!\000"
    .align    2
.LC1:
    .ascii    "Goodbye world!\000"
    .text
    .align    2
    .global   main
    .type     main, %function
main:
    @ Function supports interworking.
    @ args = 0, pretend = 0, frame = 0
    @ frame_needed = 1, uses_anonymous_args = 0
    mov    ip, sp
    stmfd    sp!, {fp, ip, lr, pc}
    sub    fp, ip, #4
    sub    sp, sp, #8
    str    r0, [fp, #-16]
    str    r1, [fp, #-20]
    @ Writes the string "Hello world!" followed by a new line to stdout.
    ldr    r0, .L3
    bl    puts
    @ Writes the string "Goodbye world!" followed by a new line to stdout.
    ldr    r0, .L3+4
    bl     puts
    @ Terminates with exit status 42.
    mov    r0, #42
    bl     exit
.L4:
    .align   2
.L3:
    .word    .LC0
    .word    .LC1
    .size    main, .-main
    .ident   "GCC: (GNU) 4.1.2"
