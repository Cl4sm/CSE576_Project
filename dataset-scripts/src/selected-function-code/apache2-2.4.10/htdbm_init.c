static apr_status_t htdbm_init(apr_pool_t **pool, htdbm_t **hdbm)
{

#if APR_CHARSET_EBCDIC
    apr_status_t rv;
#endif

    apr_pool_create( pool, NULL);
    apr_pool_abort_set(abort_on_oom, *pool);
    apr_file_open_stderr(&errfile, *pool);
    apr_signal(SIGINT, (void (*)(int)) htdbm_interrupted);

    (*hdbm) = (htdbm_t *)apr_pcalloc(*pool, sizeof(htdbm_t));
    (*hdbm)->ctx.pool = *pool;

#if APR_CHARSET_EBCDIC
    rv = apr_xlate_open(&((*hdbm)->to_ascii), "ISO-8859-1", APR_DEFAULT_CHARSET, (*hdbm)->ctx.pool);
    if (rv) {
        fprintf(stderr, "apr_xlate_open(to ASCII)->%d\n", rv);
        return APR_EGENERAL;
    }
    rv = apr_SHA1InitEBCDIC((*hdbm)->to_ascii);
    if (rv) {
        fprintf(stderr, "apr_SHA1InitEBCDIC()->%d\n", rv);
        return APR_EGENERAL;
    }
    rv = apr_MD5InitEBCDIC((*hdbm)->to_ascii);
    if (rv) {
        fprintf(stderr, "apr_MD5InitEBCDIC()->%d\n", rv);
        return APR_EGENERAL;
    }
#endif /*APR_CHARSET_EBCDIC*/

    /* Set MD5 as default */
    (*hdbm)->ctx.alg = ALG_APMD5;
    (*hdbm)->type = "default";
    return APR_SUCCESS;
}