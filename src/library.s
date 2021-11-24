    .global library
    .global library_size
    .global pyftsubset
    .global pyftsubset_size
    .section .rodata
library:
    .incbin "library.json"
library_end:
library_size:
    .int library_end - library

pyftsubset:
    .incbin "pyftsubset.py"
pyftsubset_end:
pyftsubset_size:
    .int pyftsubset_end - pyftsubset
