static char *parse_format_string(cmd_parms *cmd, header_entry *hdr, const char *s)
{
    apr_pool_t *p = cmd->pool;
    char *res;

    /* No string to parse with unset and echo commands */
    if (hdr->action == hdr_unset || hdr->action == hdr_echo) {
        return NULL;
    }
    /* Tags are in the replacement value for edit */
    else if (hdr->action == hdr_edit || hdr->action == hdr_edit_r ) {
        s = hdr->subs;
    }

    if (!strncmp(s, "expr=", 5)) { 
        const char *err;
        hdr->expr_out = ap_expr_parse_cmd(cmd, s+5, 
                                          AP_EXPR_FLAG_STRING_RESULT,
                                          &err, NULL);
        if (err) {
            return apr_pstrcat(cmd->pool,
                    "Can't parse value expression : ", err, NULL);
        }
        return NULL;
    }

    hdr->ta = apr_array_make(p, 10, sizeof(format_tag));

    while (*s) {
        if ((res = parse_format_tag(p, (format_tag *) apr_array_push(hdr->ta), &s))) {
            return res;
        }
    }
    return NULL;
}