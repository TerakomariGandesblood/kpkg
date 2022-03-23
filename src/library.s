    .global library
    .global library_size
    .global libcrypto
    .global libcrypto_size
    .global libssl
    .global libssl_size
    .global openssl
    .global openssl_size
    .global libunistring
    .global libunistring_size
    .global filters
    .global filters_size
    .global boringssl
    .global boringssl_size
    .global woff2
    .global woff2_size
    .global sqlcipher
    .global sqlcipher_size
    .global libidn2
    .global libidn2_size
    .global libspng
    .global libspng_size
    .global libvips
    .global libvips_size
    .global glib
    .global glib_size
    .section .rodata
library:
    .incbin "library.json"
library_end:
library_size:
    .int library_end - library

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

filters:
    .incbin "filters.json"
filters_end:
filters_size:
    .int filters_end - filters

boringssl:
    .incbin "0001-expose-ripemd160.patch"
boringssl_end:
boringssl_size:
    .int boringssl_end - boringssl

woff2:
    .incbin "0001-fix-brotli-link.patch"
woff2_end:
woff2_size:
    .int woff2_end - woff2
    
sqlcipher:
    .incbin "0001-fix-OPENSSL_VERSION_TEXT.patch"
sqlcipher_end:
sqlcipher_size:
    .int sqlcipher_end - sqlcipher

libidn2:
    .incbin "0001-remove-HAVE_SYMVER_ALIAS_SUPPORT.patch"
libidn2_end:
libidn2_size:
    .int libidn2_end - libidn2

libspng:
    .incbin "libspng.pc"
libspng_end:
libspng_size:
    .int libspng_end - libspng

libvips:
    .incbin "0001-Add-spngsave.patch"
libvips_end:
libvips_size:
    .int libvips_end - libvips

glib:
    .incbin "0001-fix-link-error.patch"
glib_end:
glib_size:
    .int glib_end - glib
