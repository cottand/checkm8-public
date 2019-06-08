ldr r7   =0x000FFFFF
ldr r1   =0x00000200
ldr r3   =0x20200000
ldr r4   =0x20200028
ldr r5   =0x2020001C
ldr r8   =0x08000000

str r8 [r3]
clear:
str r1 [r4]
mov r6   r7
loop1:
sub r6	 r6 #1
cmp r6   #0
bne loop1
str r1 [r5]
mov r6  r7
loop2:
sub r6	 r6 #1
cmp r6   #0
bne loop2
b clear
andeq r0 r0 r0
andeq r0 r0 r0
andeq r0 r0 r0
andeq r0 r0 r0


