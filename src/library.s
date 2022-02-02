    .global library
    .global library_size
    .global pyftsubset
    .global pyftsubset_size
    .global libcrypto
    .global libcrypto_size
    .global libssl
    .global libssl_size
    .global openssl
    .global openssl_size
    .global libunistring
    .global libunistring_size
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

libcrypto:
    .incbin "libcrypto.pc"
libcrypto_end:
libcrypto_size:
    .int libcrypto_end - libcrypto

libssl:
    .incbin "libssl.pc"
libssl_end:
libssl_size:
    .int libssl_end - libssl

openssl:
    .incbin "openssl.pc"
openssl_end:
openssl_size:
    .int openssl_end - openssl

libunistring:
    .incbin "libunistring.pc"
libunistring_end:
libunistring_size:
    .int libunistring_end - libunistring
