NOEXPORT int crl_check(X509_STORE_CTX *callback_ctx) {
    SSL *ssl;
    CLI *c;
    X509_STORE_CTX store_ctx;
    X509_OBJECT obj;
    X509_NAME *subject;
    X509_NAME *issuer;
    X509 *cert;
    X509_CRL *crl;
    X509_REVOKED *revoked;
    EVP_PKEY *pubkey;
    long serial;
    int i, n, rc;
    char *cp;
    ASN1_TIME *last_update=NULL, *next_update=NULL;

    ssl=X509_STORE_CTX_get_ex_data(callback_ctx,
        SSL_get_ex_data_X509_STORE_CTX_idx());
    c=SSL_get_ex_data(ssl, cli_index);
    cert=X509_STORE_CTX_get_current_cert(callback_ctx);
    subject=X509_get_subject_name(cert);
    issuer=X509_get_issuer_name(cert);

    /* try to retrieve a CRL corresponding to the _subject_ of
     * the current certificate in order to verify it's integrity */
    memset((char *)&obj, 0, sizeof obj);
    X509_STORE_CTX_init(&store_ctx, c->opt->revocation_store, NULL, NULL);
    rc=X509_STORE_get_by_subject(&store_ctx, X509_LU_CRL, subject, &obj);
    X509_STORE_CTX_cleanup(&store_ctx);
    crl=obj.data.crl;
    if(rc>0 && crl) {
        cp=X509_NAME2text(subject);
        s_log(LOG_INFO, "CRL: issuer: %s", cp);
        str_free(cp);
        last_update=X509_CRL_get_lastUpdate(crl);
        next_update=X509_CRL_get_nextUpdate(crl);
        log_time(LOG_INFO, "CRL: last update", last_update);
        log_time(LOG_INFO, "CRL: next update", next_update);

        /* verify the signature on this CRL */
        pubkey=X509_get_pubkey(cert);
        if(X509_CRL_verify(crl, pubkey)<=0) {
            s_log(LOG_WARNING, "CRL: Invalid signature");
            X509_STORE_CTX_set_error(callback_ctx,
                X509_V_ERR_CRL_SIGNATURE_FAILURE);
            X509_OBJECT_free_contents(&obj);
            if(pubkey)
                EVP_PKEY_free(pubkey);
            return 0; /* fail */
        }
        if(pubkey)
            EVP_PKEY_free(pubkey);

        /* check date of CRL to make sure it's not expired */
        if(!next_update) {
            s_log(LOG_WARNING, "CRL: Invalid nextUpdate field");
            X509_STORE_CTX_set_error(callback_ctx,
                X509_V_ERR_ERROR_IN_CRL_NEXT_UPDATE_FIELD);
            X509_OBJECT_free_contents(&obj);
            return 0; /* fail */
        }
        if(X509_cmp_current_time(next_update)<0) {
            s_log(LOG_WARNING, "CRL: CRL Expired - revoking all certificates");
            X509_STORE_CTX_set_error(callback_ctx, X509_V_ERR_CRL_HAS_EXPIRED);
            X509_OBJECT_free_contents(&obj);
            return 0; /* fail */
        }
        X509_OBJECT_free_contents(&obj);
    }

    /* try to retrieve a CRL corresponding to the _issuer_ of
     * the current certificate in order to check for revocation */
    memset((char *)&obj, 0, sizeof obj);
    X509_STORE_CTX_init(&store_ctx, c->opt->revocation_store, NULL, NULL);
    rc=X509_STORE_get_by_subject(&store_ctx, X509_LU_CRL, issuer, &obj);
    X509_STORE_CTX_cleanup(&store_ctx);
    crl=obj.data.crl;
    if(rc>0 && crl) {
        /* check if the current certificate is revoked by this CRL */
        n=sk_X509_REVOKED_num(X509_CRL_get_REVOKED(crl));
        for(i=0; i<n; i++) {
            revoked=sk_X509_REVOKED_value(X509_CRL_get_REVOKED(crl), i);
            if(ASN1_INTEGER_cmp(revoked->serialNumber,
                    X509_get_serialNumber(cert))==0) {
                serial=ASN1_INTEGER_get(revoked->serialNumber);
                cp=X509_NAME2text(issuer);
                s_log(LOG_WARNING, "CRL: Certificate with serial %ld (0x%lX) "
                    "revoked per CRL from issuer %s", serial, serial, cp);
                str_free(cp);
                X509_STORE_CTX_set_error(callback_ctx, X509_V_ERR_CERT_REVOKED);
                X509_OBJECT_free_contents(&obj);
                return 0; /* fail */
            }
        }
        X509_OBJECT_free_contents(&obj);
    }
    return 1; /* success */
}
