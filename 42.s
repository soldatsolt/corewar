.name "King Sombra"
.comment "Crystal empire is mine!"

start1:
zjmp %3
sti r10, r11, r12
sti r7, r8, r12
live %0
sti r10, r11, r12
sti r7, r8, r12
live %0
sti r10, r11, r12
sti r7, r8, r12
live %0
sti r10, r11, r12
sti r7, r8, r12
live %0
sti r10, r11, r12
sti r7, r8, r12
live %0
sti r10, r11, r12
sti r7, r8, r12
live %0
sti r10, r11, r12
sti r7, r8, r12
live %0
sti r10, r11, r12
sti r7, r8, r12
live %0
sti r10, r11, r12
sti r7, r8, r12
live %0
sti r10, r11, r12
sti r7, r8, r12
live %0
sti r10, r11, r12
sti r7, r8, r12
live %0
sti r10, r11, r12
sti r7, r8, r12
live %0
sti r10, r11, r12
sti r7, r8, r12
live %0
sti r10, r11, r12
sti r7, r8, r12
live %0
sti r10, r11, r12
sti r7, r8, r12
live %0
sti r10, r11, r12
sti r7, r8, r12
live %0
sti r10, r11, r12
sti r7, r8, r12
live %0
start2:
sti r10, r11, r12
sti r7, r8, r12
live %0

assaut2:
live %0
ld %190055943, r10 # sti r6, r7
ld %84476405, r11 # r5 jump 01F5 (501)
ld %150994956, r3 #0x0900000c
ld %150994956, r6 #0x0900000c
ld %26, r7
ld %495, r4
ld %0, r5
zjmp %:jump

assaut1:
live %0
ld %190055943, r10 # sti r6, r7
ld %84476405, r11 # r5 jump 01F5 (501)
fork %:second
ld %190055172, r3 #0x0B540304
ld %190055172, r6 #0x0B540304
ld %507, r7
ld %24, r4
ld %0, r5
zjmp %:jump

second:
live %0
ld %84475923, r3 #0x0509001b
ld %84475923, r6
ld %511, r7
ld %28, r4
ld %0, r5
zjmp %:jump

jump:
live %0
st r10, 511
st r11, 510
zjmp %501
