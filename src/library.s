    .global library
    .global library_size
    .section .rodata
library:
    .incbin "library.json"
1:
library_size:
    .int 1b - library
