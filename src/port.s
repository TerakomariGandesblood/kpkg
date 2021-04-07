    .global port
    .global port_size
    .section .rodata
port:
    .incbin "port.json"
1:
port_size:
    .int 1b - port
