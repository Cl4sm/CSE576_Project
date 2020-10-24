static int match_headers(request_rec *r)
{
    sei_cfg_rec *sconf;
    sei_entry *entries;
    const apr_table_entry_t *elts;
    const char *val, *err;
    apr_size_t val_len = 0;
    int i, j;
    char *last_name;
    ap_regmatch_t regm[AP_MAX_REG_MATCH];

    if (!ap_get_module_config(r->request_config, &setenvif_module)) {
        ap_set_module_config(r->request_config, &setenvif_module,
                             SEI_MAGIC_HEIRLOOM);
        sconf  = (sei_cfg_rec *) ap_get_module_config(r->server->module_config,
                                                      &setenvif_module);
    }
    else {
        sconf = (sei_cfg_rec *) ap_get_module_config(r->per_dir_config,
                                                     &setenvif_module);
    }
    entries = (sei_entry *) sconf->conditionals->elts;
    last_name = NULL;
    val = NULL;
    for (i = 0; i < sconf->conditionals->nelts; ++i) {
        sei_entry *b = &entries[i];

        if (!b->expr) {
            /* Optimize the case where a bunch of directives in a row use the
             * same header.  Remember we don't need to strcmp the two header
             * names because we made sure the pointers were equal during
             * configuration.
             */
            if (b->name != last_name) {
                last_name = b->name;
                switch (b->special_type) {
                case SPECIAL_REMOTE_ADDR:
                    val = r->useragent_ip;
                    break;
                case SPECIAL_SERVER_ADDR:
                    val = r->connection->local_ip;
                    break;
                case SPECIAL_REMOTE_HOST:
                    val =  ap_get_remote_host(r->connection, r->per_dir_config,
                                              REMOTE_NAME, NULL);
                    break;
                case SPECIAL_REQUEST_URI:
                    val = r->uri;
                    break;
                case SPECIAL_REQUEST_METHOD:
                    val = r->method;
                    break;
                case SPECIAL_REQUEST_PROTOCOL:
                    val = r->protocol;
                    break;
                case SPECIAL_NOT:
                    if (b->pnamereg) {
                        /* Matching headers_in against a regex. Iterate through
                         * the headers_in until we find a match or run out of
                         * headers.
                         */
                        const apr_array_header_t
                            *arr = apr_table_elts(r->headers_in);

                        elts = (const apr_table_entry_t *) arr->elts;
                        val = NULL;
                        for (j = 0; j < arr->nelts; ++j) {
                            if (!ap_regexec(b->pnamereg, elts[j].key, 0, NULL, 0)) {
                                val = elts[j].val;
                            }
                        }
                    }
                    else {
                        /* Not matching against a regex */
                        val = apr_table_get(r->headers_in, b->name);
                        if (val == NULL) {
                            val = apr_table_get(r->subprocess_env, b->name);
                        }
                    }
                }
                val_len = val ? strlen(val) : 0;
            }

        }

        /*
         * A NULL value indicates that the header field or special entity
         * wasn't present or is undefined.  Represent that as an empty string
         * so that REs like "^$" will work and allow envariable setting
         * based on missing or empty field. This is also necessary to make
         * ap_pregsub work after evaluating an ap_expr_t which does set the
         * regexp backref data.
         */
        if (val == NULL) {
            val = "";
            val_len = 0;
        }

        if ((b->pattern && apr_strmatch(b->pattern, val, val_len)) ||
            (b->preg && !ap_regexec(b->preg, val, AP_MAX_REG_MATCH, regm, 0)) ||
            (b->expr && ap_expr_exec_re(r, b->expr, AP_MAX_REG_MATCH, regm, &val, &err) > 0))
        {
            const apr_array_header_t *arr = apr_table_elts(b->features);
            elts = (const apr_table_entry_t *) arr->elts;

            for (j = 0; j < arr->nelts; ++j) {
                if (*(elts[j].val) == '!') {
                    apr_table_unset(r->subprocess_env, elts[j].key);
                }
                else {
                    if (!b->pattern) {
                        char *replaced = ap_pregsub(r->pool, elts[j].val, val,
                                                    AP_MAX_REG_MATCH, regm);
                        if (replaced) {
                            apr_table_setn(r->subprocess_env, elts[j].key,
                                           replaced);
                        }
                        else {
                            ap_log_rerror(APLOG_MARK, APLOG_CRIT, 0, r, APLOGNO(01505)
                                          "Regular expression replacement "
                                          "failed for '%s', value too long?",
                                          elts[j].key);
                            return HTTP_INTERNAL_SERVER_ERROR;
                        }
                    }
                    else {
                        apr_table_setn(r->subprocess_env, elts[j].key,
                                       elts[j].val);
                    }
                }
                ap_log_rerror(APLOG_MARK, APLOG_TRACE2, 0, r, "Setting %s",
                              elts[j].key);
            }
        }
    }

    return DECLINED;
}