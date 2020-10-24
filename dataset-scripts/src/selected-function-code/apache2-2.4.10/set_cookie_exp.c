static const char *set_cookie_exp(cmd_parms *parms, void *dummy,
                                  const char *arg)
{
    cookie_log_state *cls;
    time_t factor, modifier = 0;
    time_t num = 0;
    char *word;

    cls  = ap_get_module_config(parms->server->module_config,
                                &usertrack_module);
    /* The simple case first - all numbers (we assume) */
    if (apr_isdigit(arg[0]) && apr_isdigit(arg[strlen(arg) - 1])) {
        cls->expires = atol(arg);
        return NULL;
    }

    /*
     * The harder case - stolen from mod_expires
     *
     * CookieExpires "[plus] {<num> <type>}*"
     */

    word = ap_getword_conf(parms->temp_pool, &arg);
    if (!strncasecmp(word, "plus", 1)) {
        word = ap_getword_conf(parms->temp_pool, &arg);
    };

    /* {<num> <type>}* */
    while (word[0]) {
        /* <num> */
        if (apr_isdigit(word[0]))
            num = atoi(word);
        else
            return "bad expires code, numeric value expected.";

        /* <type> */
        word = ap_getword_conf(parms->temp_pool, &arg);
        if (!word[0])
            return "bad expires code, missing <type>";

        if (!strncasecmp(word, "years", 1))
            factor = 60 * 60 * 24 * 365;
        else if (!strncasecmp(word, "months", 2))
            factor = 60 * 60 * 24 * 30;
        else if (!strncasecmp(word, "weeks", 1))
            factor = 60 * 60 * 24 * 7;
        else if (!strncasecmp(word, "days", 1))
            factor = 60 * 60 * 24;
        else if (!strncasecmp(word, "hours", 1))
            factor = 60 * 60;
        else if (!strncasecmp(word, "minutes", 2))
            factor = 60;
        else if (!strncasecmp(word, "seconds", 1))
            factor = 1;
        else
            return "bad expires code, unrecognized type";

        modifier = modifier + factor * num;

        /* next <num> */
        word = ap_getword_conf(parms->temp_pool, &arg);
    }

    cls->expires = modifier;

    return NULL;
}