
ldr r7   =0x000FFFFF
ldr r1   =0x00080000
ldr r3   =0x20200004
ldr r4   =0x20200028
ldr r5   =0x2020001C
ldr [r3] =0x08000000
clear:
ldr [r4] r1
mov r6   r7
loop1:
sub r6	 r6 #1
cmp r6   #0
bne loop1
ldr [r5] r1 	
mov r6   r7    
loop2:
sub r6	 r6 #1
cmp r6   #0
bne loop2
b clear
andeq r0 r0 r0
andeq r0 r0 r0
andeq r0 r0 r0
andeq r0 r0 r0


