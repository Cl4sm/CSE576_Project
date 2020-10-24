int verify_init(SERVICE_OPTIONS *section) {
    STACK_OF(X509_NAME) *ca_dn;
    char *ca_name;
    int i;

    if(section->verify_level<0)
        return 0; /* OK - no certificate verification */

    if(section->verify_level>=2 && !section->ca_file && !section->ca_dir) {
        s_log(LOG_ERR,
            "Either CApath or CAfile has to be used for authentication");
        return 1; /* FAILED */
    }

    section->revocation_store=X509_STORE_new();
    if(!section->revocation_store) {
        sslerror("X509_STORE_new");
        return 1; /* FAILED */
    }

    if(section->ca_file) {
        if(!SSL_CTX_load_verify_locations(section->ctx,
                section->ca_file, NULL)) {
            s_log(LOG_ERR, "Error loading verify certificates from %s",
                section->ca_file);
            sslerror("SSL_CTX_load_verify_locations");
            return 1; /* FAILED */
        }
        /* revocation store needs CA certificates for CRL validation */
        if(load_file_lookup(section->revocation_store, section->ca_file))
            return 1; /* FAILED */
        /* trusted CA names sent to clients for client cert selection */
        if(!section->option.client) { /* only performed on server */
            s_log(LOG_DEBUG, "Client CA list: %s",
                section->ca_file);
            ca_dn=SSL_load_client_CA_file(section->ca_file);
            for (i=0; i<sk_X509_NAME_num(ca_dn); ++i) {
                ca_name=X509_NAME2text(sk_X509_NAME_value(ca_dn, i));
                s_log(LOG_INFO, "Client CA: %s", ca_name);
                str_free(ca_name);
            }
            SSL_CTX_set_client_CA_list(section->ctx, ca_dn);
        }
    }

    if(section->ca_dir) {
        if(!SSL_CTX_load_verify_locations(section->ctx,
                NULL, section->ca_dir)) {
            s_log(LOG_ERR, "Error setting verify directory to %s",
                section->ca_dir);
            sslerror("SSL_CTX_load_verify_locations");
            return 1; /* FAILED */
        }
        s_log(LOG_DEBUG, "Verify directory set to %s", section->ca_dir);
        add_dir_lookup(section->revocation_store, section->ca_dir);
    }

    if(section->crl_file)
        if(load_file_lookup(section->revocation_store, section->crl_file))
            return 1; /* FAILED */

    if(section->crl_dir) {
        section->revocation_store->cache=0; /* don't cache CRLs */
        add_dir_lookup(section->revocation_store, section->crl_dir);
    }

    SSL_CTX_set_verify(section->ctx, SSL_VERIFY_PEER |
        (section->verify_level>=2 ? SSL_VERIFY_FAIL_IF_NO_PEER_CERT : 0),
        verify_callback);

    if(section->ca_dir && section->verify_level>=3)
        s_log(LOG_INFO, "Peer certificate location %s", section->ca_dir);
    return 0; /* OK */
}
