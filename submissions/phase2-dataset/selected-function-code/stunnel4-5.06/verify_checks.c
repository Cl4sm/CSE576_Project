    X509 *cert;
    int depth;
    char *subject;

    cert=X509_STORE_CTX_get_current_cert(callback_ctx);
    depth=X509_STORE_CTX_get_error_depth(callback_ctx);
    subject=X509_NAME2text(X509_get_subject_name(cert));

    s_log(LOG_DEBUG, "Verification started at depth=%d: %s", depth, subject);

    if(!cert_check(callback_ctx, preverify_ok)) {
        s_log(LOG_WARNING, "Rejected by CERT at depth=%d: %s", depth, subject);
        str_free(subject);
        return 0; /* fail */
    }
    if(!crl_check(callback_ctx)) {
        s_log(LOG_WARNING, "Rejected by CRL at depth=%d: %s", depth, subject);
        str_free(subject);
        return 0; /* fail */
    }
#ifdef HAVE_OSSL_OCSP_H
    if(!ocsp_check(callback_ctx)) {
        s_log(LOG_WARNING, "Rejected by OCSP at depth=%d: %s", depth, subject);
        str_free(subject);
        return 0; /* fail */
    }
#endif /* HAVE_OSSL_OCSP_H */

    s_log(depth ? LOG_INFO : LOG_NOTICE,
        "Certificate accepted at depth=%d: %s", depth, subject);
    str_free(subject);
    return 1; /* success */
}
