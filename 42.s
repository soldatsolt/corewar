.comment    "This city needs me"
.name       "BATMANBAT
MANBATMANBATMANB
NBATMANBATMANBATMANBATMANBATMANBATM
ANBATMAN223111111111111111110"

loop:
        sti r1, %:live, %1
live:
        live %0
        ld %0, r2
        zjmp %:loop
1111

