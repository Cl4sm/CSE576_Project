NOEXPORT int verify_callback(int preverify_ok, X509_STORE_CTX *callback_ctx) {
        /* our verify callback function */
    SSL *ssl;
    CLI *c;

    /* retrieve application specific data */
    ssl=X509_STORE_CTX_get_ex_data(callback_ctx,
        SSL_get_ex_data_X509_STORE_CTX_idx());
    c=SSL_get_ex_data(ssl, cli_index);
    if(c->opt->verify_level<1) {
        s_log(LOG_INFO, "Certificate verification disabled");
        return 1; /* accept */
    }
    if(verify_checks(preverify_ok, callback_ctx))
        return 1; /* accept */
    if(c->opt->option.client || c->opt->protocol)
        return 0; /* reject */
    if(c->opt->redirect_addr.names) {
        c->redirect=REDIRECT_ON;
        return 1; /* accept */
    }
    return 0; /* reject */
}
