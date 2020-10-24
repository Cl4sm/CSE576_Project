NOEXPORT void print_cipher(CLI *c) { /* print negotiated cipher */
    SSL_CIPHER *cipher;
#if !defined(OPENSSL_NO_COMP) && OPENSSL_VERSION_NUMBER>=0x0090800fL
    const COMP_METHOD *compression, *expansion;
#endif

    if(global_options.debug_level<LOG_INFO) /* performance optimization */
        return;
    cipher=(SSL_CIPHER *)SSL_get_current_cipher(c->ssl);
    s_log(LOG_INFO, "Negotiated %s ciphersuite %s (%d-bit encryption)",
        SSL_get_version(c->ssl), SSL_CIPHER_get_name(cipher),
        SSL_CIPHER_get_bits(cipher, NULL));

#if !defined(OPENSSL_NO_COMP) && OPENSSL_VERSION_NUMBER>=0x0090800fL
    compression=SSL_get_current_compression(c->ssl);
    expansion=SSL_get_current_expansion(c->ssl);
    s_log(LOG_INFO, "Compression: %s, expansion: %s",
        compression ? SSL_COMP_get_name(compression) : "null",
        expansion ? SSL_COMP_get_name(expansion) : "null");
#endif
}
