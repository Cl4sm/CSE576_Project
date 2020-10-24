static char *parse_format_tag(apr_pool_t *p, format_tag *tag, const char **sa)
{
    const char *s = *sa;
    const char * (*tag_handler)(request_rec *,char *);

    /* Handle string literal/conditionals */
    if (*s != '%') {
        return parse_misc_string(p, tag, sa);
    }
    s++; /* skip the % */

    /* Pass through %% or % at end of string as % */
    if ((*s == '%') || (*s == '\0')) {
        tag->func = constant_item;
        tag->arg = "%";
        if (*s)
            s++;
        *sa = s;
        return NULL;
    }

    tag->arg = "\0";
    /* grab the argument if there is one */
    if (*s == '{') {
        ++s;
        tag->arg = ap_getword(p,&s,'}');
    }

    tag_handler = (const char * (*)(request_rec *,char *))apr_hash_get(format_tag_hash, s++, 1);

    if (!tag_handler) {
        char dummy[2];
        dummy[0] = s[-1];
        dummy[1] = '\0';
        return apr_pstrcat(p, "Unrecognized header format %", dummy, NULL);
    }
    tag->func = tag_handler;

    *sa = s;
    return NULL;
}