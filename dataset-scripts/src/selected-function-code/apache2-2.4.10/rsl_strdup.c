static char *rsl_strdup(request_rec *r, int start_frag, int start_pos, int len)
{
    char *result;       /* return value */
    int cur_frag,       /* current fragment number/counter */
        cur_pos,        /* current position within fragment */
        res_pos;        /* position in result string */
    magic_rsl *frag;    /* list-traversal pointer */
    magic_req_rec *req_dat = (magic_req_rec *)
                    ap_get_module_config(r->request_config, &mime_magic_module);

    /* allocate the result string */
    result = (char *) apr_palloc(r->pool, len + 1);

    /* loop through and collect the string */
    res_pos = 0;
    for (frag = req_dat->head, cur_frag = 0;
         frag->next;
         frag = frag->next, cur_frag++) {
        /* loop to the first fragment */
        if (cur_frag < start_frag)
            continue;

        /* loop through and collect chars */
        for (cur_pos = (cur_frag == start_frag) ? start_pos : 0;
             frag->str[cur_pos];
             cur_pos++) {
            if (cur_frag >= start_frag
                && cur_pos >= start_pos
                && res_pos <= len) {
                result[res_pos++] = frag->str[cur_pos];
                if (res_pos > len) {
                    break;
                }
            }
        }
    }

    /* clean up and return */
    result[res_pos] = 0;
#if MIME_MAGIC_DEBUG
    ap_log_rerror(APLOG_MARK, APLOG_DEBUG, 0, r, APLOGNO(01508)
             MODNAME ": rsl_strdup() %d chars: %s", res_pos - 1, result);
#endif
    return result;
}