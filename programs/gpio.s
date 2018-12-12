ldr r0,=0x20200000
mov r1,#1
lsl r1,#18
str r1,[r0,#4]
mov r1,#1
lsl r1,#16

loop:
str r1,[r0,#40]

mov r2,=0x3000000
sleep:
sub r2,r2,#1
cmp r2,#0
bne sleep

str r1,[r0,#28]

mov r2,=0x3000000
sleepTwo:
sub r2,r2,#1
cmp r2,#0
bne sleepTwo

b loop

