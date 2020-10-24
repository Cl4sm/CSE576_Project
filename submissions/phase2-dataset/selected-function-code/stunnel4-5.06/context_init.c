int context_init(SERVICE_OPTIONS *section) { /* init SSL context */
    /* create SSL context */
    if(section->option.client)
        section->ctx=SSL_CTX_new(section->client_method);
    else /* server mode */
        section->ctx=SSL_CTX_new(section->server_method);
    if(!section->ctx) {
        sslerror("SSL_CTX_new");
        return 1; /* FAILED */
    }
    SSL_CTX_set_ex_data(section->ctx, opt_index, section); /* for callbacks */

    /* load certificate and private key to be verified by the peer server */
#if defined(HAVE_OSSL_ENGINE_H) && OPENSSL_VERSION_NUMBER>=0x0090809fL
    /* SSL_CTX_set_client_cert_engine() was introduced in OpenSSL 0.9.8i */
    if(section->option.client && section->engine) {
        if(SSL_CTX_set_client_cert_engine(section->ctx, section->engine))
            s_log(LOG_INFO, "Client certificate engine (%s) enabled",
                ENGINE_get_id(section->engine));
        else /* no client certificate functionality in this engine */
            sslerror("SSL_CTX_set_client_cert_engine"); /* ignore error */
    }
#endif
    if(load_cert(section))
        return 1; /* FAILED */

    /* initialize verification of the peer server certificate */
    if(verify_init(section))
        return 1; /* FAILED */

    /* initialize DH/ECDH server mode */
    if(!section->option.client) {
#ifndef OPENSSL_NO_TLSEXT
        SSL_CTX_set_tlsext_servername_arg(section->ctx, section);
        SSL_CTX_set_tlsext_servername_callback(section->ctx, servername_cb);
#endif /* OPENSSL_NO_TLSEXT */
#ifndef OPENSSL_NO_DH
        init_dh(section); /* ignore the result (errors are not critical) */
#endif /* OPENSSL_NO_DH */
#ifndef OPENSSL_NO_ECDH
        init_ecdh(section); /* ignore the result (errors are not critical) */
#endif /* OPENSSL_NO_ECDH */
    }

    /* setup session cache */
    if(!section->option.client) {
        unsigned int servname_len=strlen(section->servname);
        if(servname_len>SSL_MAX_SSL_SESSION_ID_LENGTH)
            servname_len=SSL_MAX_SSL_SESSION_ID_LENGTH;
        if(!SSL_CTX_set_session_id_context(section->ctx,
                (unsigned char *)section->servname, servname_len)) {
            sslerror("SSL_CTX_set_session_id_context");
            return 1; /* FAILED */
        }
    }
    SSL_CTX_set_session_cache_mode(section->ctx,
        SSL_SESS_CACHE_SERVER|SSL_SESS_CACHE_NO_INTERNAL_STORE);
    SSL_CTX_sess_set_cache_size(section->ctx, section->session_size);
    SSL_CTX_set_timeout(section->ctx, section->session_timeout);
    if(section->option.sessiond) {
        SSL_CTX_sess_set_new_cb(section->ctx, sess_new_cb);
        SSL_CTX_sess_set_get_cb(section->ctx, sess_get_cb);
        SSL_CTX_sess_set_remove_cb(section->ctx, sess_remove_cb);
    }

    /* set info callback */
    SSL_CTX_set_info_callback(section->ctx, info_callback);

    /* ciphers, options, mode */
    if(section->cipher_list)
        if(!SSL_CTX_set_cipher_list(section->ctx, section->cipher_list)) {
            sslerror("SSL_CTX_set_cipher_list");
            return 1; /* FAILED */
        }
    SSL_CTX_set_options(section->ctx, section->ssl_options_set);
    SSL_CTX_clear_options(section->ctx, section->ssl_options_clear);
    s_log(LOG_DEBUG, "SSL options: 0x%08lX (+0x%08lX, -0x%08lX)",
        SSL_CTX_get_options(section->ctx),
        section->ssl_options_set, section->ssl_options_clear);
#ifdef SSL_MODE_RELEASE_BUFFERS
    SSL_CTX_set_mode(section->ctx,
        SSL_MODE_ENABLE_PARTIAL_WRITE |
        SSL_MODE_ACCEPT_MOVING_WRITE_BUFFER |
        SSL_MODE_RELEASE_BUFFERS);
#else
    SSL_CTX_set_mode(section->ctx,
        SSL_MODE_ENABLE_PARTIAL_WRITE |
        SSL_MODE_ACCEPT_MOVING_WRITE_BUFFER);
#endif
    return 0; /* OK */
}
