static int uncompress(request_rec *r, int method,
                      unsigned char **newch, apr_size_t n)
{
    struct uncompress_parms parm;
    apr_file_t *pipe_out = NULL;
    apr_pool_t *sub_context;
    apr_status_t rv;

    parm.r = r;
    parm.method = method;

    /* We make a sub_pool so that we can collect our child early, otherwise
     * there are cases (i.e. generating directory indicies with mod_autoindex)
     * where we would end up with LOTS of zombies.
     */
    if (apr_pool_create(&sub_context, r->pool) != APR_SUCCESS)
        return -1;

    if ((rv = create_uncompress_child(&parm, sub_context, &pipe_out)) != APR_SUCCESS) {
        ap_log_rerror(APLOG_MARK, APLOG_ERR, rv, r, APLOGNO(01553)
                    MODNAME ": couldn't spawn uncompress process: %s", r->uri);
        return -1;
    }

    *newch = (unsigned char *) apr_palloc(r->pool, n);
    rv = apr_file_read(pipe_out, *newch, &n);
    if (n == 0) {
        apr_pool_destroy(sub_context);
        ap_log_rerror(APLOG_MARK, APLOG_ERR, rv, r, APLOGNO(01554)
            MODNAME ": read failed from uncompress of %s", r->filename);
        return -1;
    }
    apr_pool_destroy(sub_context);
    return n;
}