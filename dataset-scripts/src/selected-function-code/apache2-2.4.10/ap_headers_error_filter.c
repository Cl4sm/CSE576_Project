static apr_status_t ap_headers_error_filter(ap_filter_t *f,
                                            apr_bucket_brigade *in)
{
    headers_conf *dirconf;

    dirconf = ap_get_module_config(f->r->per_dir_config,
                                    &headers_module);
    ap_log_error(APLOG_MARK, APLOG_DEBUG, 0, f->r->server, APLOGNO(01503)
                 "headers: ap_headers_error_filter()");

    /*
     * Add any header fields defined by "Header always" to r->err_headers_out.
     * Server-wide first, then per-directory to allow overriding.
     */
    do_headers_fixup(f->r, f->r->err_headers_out, dirconf->fixup_err, 0);

    /*
     * We've done our bit; remove ourself from the filter chain so there's
     * no possibility we'll be called again.
     */
    ap_remove_output_filter(f);

    /*
     * Pass the buck.  (euro?)
     */
    return ap_pass_brigade(f->next, in);
}