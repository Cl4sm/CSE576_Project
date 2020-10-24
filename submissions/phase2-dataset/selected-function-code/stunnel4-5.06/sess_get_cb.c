NOEXPORT SSL_SESSION *sess_get_cb(SSL *ssl,
        unsigned char *key, int key_len, int *do_copy) {
    unsigned char *val, *val_tmp=NULL;
    unsigned int val_len=0;
    SSL_SESSION *sess;

    *do_copy = 0; /* allow the session to be freed autmatically */
    cache_transfer(ssl->ctx, CACHE_CMD_GET, 0,
        key, key_len, NULL, 0, &val, &val_len);
    if(!val)
        return NULL;
    val_tmp=val;
    sess=d2i_SSL_SESSION(NULL,
#if OPENSSL_VERSION_NUMBER>=0x0090800fL
        (const unsigned char **)
#endif /* OpenSSL version >= 0.8.0 */
        &val_tmp, val_len);
    str_free(val);
    return sess;
}
