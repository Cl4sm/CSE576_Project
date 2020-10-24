NOEXPORT int cert_check(X509_STORE_CTX *callback_ctx, int preverify_ok) {
    SSL *ssl=X509_STORE_CTX_get_ex_data(callback_ctx,
        SSL_get_ex_data_X509_STORE_CTX_idx());
    CLI *c=SSL_get_ex_data(ssl, cli_index);
    int depth=X509_STORE_CTX_get_error_depth(callback_ctx);

    if(preverify_ok) {
        s_log(LOG_DEBUG, "CERT: preverify ok");
    } else {
        /* remote site sent an invalid certificate */
        if(c->opt->verify_level>=4 && depth>0) {
            s_log(LOG_INFO, "CERT: Invalid CA certificate ignored");
            return 1; /* success */
        } else {
            s_log(LOG_WARNING, "CERT: Verification error: %s",
                X509_verify_cert_error_string(
                    X509_STORE_CTX_get_error(callback_ctx)));
            return 0; /* fail */
        }
    }
    if(c->opt->verify_level>=3 && depth==0)
        if(!cert_check_local(callback_ctx))
            return 0; /* fail */
    return 1; /* success */
}
