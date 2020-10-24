static int spot_cookie(request_rec *r)
{
    cookie_dir_rec *dcfg = ap_get_module_config(r->per_dir_config,
                                                &usertrack_module);
    const char *cookie_header;
    ap_regmatch_t regm[NUM_SUBS];

    /* Do not run in subrequests */
    if (!dcfg->enabled || r->main) {
        return DECLINED;
    }

    if ((cookie_header = apr_table_get(r->headers_in, "Cookie"))) {
        if (!ap_regexec(dcfg->regexp, cookie_header, NUM_SUBS, regm, 0)) {
            char *cookieval = NULL;
            int err = 0;
            /* Our regexp,
             * ^cookie_name=([^;]+)|;[ \t]+cookie_name=([^;]+)
             * only allows for $1 or $2 to be available. ($0 is always
             * filled with the entire matched expression, not just
             * the part in parentheses.) So just check for either one
             * and assign to cookieval if present. */
            if (regm[1].rm_so != -1) {
                cookieval = ap_pregsub(r->pool, "$1", cookie_header,
                                       NUM_SUBS, regm);
                if (cookieval == NULL)
                    err = 1;
            }
            if (regm[2].rm_so != -1) {
                cookieval = ap_pregsub(r->pool, "$2", cookie_header,
                                       NUM_SUBS, regm);
                if (cookieval == NULL)
                    err = 1;
            }
            if (err) {
                ap_log_rerror(APLOG_MARK, APLOG_CRIT, 0, r, APLOGNO(01499)
                              "Failed to extract cookie value (out of mem?)");
                return HTTP_INTERNAL_SERVER_ERROR;
            }
            /* Set the cookie in a note, for logging */
            apr_table_setn(r->notes, "cookie", cookieval);

            return DECLINED;    /* There's already a cookie, no new one */
        }
    }
    make_cookie(r);
    return OK;                  /* We set our cookie */
}