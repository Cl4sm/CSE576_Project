#ifndef OPENSSL_NO_COMP
    SSL_COMP *comp;
    STACK_OF(SSL_COMP) *ssl_comp_methods;

    ssl_comp_methods=SSL_COMP_get_compression_methods();
    if(!ssl_comp_methods) {
        if(global->compression==COMP_NONE) {
            s_log(LOG_NOTICE, "Failed to get compression methods");
            return 0; /* ignore */
        } else {
            s_log(LOG_ERR, "Failed to get compression methods");
            return 1;
        }
    }

    /* delete OpenSSL defaults (empty the SSL_COMP stack) */
    /* cannot use sk_SSL_COMP_pop_free, as it also destroys the stack itself */
    while(sk_SSL_COMP_num(ssl_comp_methods))
        OPENSSL_free(sk_SSL_COMP_pop(ssl_comp_methods));

    if(global->compression==COMP_NONE) {
        s_log(LOG_DEBUG, "Compression disabled");
        return 0; /* success */
    }

    /* insert RFC 1951 (DEFLATE) algorithm */
    if(SSLeay()>=0x00908051L) { /* 0.9.8e-beta1 */
        /* only allow DEFLATE with OpenSSL 0.9.8 or later
           with openssl #1468 zlib memory leak fixed */
        comp=(SSL_COMP *)OPENSSL_malloc(sizeof(SSL_COMP));
        if(!comp) {
            s_log(LOG_ERR, "OPENSSL_malloc filed");
            return 1;
        }
        comp->id=1; /* RFC 1951 */
        comp->method=COMP_zlib();
        if(!comp->method || comp->method->type==NID_undef) {
            OPENSSL_free(comp);
        } else {
            comp->name=(char *)(comp->method->name);
            sk_SSL_COMP_push(ssl_comp_methods, comp);
        }
    }

    /* also insert one of obsolete (ZLIB/RLE) algorithms */
    comp=(SSL_COMP *)OPENSSL_malloc(sizeof(SSL_COMP));
    if(!comp) {
        s_log(LOG_ERR, "OPENSSL_malloc filed");
        return 1;
    }
    if(global->compression==COMP_ZLIB) {
        comp->id=0xe0; /* 224 - within private range (193 to 255) */
        comp->method=COMP_zlib();
    } else if(global->compression==COMP_RLE) {
        comp->id=0xe1; /* 225 - within private range (193 to 255) */
        comp->method=COMP_rle();
    } else {
	const int count = sk_SSL_COMP_num(ssl_comp_methods);
	OPENSSL_free(comp);
	if(count==0) {
	    s_log(LOG_ERR, "No valid compression algorithms found");
	    return 1;
	}
        s_log(LOG_INFO, "Compression enabled: %d algorithm(s)", count);
        return 0;
    }
    if(!comp->method || comp->method->type==NID_undef) {
        OPENSSL_free(comp);
        s_log(LOG_ERR, "Failed to initialize compression method");
        return 1;
    }
    comp->name=(char *)(comp->method->name);
    sk_SSL_COMP_push(ssl_comp_methods, comp);
    s_log(LOG_INFO, "Compression enabled: %d algorithm(s)",
        sk_SSL_COMP_num(ssl_comp_methods));
#endif /* OPENSSL_NO_COMP */
    return 0; /* success */
}
