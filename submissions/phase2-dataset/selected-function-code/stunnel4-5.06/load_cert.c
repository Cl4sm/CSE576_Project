    /* load the certificate */
    if(section->cert) {
        s_log(LOG_INFO, "Loading cert from file: %s", section->cert);
        if(!SSL_CTX_use_certificate_chain_file(section->ctx, section->cert)) {
            sslerror("SSL_CTX_use_certificate_chain_file");
            return 1; /* FAILED */
        }
    }

    /* load the private key */
    if(!section->key)
        section->key=section->cert;
    if(!section->key) {
        s_log(LOG_DEBUG, "No private key specified");
        return 0; /* OK */
    }
#ifdef HAVE_OSSL_ENGINE_H
    if(section->engine) {
        if(load_key_engine(section))
            return 1; /* FAILED */
    } else
#endif
    {
        if(load_key_file(section))
            return 1; /* FAILED */
    }

    /* validate the private key */
    if(!SSL_CTX_check_private_key(section->ctx)) {
        sslerror("Private key does not match the certificate");
        return 1; /* FAILED */
    }
    s_log(LOG_DEBUG, "Private key check succeeded");
    return 0; /* OK */
}
