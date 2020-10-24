static int set_unique_id(request_rec *r)
{
    const char *id = NULL;
    /* copy the unique_id if this is an internal redirect (we're never
     * actually called for sub requests, so we don't need to test for
     * them) */
    if (r->prev) {
       id = apr_table_get(r->subprocess_env, "REDIRECT_UNIQUE_ID");
    }

    if (!id) {
        /* if we have a log id, it was set by our generate_log_id() function
         * and we should reuse the same id
         */
        id = r->log_id;
    }

    if (!id) {
        id = gen_unique_id(r);
    }

    /* set the environment variable */
    apr_table_setn(r->subprocess_env, "UNIQUE_ID", id);

    return DECLINED;
}