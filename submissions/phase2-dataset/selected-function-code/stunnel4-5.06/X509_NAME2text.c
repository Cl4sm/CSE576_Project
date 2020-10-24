char *X509_NAME2text(X509_NAME *name) {
    char *text;
    BIO *bio;
    int n;

    bio=BIO_new(BIO_s_mem());
    if(!bio)
        return str_dup("BIO_new() failed");
    X509_NAME_print_ex(bio, name, 0,
        XN_FLAG_ONELINE & ~ASN1_STRFLGS_ESC_MSB & ~XN_FLAG_SPC_EQ);
    n=BIO_pending(bio);
    text=str_alloc(n+1);
    n=BIO_read(bio, text, n);
    if(n<0) {
        BIO_free(bio);
        str_free(text);
        return str_dup("BIO_read() failed");
    }
    text[n]='\0';
    BIO_free(bio);
    safestring(text);
    return text;
}
