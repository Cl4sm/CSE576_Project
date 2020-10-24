static int do_headers_fixup(request_rec *r, apr_table_t *headers,
                             apr_array_header_t *fixup, int early)
{
    echo_do v;
    int i;
    const char *val;

    for (i = 0; i < fixup->nelts; ++i) {
        header_entry *hdr = &((header_entry *) (fixup->elts))[i];
        const char *envar = hdr->condition_var;

        /* ignore early headers in late calls */
        if (!early && (envar == condition_early)) {
            continue;
        }
        /* ignore late headers in early calls */
        else if (early && (envar != condition_early)) {
            continue;
        }
        /* Do we have an expression to evaluate? */
        else if (hdr->expr != NULL) {
            const char *err = NULL;
            int eval = ap_expr_exec(r, hdr->expr, &err);
            if (err) {
                ap_log_rerror(APLOG_MARK, APLOG_ERR, 0, r, APLOGNO(01501)
                              "Failed to evaluate expression (%s) - ignoring",
                              err);
            }
            else if (!eval) {
                continue;
            }
        }
        /* Have any conditional envar-controlled Header processing to do? */
        else if (envar && !early) {
            if (*envar != '!') {
                if (apr_table_get(r->subprocess_env, envar) == NULL)
                    continue;
            }
            else {
                if (apr_table_get(r->subprocess_env, &envar[1]) != NULL)
                    continue;
            }
        }

        switch (hdr->action) {
        case hdr_add:
            apr_table_addn(headers, hdr->header, process_tags(hdr, r));
            break;
        case hdr_append:
            apr_table_mergen(headers, hdr->header, process_tags(hdr, r));
            break;
        case hdr_merge:
            val = apr_table_get(headers, hdr->header);
            if (val == NULL) {
                apr_table_addn(headers, hdr->header, process_tags(hdr, r));
            } else {
                char *new_val = process_tags(hdr, r);
                apr_size_t new_val_len = strlen(new_val);
                int tok_found = 0;

                /* modified version of logic in ap_get_token() */
                while (*val) {
                    const char *tok_start;

                    while (apr_isspace(*val))
                        ++val;

                    tok_start = val;

                    while (*val && *val != ',') {
                        if (*val++ == '"')
                            while (*val)
                                if (*val++ == '"')
                                    break;
                    }

                    if (new_val_len == (apr_size_t)(val - tok_start)
                        && !strncmp(tok_start, new_val, new_val_len)) {
                        tok_found = 1;
                        break;
                    }

                    if (*val)
                        ++val;
                }

                if (!tok_found) {
                    apr_table_mergen(headers, hdr->header, new_val);
                }
            }
            break;
        case hdr_set:
            if (!strcasecmp(hdr->header, "Content-Type")) {
                 ap_set_content_type(r, process_tags(hdr, r));
            }
            apr_table_setn(headers, hdr->header, process_tags(hdr, r));
            break;
        case hdr_setifempty:
            if (NULL == apr_table_get(headers, hdr->header)) {
                if (!strcasecmp(hdr->header, "Content-Type")) {
                    ap_set_content_type(r, process_tags(hdr, r));
                }
                apr_table_setn(headers, hdr->header, process_tags(hdr, r));
            }
            break;
        case hdr_unset:
            apr_table_unset(headers, hdr->header);
            break;
        case hdr_echo:
            v.r = r;
            v.hdr = hdr;
            apr_table_do((int (*) (void *, const char *, const char *))
                         echo_header, (void *) &v, r->headers_in, NULL);
            break;
        case hdr_edit:
        case hdr_edit_r:
            if (!strcasecmp(hdr->header, "Content-Type") && r->content_type) {
                const char *repl = process_regexp(hdr, r->content_type, r);
                if (repl == NULL)
                    return 0;
                ap_set_content_type(r, repl);
            }
            if (apr_table_get(headers, hdr->header)) {
                edit_do ed;

                ed.r = r;
                ed.hdr = hdr;
                ed.t = apr_table_make(r->pool, 5);
                if (!apr_table_do(edit_header, (void *) &ed, headers,
                                  hdr->header, NULL))
                    return 0;
                apr_table_unset(headers, hdr->header);
                apr_table_do(add_them_all, (void *) headers, ed.t, NULL);
            }
            break;
        case hdr_note:
            apr_table_setn(r->notes, process_tags(hdr, r), apr_table_get(headers, hdr->header));
            break;
 
        }
    }
    return 1;
}