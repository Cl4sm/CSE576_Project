    int i, reason;
    UI_DATA ui_data;
#if !defined(USE_WIN32) && !defined(USE_OS2)
    struct stat st; /* buffer for stat */
#endif

    s_log(LOG_INFO, "Loading key from file: %s", section->key);

#if !defined(USE_WIN32) && !defined(USE_OS2)
    /* check permissions of the private key file */
    if(stat(section->key, &st)) {
        ioerror("Private key file not found");
        return 1; /* FAILED */
    }
    if(st.st_mode & 7)
        s_log(LOG_WARNING, "Insecure file permissions on %s",
            section->key);
#endif

    ui_data.section=section; /* setup current section for callbacks */
#if OPENSSL_VERSION_NUMBER>=0x0090700fL
    SSL_CTX_set_default_passwd_cb(section->ctx, password_cb);
#endif

    for(i=0; i<=3; i++) {
        if(!i && !cache_initialized)
            continue; /* there is no cached value */
        SSL_CTX_set_default_passwd_cb_userdata(section->ctx,
            i ? &ui_data : NULL); /* try the cached password first */
        if(SSL_CTX_use_PrivateKey_file(section->ctx, section->key,
                SSL_FILETYPE_PEM))
            break;
        reason=ERR_GET_REASON(ERR_peek_error());
        if(i<=2 && reason==EVP_R_BAD_DECRYPT) {
            sslerror_queue(); /* dump the error queue */
            s_log(LOG_ERR, "Wrong pass phrase: retrying");
            continue;
        }
        sslerror("SSL_CTX_use_PrivateKey_file");
        return 1; /* FAILED */
    }
    return 0; /* OK */
}
