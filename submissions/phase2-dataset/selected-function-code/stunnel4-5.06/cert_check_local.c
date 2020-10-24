    X509 *cert=X509_STORE_CTX_get_current_cert(callback_ctx);
#if OPENSSL_VERSION_NUMBER>=0x10000000L
    STACK_OF(X509) *sk=X509_STORE_get1_certs(callback_ctx,
        X509_get_subject_name(cert));
    int i;
    if(!sk) {
        sslerror("CERT: X509_STORE_get1_certs");
        return 0; /* fail */
    }
    for(i=0; i<sk_X509_num(sk); i++)
        if(compare_pubkeys(cert, sk_X509_value(sk, i))) {
            sk_X509_pop_free(sk, X509_free);
            return 1; /* success */
        }
    sk_X509_pop_free(sk, X509_free);
#else /* pre-1.0.0 API only returns a single matching certificate */
    X509_OBJECT obj;
    if(X509_STORE_get_by_subject(callback_ctx, X509_LU_X509,
            X509_get_subject_name(cert), &obj)==1 &&
            compare_pubkeys(cert, obj.data.x509))
        return 1; /* success */
#endif
    s_log(LOG_WARNING,
        "CERT: Certificate not found in local repository");
    return 0; /* fail */
}
