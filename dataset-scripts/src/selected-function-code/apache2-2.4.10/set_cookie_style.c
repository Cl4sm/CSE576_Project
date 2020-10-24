static const char *set_cookie_style(cmd_parms *cmd, void *mconfig,
                                    const char *name)
{
    cookie_dir_rec *dcfg;

    dcfg = (cookie_dir_rec *) mconfig;

    if (strcasecmp(name, "Netscape") == 0) {
        dcfg->style = CT_NETSCAPE;
    }
    else if ((strcasecmp(name, "Cookie") == 0)
             || (strcasecmp(name, "RFC2109") == 0)) {
        dcfg->style = CT_COOKIE;
    }
    else if ((strcasecmp(name, "Cookie2") == 0)
             || (strcasecmp(name, "RFC2965") == 0)) {
        dcfg->style = CT_COOKIE2;
    }
    else {
        return apr_psprintf(cmd->pool, "Invalid %s keyword: '%s'",
                            cmd->cmd->name, name);
    }

    return NULL;
}