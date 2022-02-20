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
    .global libidn2
    .global libidn2_size
    .global libpsl
    .global libpsl_size
    .global filters
    .global filters_size
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

libidn2:
    .incbin "libidn2.pc"
libidn2_end:
libidn2_size:
    .int libidn2_end - libidn2

libpsl:
    .incbin "libpsl.pc"
libpsl_end:
libpsl_size:
    .int libpsl_end - libpsl

filters:
    .incbin "filters.json"
filters_end:
filters_size:
    .int filters_end - filters
