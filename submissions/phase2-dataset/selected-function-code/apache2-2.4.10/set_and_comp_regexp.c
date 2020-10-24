static void set_and_comp_regexp(cookie_dir_rec *dcfg,
                                apr_pool_t *p,
                                const char *cookie_name)
{
    int danger_chars = 0;
    const char *sp = cookie_name;

    /* The goal is to end up with this regexp,
     * ^cookie_name=([^;,]+)|[;,][ \t]+cookie_name=([^;,]+)
     * with cookie_name obviously substituted either
     * with the real cookie name set by the user in httpd.conf, or with the
     * default COOKIE_NAME. */

    /* Anyway, we need to escape the cookie_name before pasting it
     * into the regex
     */
    while (*sp) {
        if (!apr_isalnum(*sp)) {
            ++danger_chars;
        }
        ++sp;
    }

    if (danger_chars) {
        char *cp;
        cp = apr_palloc(p, sp - cookie_name + danger_chars + 1); /* 1 == \0 */
        sp = cookie_name;
        cookie_name = cp;
        while (*sp) {
            if (!apr_isalnum(*sp)) {
                *cp++ = '\\';
            }
            *cp++ = *sp++;
        }
        *cp = '\0';
    }

    dcfg->regexp_string = apr_pstrcat(p, "^",
                                      cookie_name,
                                      "=([^;,]+)|[;,][ \t]*",
                                      cookie_name,
                                      "=([^;,]+)", NULL);

    dcfg->regexp = ap_pregcomp(p, dcfg->regexp_string, AP_REG_EXTENDED);
    ap_assert(dcfg->regexp != NULL);
}