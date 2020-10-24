static void make_cookie(request_rec *r)
{
    cookie_log_state *cls = ap_get_module_config(r->server->module_config,
                                                 &usertrack_module);
    char cookiebuf[2 * (sizeof(apr_uint64_t) + sizeof(int)) + 2];
    unsigned int random;
    apr_time_t now = r->request_time ? r->request_time : apr_time_now();
    char *new_cookie;
    cookie_dir_rec *dcfg;

    ap_random_insecure_bytes(&random, sizeof(random));
    apr_snprintf(cookiebuf, sizeof(cookiebuf), "%x.%" APR_UINT64_T_HEX_FMT,
                 random, (apr_uint64_t)now);
    dcfg = ap_get_module_config(r->per_dir_config, &usertrack_module);
    if (cls->expires) {

        /* Cookie with date; as strftime '%a, %d-%h-%y %H:%M:%S GMT' */
        new_cookie = apr_psprintf(r->pool, "%s=%s; path=/",
                                  dcfg->cookie_name, cookiebuf);

        if ((dcfg->style == CT_UNSET) || (dcfg->style == CT_NETSCAPE)) {
            apr_time_exp_t tms;
            apr_time_exp_gmt(&tms, r->request_time
                                 + apr_time_from_sec(cls->expires));
            new_cookie = apr_psprintf(r->pool,
                                       "%s; expires=%s, "
                                       "%.2d-%s-%.2d %.2d:%.2d:%.2d GMT",
                                       new_cookie, apr_day_snames[tms.tm_wday],
                                       tms.tm_mday,
                                       apr_month_snames[tms.tm_mon],
                                       tms.tm_year % 100,
                                       tms.tm_hour, tms.tm_min, tms.tm_sec);
        }
        else {
            new_cookie = apr_psprintf(r->pool, "%s; max-age=%d",
                                      new_cookie, cls->expires);
        }
    }
    else {
        new_cookie = apr_psprintf(r->pool, "%s=%s; path=/",
                                  dcfg->cookie_name, cookiebuf);
    }
    if (dcfg->cookie_domain != NULL) {
        new_cookie = apr_pstrcat(r->pool, new_cookie, "; domain=",
                                 dcfg->cookie_domain,
                                 (dcfg->style == CT_COOKIE2
                                  ? "; version=1"
                                  : ""),
                                 NULL);
    }

    apr_table_addn(r->err_headers_out,
                   (dcfg->style == CT_COOKIE2 ? "Set-Cookie2" : "Set-Cookie"),
                   new_cookie);
    apr_table_setn(r->notes, "cookie", apr_pstrdup(r->pool, cookiebuf));   /* log first time */
    return;
}