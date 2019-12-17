.comment    "This city needs me"
.name       "BA
TM
MA
N"

loop:
        sti r1, %:live, %1
live:
        live %0
        ld %0, r2
        zjmp %:loop
1111

