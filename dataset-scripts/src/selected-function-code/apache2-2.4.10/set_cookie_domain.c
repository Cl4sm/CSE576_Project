static const char *set_cookie_domain(cmd_parms *cmd, void *mconfig,
                                     const char *name)
{
    cookie_dir_rec *dcfg;

    dcfg = (cookie_dir_rec *) mconfig;

    /*
     * Apply the restrictions on cookie domain attributes.
     */
    if (!name[0]) {
        return "CookieDomain values may not be null";
    }
    if (name[0] != '.') {
        return "CookieDomain values must begin with a dot";
    }
    if (ap_strchr_c(&name[1], '.') == NULL) {
        return "CookieDomain values must contain at least one embedded dot";
    }

    dcfg->cookie_domain = name;
    return NULL;
}