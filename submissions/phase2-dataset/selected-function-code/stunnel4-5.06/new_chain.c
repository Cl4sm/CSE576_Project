NOEXPORT void new_chain(CLI *c) {
    BIO *bio;
    int i, len;
    X509 *peer=NULL;
    STACK_OF(X509) *sk;
    char *chain;

    if(c->opt->chain) /* already cached */
        return; /* this race condition is safe to ignore */
    bio=BIO_new(BIO_s_mem());
    if(!bio)
        return;
    sk=SSL_get_peer_cert_chain(c->ssl);
    for(i=0; sk && i<sk_X509_num(sk); i++) {
        peer=sk_X509_value(sk, i);
        PEM_write_bio_X509(bio, peer);
    }
    if(!sk || !c->opt->option.client) {
        peer=SSL_get_peer_certificate(c->ssl);
        if(peer) {
            PEM_write_bio_X509(bio, peer);
            X509_free(peer);
        }
    }
    len=BIO_pending(bio);
    if(len<=0) {
        s_log(LOG_INFO, "No peer certificate received");
        BIO_free(bio);
        return;
    }
    chain=str_alloc(len+1);
    len=BIO_read(bio, chain, len);
    if(len<0) {
        s_log(LOG_ERR, "BIO_read failed");
        BIO_free(bio);
        str_free(chain);
        return;
    }
    chain[len]='\0';
    BIO_free(bio);
    str_detach(chain); /* to prevent automatic deallocation of cached value */
    c->opt->chain=chain; /* this race condition is safe to ignore */
    ui_new_chain(c->opt->section_number);
    s_log(LOG_DEBUG, "Peer certificate was cached (%d bytes)", len);
}
