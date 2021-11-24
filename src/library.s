    .global library
    .global library_size
    .global font_tools
    .global font_tools_size
    .section .rodata
library:
    .incbin "library.json"
library_end:
library_size:
    .int library_end - library

font_tools:
    .incbin "font_tools.py"
font_tools_end:
font_tools_size:
    .int font_tools_end - font_tools
