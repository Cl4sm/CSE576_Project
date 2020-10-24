NOEXPORT void log_time(const int level, const char *txt, ASN1_TIME *t) {
    char *cp;
    BIO *bio;
    int n;

    if(!t)
        return;
    bio=BIO_new(BIO_s_mem());
    if(!bio)
        return;
    ASN1_TIME_print(bio, t);
    n=BIO_pending(bio);
    cp=str_alloc(n+1);
    n=BIO_read(bio, cp, n);
    if(n<0) {
        BIO_free(bio);
        str_free(cp);
        return;
    }
    cp[n]='\0';
    BIO_free(bio);
    s_log(level, "%s: %s", txt, cp);
    str_free(cp);
}
