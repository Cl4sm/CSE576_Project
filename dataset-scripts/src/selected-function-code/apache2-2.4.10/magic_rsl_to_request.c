static int magic_rsl_to_request(request_rec *r)
{
    int cur_frag,         /* current fragment number/counter */
        cur_pos,          /* current position within fragment */
        type_frag,        /* content type starting point: fragment */
        type_pos,         /* content type starting point: position */
        type_len,         /* content type length */
        encoding_frag,    /* content encoding starting point: fragment */
        encoding_pos,     /* content encoding starting point: position */
        encoding_len;     /* content encoding length */

    char *tmp;
    magic_rsl *frag;      /* list-traversal pointer */
    rsl_states state;

    magic_req_rec *req_dat = (magic_req_rec *)
                    ap_get_module_config(r->request_config, &mime_magic_module);

    /* check if we have a result */
    if (!req_dat || !req_dat->head) {
        /* empty - no match, we defer to other Apache modules */
        return DECLINED;
    }

    /* start searching for the type and encoding */
    state = rsl_leading_space;
    type_frag = type_pos = type_len = 0;
    encoding_frag = encoding_pos = encoding_len = 0;
    for (frag = req_dat->head, cur_frag = 0;
         frag && frag->next;
         frag = frag->next, cur_frag++) {
        /* loop through the characters in the fragment */
        for (cur_pos = 0; frag->str[cur_pos]; cur_pos++) {
            if (apr_isspace(frag->str[cur_pos])) {
                /* process whitespace actions for each state */
                if (state == rsl_leading_space) {
                    /* eat whitespace in this state */
                    continue;
                }
                else if (state == rsl_type) {
                    /* whitespace: type has no slash! */
                    return DECLINED;
                }
                else if (state == rsl_subtype) {
                    /* whitespace: end of MIME type */
                    state++;
                    continue;
                }
                else if (state == rsl_separator) {
                    /* eat whitespace in this state */
                    continue;
                }
                else if (state == rsl_encoding) {
                    /* whitespace: end of MIME encoding */
                    /* we're done */
                    frag = req_dat->tail;
                    break;
                }
                else {
                    /* should not be possible */
                    /* abandon malfunctioning module */
                    ap_log_rerror(APLOG_MARK, APLOG_ERR, 0, r, APLOGNO(01509)
                                MODNAME ": bad state %d (ws)", state);
                    return DECLINED;
                }
                /* NOTREACHED */
            }
            else if (state == rsl_type &&
                     frag->str[cur_pos] == '/') {
                /* copy the char and go to rsl_subtype state */
                type_len++;
                state++;
            }
            else {
                /* process non-space actions for each state */
                if (state == rsl_leading_space) {
                    /* non-space: begin MIME type */
                    state++;
                    type_frag = cur_frag;
                    type_pos = cur_pos;
                    type_len = 1;
                    continue;
                }
                else if (state == rsl_type ||
                         state == rsl_subtype) {
                    /* non-space: adds to type */
                    type_len++;
                    continue;
                }
                else if (state == rsl_separator) {
                    /* non-space: begin MIME encoding */
                    state++;
                    encoding_frag = cur_frag;
                    encoding_pos = cur_pos;
                    encoding_len = 1;
                    continue;
                }
                else if (state == rsl_encoding) {
                    /* non-space: adds to encoding */
                    encoding_len++;
                    continue;
                }
                else {
                    /* should not be possible */
                    /* abandon malfunctioning module */
                    ap_log_rerror(APLOG_MARK, APLOG_ERR, 0, r, APLOGNO(01510)
                                MODNAME ": bad state %d (ns)", state);
                    return DECLINED;
                }
                /* NOTREACHED */
            }
            /* NOTREACHED */
        }
    }

    /* if we ended prior to state rsl_subtype, we had incomplete info */
    if (state != rsl_subtype && state != rsl_separator &&
        state != rsl_encoding) {
        /* defer to other modules */
        return DECLINED;
    }

    /* save the info in the request record */
    tmp = rsl_strdup(r, type_frag, type_pos, type_len);
    /* XXX: this could be done at config time I'm sure... but I'm
     * confused by all this magic_rsl stuff. -djg */
    ap_content_type_tolower(tmp);
    ap_set_content_type(r, tmp);

    if (state == rsl_encoding) {
        tmp = rsl_strdup(r, encoding_frag,
                                         encoding_pos, encoding_len);
        /* XXX: this could be done at config time I'm sure... but I'm
         * confused by all this magic_rsl stuff. -djg */
        ap_str_tolower(tmp);
        r->content_encoding = tmp;
    }

    /* detect memory allocation or other errors */
    if (!r->content_type ||
        (state == rsl_encoding && !r->content_encoding)) {
        ap_log_rerror(APLOG_MARK, APLOG_ERR, 0, r, APLOGNO(01511)
                      MODNAME ": unexpected state %d; could be caused by bad "
                      "data in magic file",
                      state);
        return HTTP_INTERNAL_SERVER_ERROR;
    }

    /* success! */
    return OK;
}