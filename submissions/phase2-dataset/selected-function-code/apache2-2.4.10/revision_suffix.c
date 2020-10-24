static int revision_suffix(request_rec *r)
{
    int suffix_pos, result;
    char *sub_filename;
    request_rec *sub;

#if MIME_MAGIC_DEBUG
    ap_log_rerror(APLOG_MARK, APLOG_DEBUG, 0, r, APLOGNO(01555)
                MODNAME ": revision_suffix checking %s", r->filename);
#endif /* MIME_MAGIC_DEBUG */

    /* check for recognized revision suffix */
    suffix_pos = strlen(r->filename) - 1;
    if (!apr_isdigit(r->filename[suffix_pos])) {
        return 0;
    }
    while (suffix_pos >= 0 && apr_isdigit(r->filename[suffix_pos]))
        suffix_pos--;
    if (suffix_pos < 0 || r->filename[suffix_pos] != '@') {
        return 0;
    }

    /* perform sub-request for the file name without the suffix */
    result = 0;
    sub_filename = apr_pstrndup(r->pool, r->filename, suffix_pos);
#if MIME_MAGIC_DEBUG
    ap_log_rerror(APLOG_MARK, APLOG_DEBUG, 0, r, APLOGNO(01556)
                MODNAME ": subrequest lookup for %s", sub_filename);
#endif /* MIME_MAGIC_DEBUG */
    sub = ap_sub_req_lookup_file(sub_filename, r, NULL);

    /* extract content type/encoding/language from sub-request */
    if (sub->content_type) {
        ap_set_content_type(r, apr_pstrdup(r->pool, sub->content_type));
#if MIME_MAGIC_DEBUG
        ap_log_rerror(APLOG_MARK, APLOG_DEBUG, 0, r, APLOGNO(01557)
                    MODNAME ": subrequest %s got %s",
                    sub_filename, r->content_type);
#endif /* MIME_MAGIC_DEBUG */
        if (sub->content_encoding)
            r->content_encoding =
                apr_pstrdup(r->pool, sub->content_encoding);
        if (sub->content_languages) {
            int n;
            r->content_languages = apr_array_copy(r->pool,
                                                  sub->content_languages);
            for (n = 0; n < r->content_languages->nelts; ++n) {
                char **lang = ((char **)r->content_languages->elts) + n;
                *lang = apr_pstrdup(r->pool, *lang);
            }
        }
        result = 1;
    }

    /* clean up */
    ap_destroy_sub_req(sub);

    return result;
}