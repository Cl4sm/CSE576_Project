NOEXPORT DH *read_dh(char *cert) {
    DH *dh;
    BIO *bio;

    if(!cert) {
        s_log(LOG_DEBUG, "No certificate available to load DH parameters");
        return NULL; /* FAILED */
    }
    bio=BIO_new_file(cert, "r");
    if(!bio) {
        sslerror("BIO_new_file");
        return NULL; /* FAILED */
    }
    dh=PEM_read_bio_DHparams(bio, NULL, NULL, NULL);
    BIO_free(bio);
    if(!dh) {
        while(ERR_get_error())
            ; /* OpenSSL error queue cleanup */
        s_log(LOG_DEBUG, "Could not load DH parameters from %s", cert);
        return NULL; /* FAILED */
    }
    s_log(LOG_DEBUG, "Using DH parameters from %s", cert);
    return dh;
}
